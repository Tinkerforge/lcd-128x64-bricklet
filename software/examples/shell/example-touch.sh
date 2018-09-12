#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your LCD 128x64 Bricklet

# Handle incoming touch_position callbacks
tinkerforge dispatch lcd-128x64-bricklet $uid touch-position &

# Handle incoming touch_gesture callbacks
tinkerforge dispatch lcd-128x64-bricklet $uid touch-gesture &

# Configure touch position callback with a period of 100ms
tinkerforge call lcd-128x64-bricklet $uid set-touch-position-callback-configuration 100 true

# Configure touch gesture callback with a period of 100ms
tinkerforge call lcd-128x64-bricklet $uid set-touch-gesture-callback-configuration 100 true

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
