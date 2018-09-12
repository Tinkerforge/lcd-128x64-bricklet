using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet

	// Callback function for touch_position callback
	static void TouchPositionCB(BrickletLCD128x64 sender, int pressure, int x, int y,
	                            long age)
	{
		Console.WriteLine("Pressure: " + pressure);
		Console.WriteLine("X: " + x);
		Console.WriteLine("Y: " + y);
		Console.WriteLine("Age: " + age);
		Console.WriteLine("");
	}

	// Callback function for touch_gesture callback
	static void TouchGestureCB(BrickletLCD128x64 sender, byte gesture, long duration,
	                           int xStart, int xEnd, int yStart, int yEnd, long age)
	{
		Console.WriteLine("Gesture: " + gesture);
		Console.WriteLine("Duration: " + duration);
		Console.WriteLine("X Start: " + xStart);
		Console.WriteLine("X End: " + xEnd);
		Console.WriteLine("Y Start: " + yStart);
		Console.WriteLine("Y End: " + yEnd);
		Console.WriteLine("Age: " + age);
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLCD128x64 lcd = new BrickletLCD128x64(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register touch_position callback to function TouchPositionCB
		lcd.TouchPositionCallback += TouchPositionCB;

		// Register touch_gesture callback to function TouchGestureCB
		lcd.TouchGestureCallback += TouchGestureCB;

		// Configure touch position callback with a period of 100ms
		lcd.SetTouchPositionCallbackConfiguration(100, true);

		// Configure touch gesture callback with a period of 100ms
		lcd.SetTouchGestureCallbackConfiguration(100, true);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
