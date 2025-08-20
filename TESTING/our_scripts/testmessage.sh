#!/bin/bash
exec 3<>/dev/tcp/localhost/8080
echo -n "Ayyy pobrecito" >&3
cat <&3
exec 3<&-
