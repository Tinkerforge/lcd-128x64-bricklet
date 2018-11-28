import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLCD128x64;

public class ExampleGUI {
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

		// Add gui button pressed listener
		lcd.addGUIButtonPressedListener(new BrickletLCD128x64.GUIButtonPressedListener() {
			public void guiButtonPressed(int index, boolean pressed) {
				System.out.println("Index: " + index);
				System.out.println("Pressed: " + pressed);
				System.out.println("");
			}
		});

		// Add gui slider value listener
		lcd.addGUISliderValueListener(new BrickletLCD128x64.GUISliderValueListener() {
			public void guiSliderValue(int index, int value) {
				System.out.println("Index: " + index);
				System.out.println("Value: " + value);
				System.out.println("");
			}
		});

		// Add gui tab selected listener
		lcd.addGUITabSelectedListener(new BrickletLCD128x64.GUITabSelectedListener() {
			public void guiTabSelected(int index) {
				System.out.println("Index: " + index);
			}
		});

		// Clear display
		lcd.clearDisplay();
		lcd.removeAllGUI();

		// Add GUI elements: Button, Slider and Graph with 60 data points
		lcd.setGUIButton(0, 0, 0, 60, 20, "button");
		lcd.setGUISlider(0, 0, 30, 60, BrickletLCD128x64.DIRECTION_HORIZONTAL, 50);
		lcd.setGUIGraphConfiguration(0, BrickletLCD128x64.GRAPH_TYPE_LINE, 62, 0, 60, 52,
		                             "X", "Y");

		// Add a few data points (the remaining points will be 0)
		lcd.setGUIGraphData(0,
		                    new int[]{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240});

		// Add 5 text tabs without and configure it for click and swipe without auto-redraw
		lcd.setGUITabConfiguration(BrickletLCD128x64.CHANGE_TAB_ON_CLICK_AND_SWIPE,
		                           false);
		lcd.setGUITabText(0, "Tab A");
		lcd.setGUITabText(1, "Tab B");
		lcd.setGUITabText(2, "Tab C");
		lcd.setGUITabText(3, "Tab D");
		lcd.setGUITabText(4, "Tab E");

		// Set period for gui button pressed callback to 0.1s (100ms)
		lcd.setGUIButtonPressedCallbackConfiguration(100, true);

		// Set period for gui slider value callback to 0.1s (100ms)
		lcd.setGUISliderValueCallbackConfiguration(100, true);

		// Set period for gui tab selected callback to 0.1s (100ms)
		lcd.setGUITabSelectedCallbackConfiguration(100, true);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
