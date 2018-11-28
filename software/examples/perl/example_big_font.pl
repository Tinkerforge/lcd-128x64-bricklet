#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLCD128x64;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LCD 128x64 Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lcd = Tinkerforge::BrickletLCD128x64->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Clear display
$lcd->clear_display();

# Write "Hello World" with big 24x32 font
$lcd->draw_text(0, 0, $lcd->FONT_24X32, $lcd->COLOR_BLACK, "24x32");

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
