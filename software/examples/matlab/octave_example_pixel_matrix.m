function octave_example_pixel_matrix()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LCd 128x64 Bricklet
    SCREEN_WIDTH = 128;
    SCREEN_HEIGHT = 64;

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    lcd = javaObject("com.tinkerforge.BrickletLCD128x64", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Clear display
    lcd.clearDisplay();

    % Draw checkerboard pattern
    for h = 0:SCREEN_HEIGHT-1
        for w = 0:SCREEN_WIDTH-1
            pixels(h*SCREEN_WIDTH + w + 1) = mod(floor(h / 8), 2) == mod(floor(w / 8), 2);
        end
    end

	lcd.writePixels(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, pixels);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
