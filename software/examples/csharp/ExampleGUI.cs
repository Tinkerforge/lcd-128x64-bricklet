using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet

	// Callback function for GUI button pressed callback
	static void GUIButtonPressedCB(BrickletLCD128x64 sender, byte index, bool pressed)
	{
		Console.WriteLine("Index: " + index);
		Console.WriteLine("Pressed: " + pressed);
		Console.WriteLine("");
	}

	// Callback function for GUI slider value callback
	static void GUISliderValueCB(BrickletLCD128x64 sender, byte index, byte value)
	{
		Console.WriteLine("Index: " + index);
		Console.WriteLine("Value: " + value);
		Console.WriteLine("");
	}

	// Callback function for GUI tab selected callback
	static void GUITabSelectedCB(BrickletLCD128x64 sender, short index)
	{
		Console.WriteLine("Index: " + index);
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLCD128x64 lcd = new BrickletLCD128x64(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register GUI button pressed callback to function GUIButtonPressedCB
		lcd.GUIButtonPressedCallback += GUIButtonPressedCB;

		// Register GUI slider value callback to function GUISliderValueCB
		lcd.GUISliderValueCallback += GUISliderValueCB;

		// Register GUI tab selected callback to function GUITabSelectedCB
		lcd.GUITabSelectedCallback += GUITabSelectedCB;

		// Clear display
		lcd.ClearDisplay();
		lcd.RemoveAllGUI();

		// Add GUI elements: Button, Slider and Graph with 60 data points
		lcd.SetGUIButton(0, 0, 0, 60, 20, "button");
		lcd.SetGUISlider(0, 0, 30, 60, BrickletLCD128x64.DIRECTION_HORIZONTAL, 50);
		lcd.SetGUIGraphConfiguration(0, BrickletLCD128x64.GRAPH_TYPE_LINE, 62, 0, 60, 52,
		                             "X", "Y");

		// Add a few data points (the remaining points will be 0)
		lcd.SetGUIGraphData(0,
		                    new byte[]{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240});

		// Add 5 text tabs without and configure it for click and swipe without auto-redraw
		lcd.SetGUITabConfiguration(BrickletLCD128x64.CHANGE_TAB_ON_CLICK_AND_SWIPE,
		                           false);
		lcd.SetGUITabText(0, "Tab A");
		lcd.SetGUITabText(1, "Tab B");
		lcd.SetGUITabText(2, "Tab C");
		lcd.SetGUITabText(3, "Tab D");
		lcd.SetGUITabText(4, "Tab E");

		// Set period for GUI button pressed callback to 0.1s (100ms)
		lcd.SetGUIButtonPressedCallbackConfiguration(100, true);

		// Set period for GUI slider value callback to 0.1s (100ms)
		lcd.SetGUISliderValueCallbackConfiguration(100, true);

		// Set period for GUI tab selected callback to 0.1s (100ms)
		lcd.SetGUITabSelectedCallbackConfiguration(100, true);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
