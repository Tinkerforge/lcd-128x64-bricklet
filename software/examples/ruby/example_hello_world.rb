#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_lcd_128x64'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your LCD 128x64 Bricklet

ipcon = IPConnection.new # Create IP connection
lcd = BrickletLCD128x64.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Clear display
lcd.clear_display

# Write "Hello World" starting from upper left corner of the screen
lcd.write_line 0, 0, 'Hello World'

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
