using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your LCD 128x64 Bricklet

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLCD128x64 lcd = new BrickletLCD128x64(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Clear display
		lcd.ClearDisplay();

		// Write "Hello World" with big 24x32 font
		lcd.DrawText(0, 0, BrickletLCD128x64.FONT_24X32, BrickletLCD128x64.COLOR_BLACK,
		             "24x32");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
