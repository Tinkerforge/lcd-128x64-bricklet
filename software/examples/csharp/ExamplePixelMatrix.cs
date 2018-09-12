using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your OLED 128x64 Bricklet
	private static byte WIDTH = 128;
	private static byte HEIGHT = 64;

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLCD128x64 lcd = new BrickletLCD128x64(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Clear display
		lcd.ClearDisplay();

		// Draw checkerboard pattern
		bool[] pixels = new bool[HEIGHT*WIDTH];

		for (int row = 0; row < HEIGHT; row++)
		{
			for (int column = 0; column < WIDTH; column++)
			{
				pixels[row*WIDTH + column] = (row / 8) % 2 == (column / 8) % 2;
			}
		}
		lcd.WritePixels(0, 0, (byte)(WIDTH-1), (byte)(HEIGHT-1), pixels);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
