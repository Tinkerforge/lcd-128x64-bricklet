package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/lcd_128x64_bricklet"
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

	// Write "Hello World" starting from upper left corner of the screen
	lcd.WriteLine(0, 0, "Hello World")

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
