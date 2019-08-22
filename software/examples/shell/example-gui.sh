#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your LCD 128x64 Bricklet

# Handle incoming GUI button pressed callbacks
tinkerforge dispatch lcd-128x64-bricklet $uid gui-button-pressed &

# Handle incoming GUI slider value callbacks
tinkerforge dispatch lcd-128x64-bricklet $uid gui-slider-value &

# Handle incoming GUI tab selected callbacks
tinkerforge dispatch lcd-128x64-bricklet $uid gui-tab-selected &

# Clear display
tinkerforge call lcd-128x64-bricklet $uid clear-display
tinkerforge call lcd-128x64-bricklet $uid remove-all-gui

# Add GUI elements: Button, Slider and Graph with 60 data points
tinkerforge call lcd-128x64-bricklet $uid set-gui-button 0 0 0 60 20 "button"
tinkerforge call lcd-128x64-bricklet $uid set-gui-slider 0 0 30 60 direction-horizontal 50
tinkerforge call lcd-128x64-bricklet $uid set-gui-graph-configuration 0 graph-type-line 62 0 60 52 "X" "Y"

# Add a few data points (the remaining points will be 0)
tinkerforge call lcd-128x64-bricklet $uid set-gui-graph-data 0 20,40,60,80,100,120,140,160,180,200,220,240

# Add 5 text tabs without and configure it for click and swipe without auto-redraw
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-configuration change-tab-on-click-and-swipe false
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-text 0 "Tab A"
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-text 1 "Tab B"
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-text 2 "Tab C"
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-text 3 "Tab D"
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-text 4 "Tab E"

# Set period for GUI button pressed callback to 0.1s (100ms)
tinkerforge call lcd-128x64-bricklet $uid set-gui-button-pressed-callback-configuration 100 true

# Set period for GUI slider value callback to 0.1s (100ms)
tinkerforge call lcd-128x64-bricklet $uid set-gui-slider-value-callback-configuration 100 true

# Set period for GUI tab selected callback to 0.1s (100ms)
tinkerforge call lcd-128x64-bricklet $uid set-gui-tab-selected-callback-configuration 100 true

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
