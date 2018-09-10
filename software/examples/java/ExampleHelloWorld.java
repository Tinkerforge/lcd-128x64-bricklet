import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLCD128x64;

public class ExampleHelloWorld {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your LCD 128x64 Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLCD128x64 lcd = new BrickletLCD128x64(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Clear display
		lcd.clearDisplay();

		// Write "Hello World" starting from upper left corner of the screen
		lcd.writeLine(0, 0, "Hello World");

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
