#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your LCD 128x64 Bricklet

# Clear display
tinkerforge call lcd-128x64-bricklet $uid clear-display

# Write "Hello World" starting from upper left corner of the screen
tinkerforge call lcd-128x64-bricklet $uid write-line 0 0 "Hello World"
