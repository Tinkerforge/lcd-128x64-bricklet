// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_lcd_128x64.h"

#define UID "XYZ" // Change XYZ to the UID of your LCD 128x64 Bricklet

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_LCD128x64 lcd;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_lcd_128x64_create(&lcd, UID, hal), "create device object");

	// Clear display
	check(tf_lcd_128x64_clear_display(&lcd), "call clear_display");

	// Write "Hello World" starting from upper left corner of the screen
	check(tf_lcd_128x64_write_line(&lcd, 0, 0, "Hello World"), "call write_line");
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
