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

# Write "Hello World" starting from upper left corner of the screen
$lcd->write_line(0, 0, "Hello World");

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
