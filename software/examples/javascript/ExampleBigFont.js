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

        // Write "Hello World" with big 24x32 font
        lcd.drawText(0, 0, Tinkerforge.BrickletLCD128x64.FONT_24X32,
                     Tinkerforge.BrickletLCD128x64.COLOR_BLACK, '24x32');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
