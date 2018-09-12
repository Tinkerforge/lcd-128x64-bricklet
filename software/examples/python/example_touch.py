#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your LCD 128x64 Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_lcd_128x64 import BrickletLCD128x64

# Callback function for touch_position callback
def cb_touch_position(pressure, x, y, age):
    print("Pressure: " + str(pressure))
    print("X: " + str(x))
    print("Y: " + str(y))
    print("Age: " + str(age))
    print("")

# Callback function for touch_gesture callback
def cb_touch_gesture(gesture, duration, x_start, x_end, y_start, y_end, age):
    print("Gesture: " + str(gesture))
    print("Duration: " + str(duration))
    print("X Start: " + str(x_start))
    print("X End: " + str(x_end))
    print("Y Start: " + str(y_start))
    print("Y End: " + str(y_end))
    print("Age: " + str(age))
    print("")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    lcd = BrickletLCD128x64(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register touch_position callback to function cb_touch_position
    lcd.register_callback(lcd.CALLBACK_TOUCH_POSITION, cb_touch_position)

    # Register touch_gesture callback to function cb_touch_gesture
    lcd.register_callback(lcd.CALLBACK_TOUCH_GESTURE, cb_touch_gesture)

    # Configure touch position callback with a period of 100ms
    lcd.set_touch_position_callback_configuration(100, True)

    # Configure touch gesture callback with a period of 100ms
    lcd.set_touch_gesture_callback_configuration(100, True)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
