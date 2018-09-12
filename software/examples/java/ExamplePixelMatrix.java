import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLCD128x64;

public class ExamplePixelMatrix {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your OLED 128x64 Bricklet
	private static final String UID = "XYZ";
	private static final short SCREEN_WIDTH = 128;
	private static final short SCREEN_HEIGHT = 64;

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLCD128x64 lcd = new BrickletLCD128x64(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Clear display
		lcd.clearDisplay();

		// Draw checkerboard pattern
		boolean[] pixels = new boolean[SCREEN_HEIGHT * SCREEN_WIDTH];

		for (short h = 0; h < SCREEN_HEIGHT; h++) {
			for (short w = 0; w < SCREEN_WIDTH; w++) {
				pixels[h*SCREEN_WIDTH + w] = (h / 8) % 2 == (w / 8) % 2;
			}
		}

		lcd.writePixels(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, pixels);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
