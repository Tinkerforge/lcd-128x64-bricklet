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

	lcd.RegisterGUIButtonPressedCallback(func(index uint8, pressed bool) {
		fmt.Printf("Index: %d\n", index)
		fmt.Printf("Pressed: %s\n", pressed)
		fmt.Println()
	})

	lcd.RegisterGUISliderValueCallback(func(index uint8, value uint8) {
		fmt.Printf("Index: %d\n", index)
		fmt.Printf("Value: %d\n", value)
		fmt.Println()
	})

	lcd.RegisterGUITabSelectedCallback(func(index int8) {
		fmt.Printf("Index: %d\n", index)
	})

	// Clear display
	lcd.ClearDisplay()
	lcd.RemoveAllGUI()

	// Add GUI elements: Button, Slider and Graph with 60 data points
	lcd.SetGUIButton(0, 0, 0, 60, 20, "button")
	lcd.SetGUISlider(0, 0, 30, 60, lcd_128x64_bricklet.DirectionHorizontal, 50)
	lcd.SetGUIGraphConfiguration(0, lcd_128x64_bricklet.GraphTypeLine, 62, 0, 60, 52,
		"X", "Y")

	// Add a few data points (the remaining points will be 0)
	lcd.SetGUIGraphData(0,
		[]uint8{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240})

	// Add 5 text tabs without and configure it for click and swipe without auto-redraw
	lcd.SetGUITabConfiguration(lcd_128x64_bricklet.ChangeTabOnClickAndSwipe, false)
	lcd.SetGUITabText(0, "Tab A")
	lcd.SetGUITabText(1, "Tab B")
	lcd.SetGUITabText(2, "Tab C")
	lcd.SetGUITabText(3, "Tab D")
	lcd.SetGUITabText(4, "Tab E")

	// Set period for gui button pressed callback to 0.1s (100ms).
	lcd.SetGUIButtonPressedCallbackConfiguration(100, true)

	// Set period for gui slider value callback to 0.1s (100ms).
	lcd.SetGUISliderValueCallbackConfiguration(100, true)

	// Set period for gui tab selected callback to 0.1s (100ms).
	lcd.SetGUITabSelectedCallbackConfiguration(100, true)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
