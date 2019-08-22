<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLCD128x64.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLCD128x64;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LCD 128x64 Bricklet

// Callback function for GUI button pressed callback
function cb_guiButtonPressed($index, $pressed)
{
    echo "Index: $index\n";
    echo "Pressed: $pressed\n";
    echo "\n";
}

// Callback function for GUI slider value callback
function cb_guiSliderValue($index, $value)
{
    echo "Index: $index\n";
    echo "Value: $value\n";
    echo "\n";
}

// Callback function for GUI tab selected callback
function cb_guiTabSelected($index)
{
    echo "Index: $index\n";
}

$ipcon = new IPConnection(); // Create IP connection
$lcd = new BrickletLCD128x64(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register GUI button pressed callback to function cb_guiButtonPressed
$lcd->registerCallback(BrickletLCD128x64::CALLBACK_GUI_BUTTON_PRESSED,
                       'cb_guiButtonPressed');

// Register GUI slider value callback to function cb_guiSliderValue
$lcd->registerCallback(BrickletLCD128x64::CALLBACK_GUI_SLIDER_VALUE, 'cb_guiSliderValue');

// Register GUI tab selected callback to function cb_guiTabSelected
$lcd->registerCallback(BrickletLCD128x64::CALLBACK_GUI_TAB_SELECTED, 'cb_guiTabSelected');

// Clear display
$lcd->clearDisplay();
$lcd->removeAllGUI();

// Add GUI elements: Button, Slider and Graph with 60 data points
$lcd->setGUIButton(0, 0, 0, 60, 20, 'button');
$lcd->setGUISlider(0, 0, 30, 60, BrickletLCD128x64::DIRECTION_HORIZONTAL, 50);
$lcd->setGUIGraphConfiguration(0, BrickletLCD128x64::GRAPH_TYPE_LINE, 62, 0, 60, 52, 'X',
                               'Y');

// Add a few data points (the remaining points will be 0)
$lcd->setGUIGraphData(0, array(20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240));

// Add 5 text tabs without and configure it for click and swipe without auto-redraw
$lcd->setGUITabConfiguration(BrickletLCD128x64::CHANGE_TAB_ON_CLICK_AND_SWIPE, FALSE);
$lcd->setGUITabText(0, 'Tab A');
$lcd->setGUITabText(1, 'Tab B');
$lcd->setGUITabText(2, 'Tab C');
$lcd->setGUITabText(3, 'Tab D');
$lcd->setGUITabText(4, 'Tab E');

// Set period for GUI button pressed callback to 0.1s (100ms)
$lcd->setGUIButtonPressedCallbackConfiguration(100, TRUE);

// Set period for GUI slider value callback to 0.1s (100ms)
$lcd->setGUISliderValueCallbackConfiguration(100, TRUE);

// Set period for GUI tab selected callback to 0.1s (100ms)
$lcd->setGUITabSelectedCallbackConfiguration(100, TRUE);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
