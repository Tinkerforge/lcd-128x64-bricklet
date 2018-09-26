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
        // Set period for touch position callback to 0.1s (100ms)
        lcd.setTouchPositionCallbackConfiguration(100, true);

        // Set period for touch gesture callback to 0.1s (100ms)
        lcd.setTouchGestureCallbackConfiguration(100, true);
    }
);

// Register touch position callback
lcd.on(Tinkerforge.BrickletLCD128x64.CALLBACK_TOUCH_POSITION,
    // Callback function for touch position callback
    function (pressure, x, y, age) {
        console.log('Pressure: ' + pressure);
        console.log('X: ' + x);
        console.log('Y: ' + y);
        console.log('Age: ' + age);
        console.log();
    }
);

// Register touch gesture callback
lcd.on(Tinkerforge.BrickletLCD128x64.CALLBACK_TOUCH_GESTURE,
    // Callback function for touch gesture callback
    function (gesture, duration, pressureMax, xStart, xEnd, yStart, yEnd, age) {
        console.log('Gesture: ' + gesture);
        console.log('Duration: ' + duration);
        console.log('Pressure Max: ' + pressureMax);
        console.log('X Start: ' + xStart);
        console.log('X End: ' + xEnd);
        console.log('Y Start: ' + yStart);
        console.log('Y End: ' + yEnd);
        console.log('Age: ' + age);
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
