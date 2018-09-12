var Tinkerforge = require('tinkerforge');
var GM = require('gm'); // FIXME: maybe use node-gd instead
var getPixels = require('get-pixels');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID
var WIDTH = 128;
var HEIGHT = 64;

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var lcd = new Tinkerforge.BrickletLCD128x64(UID, ipcon); // Create device object
var originX = WIDTH / 2;
var originY = HEIGHT / 2;
var length = HEIGHT / 2 - 2;
var angle = 0;

function drawImage(lcd, image) {
    // FIXME: GraphicsMagick doesn't seem to have a way to access the individual
    // pixels. Convert to PNG and then used get-pixels to read the pixels back.
    // This is far from ideal, but better than nothing.
    image.toBuffer('PNG', function (err, buffer) {
        if (err) {
            console.log('toBuffer: ' + err);
            return;
        }

        getPixels(buffer, 'image/png', function(err, pixels) {
            if (err) {
                console.log('getPixels: ' + err);
                return;
            }

            pixels = [];

            for (var h = 0; h < HEIGHT; h++) {
                for (var w = 0; w < WIDTH; w++) {
                    pixels[h*WIDTH + w] = pixels.get(w, h, 0) > 0;
                }
            }

            lcd.writePixels(0, 0, WIDTH-1, HEIGHT-1, pixels);
        });
    });
}

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

        // Draw rotating line
        function render() {
            // FIXME: Creating the image once and reusing it would be better, but
            // on the second call toBuffer() fails with "Stream yields empty buffer"
            var image = GM(WIDTH, HEIGHT, '#000');
            var radians = angle * Math.PI / 180;
            var x = Math.floor(originX + length * Math.cos(radians));
            var y = Math.floor(originY + length * Math.sin(radians));

            image.fill('#FFF').drawLine(originX, originY, x, y);

            drawImage(lcd, image);

            angle++;
        }

        setInterval(render, 25);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
