#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_lcd_128x64'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your LCD 128x64 Bricklet
WIDTH = 128 # Columns
HEIGHT = 64 # Rows

ipcon = IPConnection.new # Create IP connection
lcd = BrickletLCD128x64.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Clear display
lcd.clear_display

# Draw checkerboard pattern
pixels = []

for row in 0..HEIGHT - 1
  for column in 0..WIDTH - 1
    pixels[row * WIDTH + column] = (row / 8) % 2 == (column / 8) % 2
  end
end

lcd.write_pixels 0, 0, WIDTH-1, HEIGHT-1, pixels

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
