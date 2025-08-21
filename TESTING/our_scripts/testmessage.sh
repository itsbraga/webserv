#!/bin/bash

exec 3<>/dev/tcp/localhost/8080
printf "GET / HTTP/1.1\r\n" >&3
printf "Host: localhost\r\n" >&3
printf "Connection: close" >&3
printf "\r\n\r\n" >&3
cat <&3
exec 3<&-