use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, lcd_128x64_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let lcd = Lcd128x64Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let gui_button_pressed_receiver = lcd.get_gui_button_pressed_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lcd` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for gui_button_pressed in gui_button_pressed_receiver {
            println!("Index: {}", gui_button_pressed.index);
            println!("Pressed: {}", gui_button_pressed.pressed);
            println!();
        }
    });

    let gui_slider_value_receiver = lcd.get_gui_slider_value_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lcd` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for gui_slider_value in gui_slider_value_receiver {
            println!("Index: {}", gui_slider_value.index);
            println!("Value: {}", gui_slider_value.value);
            println!();
        }
    });

    let gui_tab_selected_receiver = lcd.get_gui_tab_selected_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lcd` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for gui_tab_selected in gui_tab_selected_receiver {
            println!("Index: {}", gui_tab_selected);
        }
    });

    // Clear display
    lcd.clear_display();
    lcd.remove_all_gui();

    // Add GUI elements: Button, Slider and Graph with 60 data points
    lcd.set_gui_button(0, 0, 0, 60, 20, "button".to_string());
    lcd.set_gui_slider(0, 0, 30, 60, LCD_128X64_BRICKLET_DIRECTION_HORIZONTAL, 50);
    lcd.set_gui_graph_configuration(0, LCD_128X64_BRICKLET_GRAPH_TYPE_LINE, 62, 0, 60, 52, "X".to_string(), "Y".to_string());

    // Add a few data points (the remaining points will be 0)
    lcd.set_gui_graph_data(0, &[20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240])?;

    // Add 5 text tabs without and configure it for click and swipe without auto-redraw
    lcd.set_gui_tab_configuration(LCD_128X64_BRICKLET_CHANGE_TAB_ON_CLICK_AND_SWIPE, false);
    lcd.set_gui_tab_text(0, "Tab A".to_string());
    lcd.set_gui_tab_text(1, "Tab B".to_string());
    lcd.set_gui_tab_text(2, "Tab C".to_string());
    lcd.set_gui_tab_text(3, "Tab D".to_string());
    lcd.set_gui_tab_text(4, "Tab E".to_string());

    // Set period for GUI button pressed callback to 0.1s (100ms).
    lcd.set_gui_button_pressed_callback_configuration(100, true);

    // Set period for GUI slider value callback to 0.1s (100ms).
    lcd.set_gui_slider_value_callback_configuration(100, true);

    // Set period for GUI tab selected callback to 0.1s (100ms).
    lcd.set_gui_tab_selected_callback_configuration(100, true);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
