#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your LCD 128x64 Bricklet

# Clear display
tinkerforge call lcd-128x64-bricklet $uid clear-display

# Write "Hello World" with big 24x32 font
tinkerforge call lcd-128x64-bricklet $uid draw-text 0 0 font-24x32 color-black "24x32"
