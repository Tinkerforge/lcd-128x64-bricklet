#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLCD128x64;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LCD 128x64 Bricklet

# Callback subroutine for touch_position callback
sub cb_touch_position
{
    my ($pressure, $x, $y, $age) = @_;

    print "Pressure: $pressure\n";
    print "X: $x\n";
    print "Y: $y\n";
    print "Age: $age\n";
    print "\n";
}

# Callback subroutine for touch_gesture callback
sub cb_touch_gesture
{
    my ($gesture, $duration, $x_start, $x_end, $y_start, $y_end, $age) = @_;

    print "Gesture: $gesture\n";
    print "Duration: $duration\n";
    print "X Start: $x_start\n";
    print "X End: $x_end\n";
    print "Y Start: $y_start\n";
    print "Y End: $y_end\n";
    print "Age: $age\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lcd = Tinkerforge::BrickletLCD128x64->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register touch_position callback to subroutine cb_touch_position
$lcd->register_callback($lcd->CALLBACK_TOUCH_POSITION, 'cb_touch_position');

# Register touch_gesture callback to subroutine cb_touch_gesture
$lcd->register_callback($lcd->CALLBACK_TOUCH_GESTURE, 'cb_touch_gesture');

# Configure touch position callback with a period of 100ms
$lcd->set_touch_position_callback_configuration(100, 1);

# Configure touch gesture callback with a period of 100ms
$lcd->set_touch_gesture_callback_configuration(100, 1);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
