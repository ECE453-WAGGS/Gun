#!/bin/bash
temp_file=`mktemp`
./detect_laser.py > $temp_file&
./servo_control < $temp_file&