#!/bin/bash
temp_file=`mktemp`
./bluetooth_detect > $temp_file&
./gun_control < $temp_file&
