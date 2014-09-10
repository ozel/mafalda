#!/bin/bash
while true; do 
	nc -u -w 0 $1 8123 < frame_
	echo "new frame:" | tee /dev/stderr | nc -u -w 0 $1 8123 
done
