use std::{error::Error, io};

use tinkerforge::{ip_connection::IpConnection, lcd_128x64_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let lcd = Lcd128x64Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Clear display
    lcd.clear_display().recv()?;

    // Write "Hello World" with big 24x32 font
    lcd.draw_text(0, 0, LCD_128X64_BRICKLET_FONT_24X32, LCD_128X64_BRICKLET_COLOR_BLACK, "24x32".to_string()).recv()?;

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
