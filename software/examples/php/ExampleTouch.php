<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLCD128x64.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLCD128x64;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LCD 128x64 Bricklet

// Callback function for touch_position callback
function cb_touchPosition($pressure, $x, $y, $age)
{
    echo "Pressure: $pressure\n";
    echo "X: $x\n";
    echo "Y: $y\n";
    echo "Age: $age\n";
    echo "\n";
}

// Callback function for touch_gesture callback
function cb_touchGesture($gesture, $duration, $x_start, $x_end, $y_start, $y_end, $age)
{
    echo "Gesture: $gesture\n";
    echo "Duration: $duration\n";
    echo "X Start: $x_start\n";
    echo "X End: $x_end\n";
    echo "Y Start: $y_start\n";
    echo "Y End: $y_end\n";
    echo "Age: $age\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$lcd = new BrickletLCD128x64(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register touch_position callback to function cb_touchPosition
$lcd->registerCallback(BrickletLCD128x64::CALLBACK_TOUCH_POSITION, 'cb_touchPosition');

// Register touch_gesture callback to function cb_touchGesture
$lcd->registerCallback(BrickletLCD128x64::CALLBACK_TOUCH_GESTURE, 'cb_touchGesture');

// Configure touch position callback with a period of 100ms
$lcd->setTouchPositionCallbackConfiguration(100, TRUE);

// Configure touch gesture callback with a period of 100ms
$lcd->setTouchGestureCallbackConfiguration(100, TRUE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
