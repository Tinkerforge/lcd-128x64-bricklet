

#include "bindings/hal_common.h"
#include "bindings/bricklet_lcd_128x64.h"

#define UID "XYZ" // Change XYZ to the UID of your LCD 128x64 Bricklet

void check(int rc, const char* msg);



// Callback function for touch position callback
void touch_position_handler(TF_LCD128x64 *device, uint16_t pressure, uint16_t x,
                            uint16_t y, uint32_t age, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Pressure: %u\n", pressure);
	tf_hal_printf("X: %u\n", x);
	tf_hal_printf("Y: %u\n", y);
	tf_hal_printf("Age: %u\n", age);
	tf_hal_printf("\n");
}

// Callback function for touch gesture callback
void touch_gesture_handler(TF_LCD128x64 *device, uint8_t gesture, uint32_t duration,
                           uint16_t pressure_max, uint16_t x_start, uint16_t x_end,
                           uint16_t y_start, uint16_t y_end, uint32_t age,
                           void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	if(gesture == TF_LCD_128X64_GESTURE_LEFT_TO_RIGHT) {
		tf_hal_printf("Gesture: Left To Right\n");
	} else if(gesture == TF_LCD_128X64_GESTURE_RIGHT_TO_LEFT) {
		tf_hal_printf("Gesture: Right To Left\n");
	} else if(gesture == TF_LCD_128X64_GESTURE_TOP_TO_BOTTOM) {
		tf_hal_printf("Gesture: Top To Bottom\n");
	} else if(gesture == TF_LCD_128X64_GESTURE_BOTTOM_TO_TOP) {
		tf_hal_printf("Gesture: Bottom To Top\n");
	}

	tf_hal_printf("Duration: %u\n", duration);
	tf_hal_printf("Pressure Max: %u\n", pressure_max);
	tf_hal_printf("X Start: %u\n", x_start);
	tf_hal_printf("X End: %u\n", x_end);
	tf_hal_printf("Y Start: %u\n", y_start);
	tf_hal_printf("Y End: %u\n", y_end);
	tf_hal_printf("Age: %u\n", age);
	tf_hal_printf("\n");
}


TF_LCD128x64 lcd;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_lcd_128x64_create(&lcd, UID, hal), "create device object");


	// Register touch position callback to function touch_position_handler
	tf_lcd_128x64_register_touch_position_callback(&lcd,
	                                              touch_position_handler,
	                                              NULL);

	// Register touch gesture callback to function touch_gesture_handler
	tf_lcd_128x64_register_touch_gesture_callback(&lcd,
	                                             touch_gesture_handler,
	                                             NULL);

	// Set period for touch position callback to 0.1s (100ms)
	tf_lcd_128x64_set_touch_position_callback_configuration(&lcd, 100, true);

	// Set period for touch gesture callback to 0.1s (100ms)
	tf_lcd_128x64_set_touch_gesture_callback_configuration(&lcd, 100, true);

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
