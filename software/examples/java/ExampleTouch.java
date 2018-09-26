import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLCD128x64;

public class ExampleTouch {
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

		// Add touch position listener
		lcd.addTouchPositionListener(new BrickletLCD128x64.TouchPositionListener() {
			public void touchPosition(int pressure, int x, int y, long age) {
				System.out.println("Pressure: " + pressure);
				System.out.println("X: " + x);
				System.out.println("Y: " + y);
				System.out.println("Age: " + age);
				System.out.println("");
			}
		});

		// Add touch gesture listener
		lcd.addTouchGestureListener(new BrickletLCD128x64.TouchGestureListener() {
			public void touchGesture(int gesture, long duration, int pressureMax,
			                         int xStart, int xEnd, int yStart, int yEnd,
			                         long age) {
				System.out.println("Gesture: " + gesture);
				System.out.println("Duration: " + duration);
				System.out.println("Pressure Max: " + pressureMax);
				System.out.println("X Start: " + xStart);
				System.out.println("X End: " + xEnd);
				System.out.println("Y Start: " + yStart);
				System.out.println("Y End: " + yEnd);
				System.out.println("Age: " + age);
				System.out.println("");
			}
		});

		// Set period for touch position callback to 0.1s (100ms)
		lcd.setTouchPositionCallbackConfiguration(100, true);

		// Set period for touch gesture callback to 0.1s (100ms)
		lcd.setTouchGestureCallbackConfiguration(100, true);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
