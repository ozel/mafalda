#!/bin/bash
while true
do 
	nc -u -w 0 $1 8123 < test.data
	echo "new frame:" | tee /dev/stderr | nc -u -w 0 $1 8123
	sleep 2.1
done
