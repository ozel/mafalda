#!/bin/bash
while true; do 
	nc -u -w 0 ozelipad 8123 < frame_
	echo "new frame:" | tee /dev/stderr | nc -u -w 0 ozelipad 8123 
done
