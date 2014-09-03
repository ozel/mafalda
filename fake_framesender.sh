#!/bin/bash
while true
do 
	cat test.data | nc -u -w 0 ozelipad 8123
	sleep 1
done
