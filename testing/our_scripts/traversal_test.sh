#!/bin/sh
# Path traversal regression test for webserv.
# Run from the repository root after `make`: it starts the server with the
# default configuration, replays attack and legitimate requests, and compares
# each HTTP status code with the expected one.

set -e

CONF="file_conf/default.conf"
PROOF="/tmp/webserv_cgi_proof"
STEPS=13
STEP=0
FAIL=0

# Deep enough to reach / from any working directory
UP="../../../../../../../../../../../../../../../../../../../../"

# Fixtures outside the web roots: if one of them is ever served or run, a check is missing
mkdir -p /tmp/www
printf 'alias index\n' > /tmp/www/test_index.html
printf 'SECRET\n' > /tmp/secret_ws.txt
printf '#!/bin/sh\ntouch %s\nprintf "Content-Type: text/plain\\r\\n\\r\\nexecuted\\n"\n' "$PROOF" > /tmp/test.sh
chmod +x /tmp/test.sh
rm -f "$PROOF"

./webserv "$CONF" > /dev/null 2>&1 &
PID=$!
# Stop the server and remove the fixtures whatever happens
trap 'kill "$PID" 2>/dev/null; rm -f /tmp/secret_ws.txt /tmp/test.sh "$PROOF"' EXIT
sleep 1

report()
{
	STEP=$((STEP + 1))
	if [ "$1" = "$2" ]; then
		printf '[%d/%d] OK    %s -> %s\n' "$STEP" "$STEPS" "$3" "$1"
	else
		printf '[%d/%d] FAIL  %s -> %s (expected %s)\n' "$STEP" "$STEPS" "$3" "$1" "$2"
		FAIL=1
	fi
}

check()
{
	expected="$1"
	label="$2"
	shift 2
	# --path-as-is sends "../" untouched: without it, curl normalises the URL itself
	code=$(curl -s -o /dev/null -w '%{http_code}' --path-as-is --max-time 5 "$@" || true)
	report "$code" "$expected" "$label"
}

echo "Attacks (expected: 403)"
check 403 "GET outside root"        "http://localhost:8080/../../../../etc/passwd"
check 403 "HEAD outside root"       -I "http://localhost:8080/../../../../etc/passwd"
check 403 "DELETE outside root"     -X DELETE "http://localhost:8080/../../../../tmp/secret_ws.txt"
check 403 "POST outside root"       -X POST -H "Content-Type: text/plain" --data x \
                                    "http://localhost:8080/uploads/../../../../tmp/webserv_post_proof"
check 403 "GET alias escape"        "http://localhost:8080/kapouet/../secret_ws.txt"
check 403 "CGI outside root"        "http://localhost:9090/cgi-bin/${UP}tmp/test.sh"
# A 403 is not enough for the CGI: the script must not have run at all
if [ -f "$PROOF" ]; then
	report "executed" "not run" "CGI script outside root"
else
	report "not run" "not run" "CGI script outside root"
fi

echo "Legitimate requests (expected: 200 / 201)"
check 200 "GET /"                   "http://localhost:8080/"
check 200 "GET alias index"         "http://localhost:8080/kapouet/"
check 200 "GET existing upload"     "http://localhost:8080/uploads/test.coucou"
check 200 "CGI inside root"         "http://localhost:9090/cgi-bin/shell/winter.sh"
check 201 "POST new upload"         -X POST -H "Content-Type: text/plain" --data x \
                                    "http://localhost:8080/uploads/traversal_test.txt"
check 200 "DELETE that upload"      -X DELETE "http://localhost:8080/uploads/traversal_test.txt"

exit "$FAIL"
