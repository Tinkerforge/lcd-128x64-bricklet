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

    let touch_position_receiver = lcd.get_touch_position_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lcd` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for touch_position in touch_position_receiver {
            println!("Pressure: {}", touch_position.pressure);
            println!("X: {}", touch_position.x);
            println!("Y: {}", touch_position.y);
            println!("Age: {}", touch_position.age);
            println!();
        }
    });

    let touch_gesture_receiver = lcd.get_touch_gesture_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `lcd` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for touch_gesture in touch_gesture_receiver {
            if touch_gesture.gesture == LCD_128X64_BRICKLET_GESTURE_LEFT_TO_RIGHT {
                println!("Gesture: Left To Right");
            } else if touch_gesture.gesture == LCD_128X64_BRICKLET_GESTURE_RIGHT_TO_LEFT {
                println!("Gesture: Right To Left");
            } else if touch_gesture.gesture == LCD_128X64_BRICKLET_GESTURE_TOP_TO_BOTTOM {
                println!("Gesture: Top To Bottom");
            } else if touch_gesture.gesture == LCD_128X64_BRICKLET_GESTURE_BOTTOM_TO_TOP {
                println!("Gesture: Bottom To Top");
            }

            println!("Duration: {}", touch_gesture.duration);
            println!("Pressure Max: {}", touch_gesture.pressure_max);
            println!("X Start: {}", touch_gesture.x_start);
            println!("X End: {}", touch_gesture.x_end);
            println!("Y Start: {}", touch_gesture.y_start);
            println!("Y End: {}", touch_gesture.y_end);
            println!("Age: {}", touch_gesture.age);
            println!();
        }
    });

    // Set period for touch position callback to 0.1s (100ms).
    lcd.set_touch_position_callback_configuration(100, true);

    // Set period for touch gesture callback to 0.1s (100ms).
    lcd.set_touch_gesture_callback_configuration(100, true);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
