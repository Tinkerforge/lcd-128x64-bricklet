<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLCD128x64.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLCD128x64;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LCD 128x64 Bricklet
const WIDTH = 128; // Columns
const HEIGHT = 64; // Rows

$ipcon = new IPConnection(); // Create IP connection
$lcd = new BrickletLCD128x64(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Clear display
$lcd->clearDisplay();

// Draw checkerboard pattern
$pixels = array();

for ($y = 0; $y < HEIGHT; $y++)
{
    for ($x = 0; $x < WIDTH; $x++)
    {
        $pixels[$y*WIDTH + $x] = ($y / 8) % 2 == ($x / 8) % 2;
    }
}

$lcd->writePixels(0, 0, WIDTH-1, HEIGHT-1, $pixels);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
