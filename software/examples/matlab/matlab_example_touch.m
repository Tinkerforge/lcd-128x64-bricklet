function matlab_example_touch()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLCD128x64;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your LCD 128x64 Bricklet

    ipcon = IPConnection(); % Create IP connection
    lcd = handle(BrickletLCD128x64(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register touch position callback to function cb_touch_position
    set(lcd, 'TouchPositionCallback', @(h, e) cb_touch_position(e));

    % Register touch gesture callback to function cb_touch_gesture
    set(lcd, 'TouchGestureCallback', @(h, e) cb_touch_gesture(e));

    % Set period for touch position callback to 0.1s (100ms)
    lcd.setTouchPositionCallbackConfiguration(100, true);

    % Set period for touch gesture callback to 0.1s (100ms)
    lcd.setTouchGestureCallbackConfiguration(100, true);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for touch position callback
function cb_touch_position(e)
    fprintf('Pressure: %i\n', e.pressure);
    fprintf('X: %i\n', e.x);
    fprintf('Y: %i\n', e.y);
    fprintf('Age: %i\n', e.age);
    fprintf('\n');
end

% Callback function for touch gesture callback
function cb_touch_gesture(e)
    fprintf('Gesture: %i\n', e.gesture);
    fprintf('Duration: %i\n', e.duration);
    fprintf('Pressure Max: %i\n', e.pressureMax);
    fprintf('X Start: %i\n', e.xStart);
    fprintf('X End: %i\n', e.xEnd);
    fprintf('Y Start: %i\n', e.yStart);
    fprintf('Y End: %i\n', e.yEnd);
    fprintf('Age: %i\n', e.age);
    fprintf('\n');
end
