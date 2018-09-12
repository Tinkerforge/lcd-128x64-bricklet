var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your OLED 128x64 Bricklet
var SCREEN_WIDTH = 128;
var SCREEN_HEIGHT = 64;

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

        // Draw checkerboard pattern
        var pixels = [];
        for (var h = 0; h < SCREEN_HEIGHT; h++) {
            for (var w = 0; w < SCREEN_WIDTH; w++) {
                pixels[h*SCREEN_WIDTH + w] = Math.floor(h / 8) % 2 == Math.floor(w / 8) % 2;
            }
        }

        lcd.writePixels(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, pixels);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
