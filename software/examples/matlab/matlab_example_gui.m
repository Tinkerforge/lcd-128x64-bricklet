function matlab_example_gui()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLCD128x64;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your LCD 128x64 Bricklet

    ipcon = IPConnection(); % Create IP connection
    lcd = handle(BrickletLCD128x64(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register gui button pressed callback to function cb_gui_button_pressed
    set(lcd, 'GUIButtonPressedCallback', @(h, e) cb_gui_button_pressed(e));

    % Register gui slider value callback to function cb_gui_slider_value
    set(lcd, 'GUISliderValueCallback', @(h, e) cb_gui_slider_value(e));

    % Register gui tab selected callback to function cb_gui_tab_selected
    set(lcd, 'GUITabSelectedCallback', @(h, e) cb_gui_tab_selected(e));

    % Clear display
    lcd.clearDisplay();
    lcd.removeAllGUI();

    % Add GUI elements: Button, Slider and Graph with 60 data points
    lcd.setGUIButton(0, 0, 0, 60, 20, 'button');
    lcd.setGUISlider(0, 0, 30, 60, BrickletLCD128x64.DIRECTION_HORIZONTAL, 50);
    lcd.setGUIGraphConfiguration(0, BrickletLCD128x64.GRAPH_TYPE_LINE, 62, 0, 60, 52, ...
                                 'X', 'Y');

    % Add a few data points (the remaining points will be 0)
    lcd.setGUIGraphData(0, [20 40 60 80 100 120 140 160 180 200 220 240]);

    % Add 5 text tabs without and configure it for click and swipe without auto-redraw
    lcd.setGUITabConfiguration(BrickletLCD128x64.CHANGE_TAB_ON_CLICK_AND_SWIPE, ...
                               false);
    lcd.setGUITabText(0, 'Tab A');
    lcd.setGUITabText(1, 'Tab B');
    lcd.setGUITabText(2, 'Tab C');
    lcd.setGUITabText(3, 'Tab D');
    lcd.setGUITabText(4, 'Tab E');

    % Set period for gui button pressed callback to 0.1s (100ms)
    lcd.setGUIButtonPressedCallbackConfiguration(100, true);

    % Set period for gui slider value callback to 0.1s (100ms)
    lcd.setGUISliderValueCallbackConfiguration(100, true);

    % Set period for gui tab selected callback to 0.1s (100ms)
    lcd.setGUITabSelectedCallbackConfiguration(100, true);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for gui button pressed callback
function cb_gui_button_pressed(e)
    fprintf('Index: %i\n', e.index);
    fprintf('Pressed: %i\n', e.pressed);
    fprintf('\n');
end

% Callback function for gui slider value callback
function cb_gui_slider_value(e)
    fprintf('Index: %i\n', e.index);
    fprintf('Value: %i\n', e.value);
    fprintf('\n');
end

% Callback function for gui tab selected callback
function cb_gui_tab_selected(e)
    fprintf('Index: %i\n', e.index);
end
