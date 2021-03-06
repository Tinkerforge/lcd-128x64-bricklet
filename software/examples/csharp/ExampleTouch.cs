using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet

	// Callback function for touch position callback
	static void TouchPositionCB(BrickletLCD128x64 sender, int pressure, int x, int y,
	                            long age)
	{
		Console.WriteLine("Pressure: " + pressure);
		Console.WriteLine("X: " + x);
		Console.WriteLine("Y: " + y);
		Console.WriteLine("Age: " + age);
		Console.WriteLine("");
	}

	// Callback function for touch gesture callback
	static void TouchGestureCB(BrickletLCD128x64 sender, byte gesture, long duration,
	                           int pressureMax, int xStart, int xEnd, int yStart,
	                           int yEnd, long age)
	{
		if(gesture == BrickletLCD128x64.GESTURE_LEFT_TO_RIGHT)
		{
			Console.WriteLine("Gesture: Left To Right");
		}
		else if(gesture == BrickletLCD128x64.GESTURE_RIGHT_TO_LEFT)
		{
			Console.WriteLine("Gesture: Right To Left");
		}
		else if(gesture == BrickletLCD128x64.GESTURE_TOP_TO_BOTTOM)
		{
			Console.WriteLine("Gesture: Top To Bottom");
		}
		else if(gesture == BrickletLCD128x64.GESTURE_BOTTOM_TO_TOP)
		{
			Console.WriteLine("Gesture: Bottom To Top");
		}

		Console.WriteLine("Duration: " + duration);
		Console.WriteLine("Pressure Max: " + pressureMax);
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

		// Register touch position callback to function TouchPositionCB
		lcd.TouchPositionCallback += TouchPositionCB;

		// Register touch gesture callback to function TouchGestureCB
		lcd.TouchGestureCallback += TouchGestureCB;

		// Set period for touch position callback to 0.1s (100ms)
		lcd.SetTouchPositionCallbackConfiguration(100, true);

		// Set period for touch gesture callback to 0.1s (100ms)
		lcd.SetTouchGestureCallbackConfiguration(100, true);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
