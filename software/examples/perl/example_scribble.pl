#!/usr/bin/perl

use strict;
use Math::Trig;
use Term::ReadKey;
use GD;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLCD128x64;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID
use constant WIDTH => 128;
use constant HEIGHT => 64;

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $lcd = Tinkerforge::BrickletLCD128x64->new(&UID, $ipcon); # Create device object

sub draw_image
{
    my ($lcd, $image) = @_;
    my @pixels = ();

    foreach my $row (0..&HEIGHT - 1) {
        foreach my $column (0..&WIDTH - 1) {
            my $index = $image->getPixel($column, $row);
            $pixels[$row*&WIDTH + $column] = $image->rgb($index) > 0;
        }
    }
    $lcd->write_pixels(0, 0, &WIDTH-1, &HEIGHT-1, \@pixels);
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Clear display
$lcd->clear_display();

# Draw rotating line
my $image = new GD::Image(&WIDTH, &HEIGHT);
my $black = $image->colorAllocate(0, 0, 0);
my $white = $image->colorAllocate(255, 255, 255);
my $origin_x = &WIDTH / 2;
my $origin_y = &HEIGHT / 2;
my $length = &HEIGHT / 2 - 2;
my $angle = 0;

print "Press key to exit\n";
ReadMode(4);

while (!defined(ReadKey(-1))) {
    my $radians = pi * $angle / 180.0;
    my $x = int($origin_x + $length * cos($radians));
    my $y = int($origin_y + $length * sin($radians));

    $image->filledRectangle(0, 0, &WIDTH, &HEIGHT, $black);
    $image->line($origin_x, $origin_y, $x, $y, $white);

    draw_image($lcd, $image);
    sleep 0.025;

    $angle++;
}

ReadMode(0);
$ipcon->disconnect();
