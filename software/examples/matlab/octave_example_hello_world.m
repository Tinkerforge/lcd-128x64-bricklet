function octave_example_hello_world()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LCD 128x64 Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    lcd = javaObject("com.tinkerforge.BrickletLCD128x64", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Clear display
    lcd.clearDisplay();

    % Write "Hello World" starting from upper left corner of the screen
    lcd.writeLine(0, 0, "Hello World");

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
