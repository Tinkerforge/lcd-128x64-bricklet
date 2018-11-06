use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, lcd_128x64_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let lcd_128x64_bricklet = LCD128x64Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for touch position events.
    let touch_position_listener = lcd_128x64_bricklet.get_touch_position_receiver();
    // Spawn thread to handle received events. This thread ends when the lcd_128x64_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in touch_position_listener {
            println!("Pressure: {}", event.pressure);
            println!("X: {}", event.x);
            println!("Y: {}", event.y);
            println!("Age: {}", event.age);
            println!();
        }
    });

    //Create listener for touch gesture events.
    let touch_gesture_listener = lcd_128x64_bricklet.get_touch_gesture_receiver();
    // Spawn thread to handle received events. This thread ends when the lcd_128x64_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in touch_gesture_listener {
            println!("Gesture: {}", event.gesture);
            println!("Duration: {}", event.duration);
            println!("Pressure Max: {}", event.pressure_max);
            println!("X Start: {}", event.x_start);
            println!("X End: {}", event.x_end);
            println!("Y Start: {}", event.y_start);
            println!("Y End: {}", event.y_end);
            println!("Age: {}", event.age);
            println!();
        }
    });

    // Set period for touch position callback to 0.1s (100ms)
    lcd_128x64_bricklet.set_touch_position_callback_configuration(100, true);

    // Set period for touch gesture callback to 0.1s (100ms)
    lcd_128x64_bricklet.set_touch_gesture_callback_configuration(100, true);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
