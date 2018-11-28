<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLCD128x64.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLCD128x64;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LCD 128x64 Bricklet

$ipcon = new IPConnection(); // Create IP connection
$lcd = new BrickletLCD128x64(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Clear display
$lcd->clearDisplay();

// Write "Hello World" with big 24x32 font
$lcd->drawText(0, 0, BrickletLCD128x64::FONT_24X32, BrickletLCD128x64::COLOR_BLACK,
               '24x32');

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
