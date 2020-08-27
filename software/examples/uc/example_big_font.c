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

	// Write "Hello World" with big 24x32 font
	check(tf_lcd_128x64_draw_text(&lcd, 0, 0, TF_LCD_128X64_FONT_24X32,
	                              TF_LCD_128X64_COLOR_BLACK, "24x32"), "call draw_text");
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
