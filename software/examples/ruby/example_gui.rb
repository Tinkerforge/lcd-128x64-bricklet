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

# Register GUI button pressed callback
lcd.register_callback(BrickletLCD128x64::CALLBACK_GUI_BUTTON_PRESSED) do |index, pressed|
  puts "Index: #{index}"
  puts "Pressed: #{pressed}"
  puts ''
end

# Register GUI slider value callback
lcd.register_callback(BrickletLCD128x64::CALLBACK_GUI_SLIDER_VALUE) do |index, value|
  puts "Index: #{index}"
  puts "Value: #{value}"
  puts ''
end

# Register GUI tab selected callback
lcd.register_callback(BrickletLCD128x64::CALLBACK_GUI_TAB_SELECTED) do |index|
  puts "Index: #{index}"
end

# Clear display
lcd.clear_display
lcd.remove_all_gui

# Add GUI elements: Button, Slider and Graph with 60 data points
lcd.set_gui_button 0, 0, 0, 60, 20, 'button'
lcd.set_gui_slider 0, 0, 30, 60, BrickletLCD128x64::DIRECTION_HORIZONTAL, 50
lcd.set_gui_graph_configuration 0, BrickletLCD128x64::GRAPH_TYPE_LINE, 62, 0, 60, 52, \
                                'X', 'Y'

# Add a few data points (the remaining points will be 0)
lcd.set_gui_graph_data 0, [20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240]

# Add 5 text tabs without and configure it for click and swipe without auto-redraw
lcd.set_gui_tab_configuration BrickletLCD128x64::CHANGE_TAB_ON_CLICK_AND_SWIPE, false
lcd.set_gui_tab_text 0, 'Tab A'
lcd.set_gui_tab_text 1, 'Tab B'
lcd.set_gui_tab_text 2, 'Tab C'
lcd.set_gui_tab_text 3, 'Tab D'
lcd.set_gui_tab_text 4, 'Tab E'

# Set period for GUI button pressed callback to 0.1s (100ms)
lcd.set_gui_button_pressed_callback_configuration 100, true

# Set period for GUI slider value callback to 0.1s (100ms)
lcd.set_gui_slider_value_callback_configuration 100, true

# Set period for GUI tab selected callback to 0.1s (100ms)
lcd.set_gui_tab_selected_callback_configuration 100, true

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
