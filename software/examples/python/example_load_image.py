#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID
WIDTH = 128 # Columns
HEIGHT = 64 # Rows

import sys
from PIL import Image
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_lcd_128x64 import BrickletLCD128x64

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    lcd = BrickletLCD128x64(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Clear display
    lcd.clear_display()

    # Convert image to black/white pixels
    image = Image.open(sys.argv[1])
    image_data = image.load()
    pixels = []

    for row in range(HEIGHT):
        for column in range(WIDTH):
            if column < image.size[0] and row < image.size[1]:
                pixel = image_data[column, row] > 0
            else:
                pixel = False

            pixels.append(pixel)

    lcd.write_pixels(0, 0, WIDTH-1, HEIGHT-1, pixels)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
