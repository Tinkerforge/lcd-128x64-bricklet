var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your LCD 128x64 Bricklet

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var lcd = new Tinkerforge.BrickletLCD128x64(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Clear display
        lcd.clearDisplay();
        lcd.removeAllGUI();

        // Add GUI elements: Button, Slider and Graph with 60 data points
        lcd.setGUIButton(0, 0, 0, 60, 20, 'button');
        lcd.setGUISlider(0, 0, 30, 60,
                         Tinkerforge.BrickletLCD128x64.DIRECTION_HORIZONTAL, 50);
        lcd.setGUIGraphConfiguration(0, Tinkerforge.BrickletLCD128x64.GRAPH_TYPE_LINE,
                                     62, 0, 60, 52, 'X', 'Y');

        // Add a few data points (the remaining points will be 0)
        lcd.setGUIGraphData(0, [20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240]);

        // Add 5 text tabs without and configure it for click and swipe without auto-redraw
        lcd.setGUITabConfiguration(Tinkerforge.BrickletLCD128x64.CHANGE_TAB_ON_CLICK_AND_SWIPE,
                                   false);
        lcd.setGUITabText(0, 'Tab A');
        lcd.setGUITabText(1, 'Tab B');
        lcd.setGUITabText(2, 'Tab C');
        lcd.setGUITabText(3, 'Tab D');
        lcd.setGUITabText(4, 'Tab E');

        // Set period for gui button pressed callback to 0.1s (100ms)
        lcd.setGUIButtonPressedCallbackConfiguration(100, true);

        // Set period for gui slider value callback to 0.1s (100ms)
        lcd.setGUISliderValueCallbackConfiguration(100, true);

        // Set period for gui tab selected callback to 0.1s (100ms)
        lcd.setGUITabSelectedCallbackConfiguration(100, true);
    }
);

// Register gui button pressed callback
lcd.on(Tinkerforge.BrickletLCD128x64.CALLBACK_GUI_BUTTON_PRESSED,
    // Callback function for gui button pressed callback
    function (index, pressed) {
        console.log('Index: ' + index);
        console.log('Pressed: ' + pressed);
        console.log();
    }
);

// Register gui slider value callback
lcd.on(Tinkerforge.BrickletLCD128x64.CALLBACK_GUI_SLIDER_VALUE,
    // Callback function for gui slider value callback
    function (index, value) {
        console.log('Index: ' + index);
        console.log('Value: ' + value);
        console.log();
    }
);

// Register gui tab selected callback
lcd.on(Tinkerforge.BrickletLCD128x64.CALLBACK_GUI_TAB_SELECTED,
    // Callback function for gui tab selected callback
    function (index) {
        console.log('Index: ' + index);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
