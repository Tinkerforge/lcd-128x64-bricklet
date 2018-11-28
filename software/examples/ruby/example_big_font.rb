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

# Write "Hello World" with big 24x32 font
lcd.draw_text 0, 0, BrickletLCD128x64::FONT_24X32, BrickletLCD128x64::COLOR_BLACK, \
              '24x32'

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
