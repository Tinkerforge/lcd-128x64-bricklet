package main

import (
	"fmt"
	"github.com/tinkerforge/go-api-bindings/ipconnection"
	"github.com/tinkerforge/go-api-bindings/lcd_128x64_bricklet"
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

	lcd.RegisterTouchPositionCallback(func(pressure uint16, x uint16, y uint16, age uint32) {
		fmt.Printf("Pressure: %d\n", pressure)
		fmt.Printf("X: %d\n", x)
		fmt.Printf("Y: %d\n", y)
		fmt.Printf("Age: %d\n", age)
		fmt.Println()
	})

	lcd.RegisterTouchGestureCallback(func(gesture lcd_128x64_bricklet.Gesture, duration uint32, pressureMax uint16, xStart uint16, xEnd uint16, yStart uint16, yEnd uint16, age uint32) {

		if gesture == lcd_128x64_bricklet.GestureLeftToRight {
			fmt.Println("Gesture: Left To Right")
		} else if gesture == lcd_128x64_bricklet.GestureRightToLeft {
			fmt.Println("Gesture: Right To Left")
		} else if gesture == lcd_128x64_bricklet.GestureTopToBottom {
			fmt.Println("Gesture: Top To Bottom")
		} else if gesture == lcd_128x64_bricklet.GestureBottomToTop {
			fmt.Println("Gesture: Bottom To Top")
		}

		fmt.Printf("Duration: %d\n", duration)
		fmt.Printf("Pressure Max: %d\n", pressureMax)
		fmt.Printf("X Start: %d\n", xStart)
		fmt.Printf("X End: %d\n", xEnd)
		fmt.Printf("Y Start: %d\n", yStart)
		fmt.Printf("Y End: %d\n", yEnd)
		fmt.Printf("Age: %d\n", age)
		fmt.Println()
	})

	// Set period for touch position callback to 0.1s (100ms).
	lcd.SetTouchPositionCallbackConfiguration(100, true)

	// Set period for touch gesture callback to 0.1s (100ms).
	lcd.SetTouchGestureCallbackConfiguration(100, true)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
