#!/bin/bash
# Connects to localhost:4223 by default, use --host and --port to change this

# This example requires Bash 4

uid=XYZ # Change XYZ to the UID of your LCD 128x64 Bricklet

SCREEN_WIDTH=128
SCREEN_HEIGHT=64

# Clear display
tinkerforge call lcd-128x64-bricklet $uid clear-display

# Draw checkerboard pattern
pixels=""

for ((h=0;h<${SCREEN_HEIGHT};h++))
do
	for ((w=0;w<${SCREEN_WIDTH};w++))
	do
		if [ $((((${h}/8))%2==((${w}/8))%2)) -gt 0  ]; then
			pixels+="true,"
		else
			pixels+="false,"
		fi	
	done
done
	
w=`expr $SCREEN_WIDTH - 1`
h=`expr $SCREEN_HEIGHT - 1`
tinkerforge call lcd-128x64-bricklet ${uid} write-pixels 0 0 $w $h ${pixels::-1}
