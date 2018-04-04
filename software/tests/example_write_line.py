#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "27WhHz" # Change XYZ to the UID of your OLED 128x64 Bricklet
WIDTH = 128 #
HEIGHT = 64 #

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_lcd_128x64 import BrickletLCD128x64
import time

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    lcd = BrickletLCD128x64(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Clear display
#    lcd.clear_display()

    lcd.write_line(2, 5, "Hello World")
    lcd.write_line(4, 5, "Tinkerforge")
    lcd.write_line(6, 5, "<--------->")

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
