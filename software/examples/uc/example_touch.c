// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_lcd_128x64.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Callback function for touch position callback
static void touch_position_handler(TF_LCD128x64 *device, uint16_t pressure, uint16_t x,
                                   uint16_t y, uint32_t age, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Pressure: %I16u\n", pressure);
	tf_hal_printf("X: %I16u\n", x);
	tf_hal_printf("Y: %I16u\n", y);
	tf_hal_printf("Age: %I32u\n", age);
	tf_hal_printf("\n");
}

// Callback function for touch gesture callback
static void touch_gesture_handler(TF_LCD128x64 *device, uint8_t gesture,
                                  uint32_t duration, uint16_t pressure_max,
                                  uint16_t x_start, uint16_t x_end, uint16_t y_start,
                                  uint16_t y_end, uint32_t age, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	if (gesture == TF_LCD_128X64_GESTURE_LEFT_TO_RIGHT) {
		tf_hal_printf("Gesture: Left To Right\n");
	} else if (gesture == TF_LCD_128X64_GESTURE_RIGHT_TO_LEFT) {
		tf_hal_printf("Gesture: Right To Left\n");
	} else if (gesture == TF_LCD_128X64_GESTURE_TOP_TO_BOTTOM) {
		tf_hal_printf("Gesture: Top To Bottom\n");
	} else if (gesture == TF_LCD_128X64_GESTURE_BOTTOM_TO_TOP) {
		tf_hal_printf("Gesture: Bottom To Top\n");
	}

	tf_hal_printf("Duration: %I32u\n", duration);
	tf_hal_printf("Pressure Max: %I16u\n", pressure_max);
	tf_hal_printf("X Start: %I16u\n", x_start);
	tf_hal_printf("X End: %I16u\n", x_end);
	tf_hal_printf("Y Start: %I16u\n", y_start);
	tf_hal_printf("Y End: %I16u\n", y_end);
	tf_hal_printf("Age: %I32u\n", age);
	tf_hal_printf("\n");
}

static TF_LCD128x64 lcd;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_lcd_128x64_create(&lcd, NULL, hal), "create device object");

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

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
