package main

import (
	"fmt"
	"tinkerforge/ipconnection"
	"tinkerforge/lcd_128x64_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your LCD 128x64 Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	lcd, _ := lcd_128x64_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Clear display
	lcd.ClearDisplay()

	// Write "Hello World" with big 24x32 font
	lcd.DrawText(0, 0, lcd_128x64_bricklet.Font24x32, lcd_128x64_bricklet.ColorBlack,
		"24x32")

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
