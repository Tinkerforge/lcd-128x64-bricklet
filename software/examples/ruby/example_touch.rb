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

# Register touch position callback
lcd.register_callback(BrickletLCD128x64::CALLBACK_TOUCH_POSITION) do |pressure, x, y, age|
  puts "Pressure: #{pressure}"
  puts "X: #{x}"
  puts "Y: #{y}"
  puts "Age: #{age}"
  puts ''
end

# Register touch gesture callback
lcd.register_callback(BrickletLCD128x64::CALLBACK_TOUCH_GESTURE) do |gesture, duration,
                                                                     pressure_max,
                                                                     x_start, x_end,
                                                                     y_start, y_end, age|
  if gesture == BrickletLCD128x64::GESTURE_LEFT_TO_RIGHT
    puts "Gesture: Left To Right"
  elsif gesture == BrickletLCD128x64::GESTURE_RIGHT_TO_LEFT
    puts "Gesture: Right To Left"
  elsif gesture == BrickletLCD128x64::GESTURE_TOP_TO_BOTTOM
    puts "Gesture: Top To Bottom"
  elsif gesture == BrickletLCD128x64::GESTURE_BOTTOM_TO_TOP
    puts "Gesture: Bottom To Top"
  end

  puts "Duration: #{duration}"
  puts "Pressure Max: #{pressure_max}"
  puts "X Start: #{x_start}"
  puts "X End: #{x_end}"
  puts "Y Start: #{y_start}"
  puts "Y End: #{y_end}"
  puts "Age: #{age}"
  puts ''
end

# Set period for touch position callback to 0.1s (100ms)
lcd.set_touch_position_callback_configuration 100, true

# Set period for touch gesture callback to 0.1s (100ms)
lcd.set_touch_gesture_callback_configuration 100, true

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
