function octave_example_touch()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LCD 128x64 Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    lcd = javaObject("com.tinkerforge.BrickletLCD128x64", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register touch position callback to function cb_touch_position
    lcd.addTouchPositionCallback(@cb_touch_position);

    % Register touch gesture callback to function cb_touch_gesture
    lcd.addTouchGestureCallback(@cb_touch_gesture);

    % Set period for touch position callback to 0.1s (100ms)
    lcd.setTouchPositionCallbackConfiguration(100, true);

    % Set period for touch gesture callback to 0.1s (100ms)
    lcd.setTouchGestureCallbackConfiguration(100, true);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for touch position callback
function cb_touch_position(e)
    fprintf("Pressure: %d\n", e.pressure);
    fprintf("X: %d\n", e.x);
    fprintf("Y: %d\n", e.y);
    fprintf("Age: %d\n", java2int(e.age));
    fprintf("\n");
end

% Callback function for touch gesture callback
function cb_touch_gesture(e)
    fprintf("Gesture: %d\n", e.gesture);
    fprintf("Duration: %d\n", java2int(e.duration));
    fprintf("X Start: %d\n", e.xStart);
    fprintf("X End: %d\n", e.xEnd);
    fprintf("Y Start: %d\n", e.yStart);
    fprintf("Y End: %d\n", e.yEnd);
    fprintf("Age: %d\n", java2int(e.age));
    fprintf("\n");
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
