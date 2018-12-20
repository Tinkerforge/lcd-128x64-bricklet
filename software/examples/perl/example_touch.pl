#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLCD128x64;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LCD 128x64 Bricklet

# Callback subroutine for touch position callback
sub cb_touch_position
{
    my ($pressure, $x, $y, $age) = @_;

    print "Pressure: $pressure\n";
    print "X: $x\n";
    print "Y: $y\n";
    print "Age: $age\n";
    print "\n";
}

# Callback subroutine for touch gesture callback
sub cb_touch_gesture
{
    my ($gesture, $duration, $pressure_max, $x_start, $x_end, $y_start, $y_end,
        $age) = @_;

    if ($gesture == Tinkerforge::BrickletLCD128x64->GESTURE_LEFT_TO_RIGHT)
    {
        print "Gesture: Left To Right\n";
    }
    elsif ($gesture == Tinkerforge::BrickletLCD128x64->GESTURE_RIGHT_TO_LEFT)
    {
        print "Gesture: Right To Left\n";
    }
    elsif ($gesture == Tinkerforge::BrickletLCD128x64->GESTURE_TOP_TO_BOTTOM)
    {
        print "Gesture: Top To Bottom\n";
    }
    elsif ($gesture == Tinkerforge::BrickletLCD128x64->GESTURE_BOTTOM_TO_TOP)
    {
        print "Gesture: Bottom To Top\n";
    }

    print "Duration: $duration\n";
    print "Pressure Max: $pressure_max\n";
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

# Register touch position callback to subroutine cb_touch_position
$lcd->register_callback($lcd->CALLBACK_TOUCH_POSITION, 'cb_touch_position');

# Register touch gesture callback to subroutine cb_touch_gesture
$lcd->register_callback($lcd->CALLBACK_TOUCH_GESTURE, 'cb_touch_gesture');

# Set period for touch position callback to 0.1s (100ms)
$lcd->set_touch_position_callback_configuration(100, 1);

# Set period for touch gesture callback to 0.1s (100ms)
$lcd->set_touch_gesture_callback_configuration(100, 1);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
