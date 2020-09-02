// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_lcd_128x64.h"

#define UID "XYZ" // Change XYZ to the UID of your LCD 128x64 Bricklet

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for GUI button pressed callback
static void gui_button_pressed_handler(TF_LCD128x64 *device, uint8_t index, bool pressed,
                                       void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Index: %I8u\n", index);
	tf_hal_printf("Pressed: %s\n", pressed ? "true" : "false");
	tf_hal_printf("\n");
}

// Callback function for GUI slider value callback
static void gui_slider_value_handler(TF_LCD128x64 *device, uint8_t index, uint8_t value,
                                     void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Index: %I8u\n", index);
	tf_hal_printf("Value: %I8u\n", value);
	tf_hal_printf("\n");
}

// Callback function for GUI tab selected callback
static void gui_tab_selected_handler(TF_LCD128x64 *device, int8_t index,
                                     void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Index: %I8d\n", index);
}

static TF_LCD128x64 lcd;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_lcd_128x64_create(&lcd, UID, hal), "create device object");

	// Register GUI button pressed callback to function gui_button_pressed_handler
	tf_lcd_128x64_register_gui_button_pressed_callback(&lcd,
	                                                   gui_button_pressed_handler,
	                                                   NULL);

	// Register GUI slider value callback to function gui_slider_value_handler
	tf_lcd_128x64_register_gui_slider_value_callback(&lcd,
	                                                 gui_slider_value_handler,
	                                                 NULL);

	// Register GUI tab selected callback to function gui_tab_selected_handler
	tf_lcd_128x64_register_gui_tab_selected_callback(&lcd,
	                                                 gui_tab_selected_handler,
	                                                 NULL);

	// Clear display
	check(tf_lcd_128x64_clear_display(&lcd), "call clear_display");
	check(tf_lcd_128x64_remove_all_gui(&lcd), "call remove_all_gui");

	// Add GUI elements: Button, Slider and Graph with 60 data points
	check(tf_lcd_128x64_set_gui_button(&lcd, 0, 0, 0, 60, 20,
	                                   "button"), "call set_gui_button");
	check(tf_lcd_128x64_set_gui_slider(&lcd, 0, 0, 30, 60,
	                                   TF_LCD_128X64_DIRECTION_HORIZONTAL,
	                                   50), "call set_gui_slider");
	check(tf_lcd_128x64_set_gui_graph_configuration(&lcd, 0,
	                                                TF_LCD_128X64_GRAPH_TYPE_LINE, 62, 0,
	                                                60, 52, "X",
	                                                "Y"), "call set_gui_graph_configuration");

	// Add a few data points (the remaining points will be 0)
	uint8_t data[12] = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240};
	check(tf_lcd_128x64_set_gui_graph_data(&lcd, 0, data, 12), "call set_gui_graph_data");

	// Add 5 text tabs without and configure it for click and swipe without auto-redraw
	check(tf_lcd_128x64_set_gui_tab_configuration(&lcd,
	                                              TF_LCD_128X64_CHANGE_TAB_ON_CLICK_AND_SWIPE,
	                                              false), "call set_gui_tab_configuration");
	check(tf_lcd_128x64_set_gui_tab_text(&lcd, 0, "Tab A"), "call set_gui_tab_text");
	check(tf_lcd_128x64_set_gui_tab_text(&lcd, 1, "Tab B"), "call set_gui_tab_text");
	check(tf_lcd_128x64_set_gui_tab_text(&lcd, 2, "Tab C"), "call set_gui_tab_text");
	check(tf_lcd_128x64_set_gui_tab_text(&lcd, 3, "Tab D"), "call set_gui_tab_text");
	check(tf_lcd_128x64_set_gui_tab_text(&lcd, 4, "Tab E"), "call set_gui_tab_text");

	// Set period for GUI button pressed callback to 0.1s (100ms)
	tf_lcd_128x64_set_gui_button_pressed_callback_configuration(&lcd, 100, true);

	// Set period for GUI slider value callback to 0.1s (100ms)
	tf_lcd_128x64_set_gui_slider_value_callback_configuration(&lcd, 100, true);

	// Set period for GUI tab selected callback to 0.1s (100ms)
	tf_lcd_128x64_set_gui_tab_selected_callback_configuration(&lcd, 100, true);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
