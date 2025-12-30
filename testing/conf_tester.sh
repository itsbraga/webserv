#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Timeout in seconds (if webserv doesn't exit within this time, consider it running)
TIMEOUT=3

# Directory containing config files
CONF_DIR="file_conf_sau6"

# Check if webserv binary exists
if [ ! -f "./webserv" ]; then
    echo -e "${RED}Error: webserv binary not found in current directory${NC}"
    exit 1
fi

# Check if conf directory exists
if [ ! -d "$CONF_DIR" ]; then
    echo -e "${RED}Error: $CONF_DIR directory not found${NC}"
    exit 1
fi

echo -e "${BLUE}=== Testing configuration files ===${NC}\n"

# Counters
total=0
running=0
errors=0

# Loop through all .conf files
for config_file in "$CONF_DIR"/*.conf; do
    # Check if file exists (in case no .conf files found)
    if [ ! -f "$config_file" ]; then
        continue
    fi
    
    total=$((total + 1))
    filename=$(basename "$config_file")
    
    echo -e "${YELLOW}[$total] Testing: $filename${NC}"
    echo -e "Command: ${BLUE}./webserv $config_file${NC}"
    
    # Run webserv in background and capture output
    ./webserv "$config_file" > /tmp/webserv_output_$$.txt 2>&1 &
    pid=$!
    
    # Wait a short time to see if it exits immediately (error) or keeps running (success)
    sleep 0.3
    
    # Check if process is still running
    if kill -0 $pid 2>/dev/null; then
        # Process is still running after 0.3s, consider it successful
        # Kill it since we're just testing
        kill $pid 2>/dev/null
        wait $pid 2>/dev/null
        echo -e "Status: ${GREEN}RUNNING${NC} (server started successfully)"
        running=$((running + 1))
    else
        # Process exited quickly, check exit code
        wait $pid
        exit_code=$?
        if [ $exit_code -eq 0 ]; then
            echo -e "Status: ${GREEN}RUNNING${NC} (exited with code 0)"
            running=$((running + 1))
        else
            echo -e "Status: ${RED}ERROR${NC} (exited with code $exit_code)"
            errors=$((errors + 1))
            # Show error output
            if [ -s /tmp/webserv_output_$$.txt ]; then
                echo -e "${RED}Error output:${NC}"
                head -5 /tmp/webserv_output_$$.txt | sed 's/^/  /'
            fi
        fi
    fi
    
    echo ""
done

# Cleanup
rm -f /tmp/webserv_output_$$.txt

# Summary
echo -e "${BLUE}=== Summary ===${NC}"
echo -e "Total files tested: $total"
echo -e "${GREEN}Running: $running${NC}"
echo -e "${RED}Errors: $errors${NC}"