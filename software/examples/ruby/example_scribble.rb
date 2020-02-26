#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'rgd'
require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_lcd_128x64'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your LCD 128x64 Bricklet
WIDTH = 128 # Columns
HEIGHT = 64 # Rows

def draw_image(lcd, start_column, start_row, column_count, row_count, image)
  pixels = []

  # Convert image pixels into 8bit pages
  for row in 0..row_count - 1

    for column in 0..column_count - 1
      index = image[column, row]
      rgba = image.rgba index
	  pixels[row*column_count + column] = rgba[0] > 0
    end
  end

  lcd.write_pixels 0, 0, WIDTH-1, HEIGHT-1, pixels
end

ipcon = IPConnection.new # Create IP connection
lcd = BrickletLCD128x64.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Clear display
lcd.clear_display

# Draw checkerboard pattern
image = RGD::Image.create WIDTH, HEIGHT
black = image.color_allocate 0, 0, 0
white = image.color_allocate 255, 255, 255
origin_x = WIDTH / 2
origin_y = HEIGHT / 2
length = HEIGHT / 2 - 2
angle = 0

puts 'Press key to exit'

Thread.new do
  $stdin.gets
  exit
end

while true
    radians = Math::PI * angle / 180.0
    x = (origin_x + length * Math.cos(radians)).to_i
    y = (origin_y + length * Math.sin(radians)).to_i

    image.filled_rectangle 0, 0, WIDTH, HEIGHT, black
    image.line origin_x, origin_y, x, y, white

    draw_image lcd, 0, 0, WIDTH, HEIGHT, image
    sleep 0.025

    angle += 1
end
