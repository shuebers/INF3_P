#!/bin/sh
echo "Enter port between 1024 and 50000"
read port
echo "Enter cracking algorithm (-r, -s)"
read alg
./client $port $alg & 
./server $port &
