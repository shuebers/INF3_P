#!/bin/sh
echo "Enter port between 1024 and 50000"
read port
echo "enter password length"
read leng
echo "enter amount of symbols"
read symb
echo "Enter cracking algorithm (r, s)"
read algo
./client $port $leng $symb -$algo & 
./server $port &
