/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * tsc2046e.c: Driver for TSC2046E touch screen controller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "tsc2046e.h"

#include "configs/config_tsc2046e.h"

#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/os/coop_task.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "communication.h"

#include "spi.h"

CoopTask tsc2046e_task;
TSC2046E tsc2046e;

#define TSC2046E_ADDR_START       0x80
#define TSC2046E_ADDR_12BIT       0x00
#define TSC2046E_ADDR_8BIT        0x08
#define TSC2046E_ADDR_DIFF        0x00
#define TSC2046E_ADDR_SINGLE      0x04
#define TSC2046E_ADDR_X_POS       0x50
#define TSC2046E_ADDR_Y_POS       0x10
#define TSC2046E_ADDR_Z1_POS      0x30
#define TSC2046E_ADDR_Z2_POS      0x40
#define TSC2046E_ADDR_PWD         0x00
#define TSC2046E_ADDR_ALWAYSON    0x03

const char *TSC2046E_SWIPE_STRINGS[] = {
	"left to right",
	"right to left",
	"top to bottom",
	"bottom to top",
};

void tsc2046e_task_new_gesture(void) {
	if(tsc2046e.gesture_num > 25) {
		uint8_t x_swipe;
		uint8_t y_swipe;
		uint32_t x_diff = 0;
		uint32_t y_diff = 0;

		// left to right / right to left
		if(tsc2046e.gesture_x_start < tsc2046e.gesture_x_end) {
			x_diff = tsc2046e.gesture_x_end - tsc2046e.gesture_x_start;
			x_swipe = LCD_128X64_GESTURE_LEFT_TO_RIGHT;
		} else {
			x_diff = tsc2046e.gesture_x_start - tsc2046e.gesture_x_end;
			x_swipe = LCD_128X64_GESTURE_RIGHT_TO_LEFT;
		}

		// top to bottom / bottom to top
		if(tsc2046e.gesture_y_start < tsc2046e.gesture_y_end) {
			y_diff = tsc2046e.gesture_y_end - tsc2046e.gesture_y_start;
			y_swipe = LCD_128X64_GESTURE_TOP_TO_BOTTOM;
		} else {
			y_diff = tsc2046e.gesture_y_start - tsc2046e.gesture_y_end;
			y_swipe = LCD_128X64_GESTURE_BOTTOM_TO_TOP;
		}

		if((x_diff > 5) || (y_diff > 5)) {
			uint8_t swipe = x_diff > y_diff ? x_swipe : y_swipe;

			tsc2046e.gesture_api_gesture = swipe;
			tsc2046e.gesture_api_x_start = tsc2046e.gesture_x_start;
			tsc2046e.gesture_api_y_start = tsc2046e.gesture_y_start;
			tsc2046e.gesture_api_x_end = tsc2046e.gesture_x_end;
			tsc2046e.gesture_api_y_end = tsc2046e.gesture_y_end;
			tsc2046e.gesture_api_pressure_max = tsc2046e.gesture_pressure_max;
			tsc2046e.gesture_api_time = system_timer_get_ms();
			tsc2046e.gesture_api_duration = tsc2046e.gesture_api_time - tsc2046e.gesture_time_start;
#if 0
			uartbb_printf("gesture: %s, start: %d %d, end: %d %d, pressure-max: %d time: %d\n\r",
						  TSC2046E_SWIPE_STRINGS[swipe],
						  tsc2046e.gesture_x_start,
						  tsc2046e.gesture_y_start,
						  tsc2046e.gesture_x_end,
						  tsc2046e.gesture_y_end,
						  tsc2046e.gesture_pressure_max,
						  system_timer_get_ms() - tsc2046e.gesture_time_start);
#endif
		}
	}
}

void tsc2046e_task_tick(void) {
	bool last_measure_ok = false;
	while(true) {
		uint8_t data[] = {
			TSC2046E_ADDR_START | TSC2046E_ADDR_12BIT | TSC2046E_ADDR_DIFF | TSC2046E_ADDR_Z1_POS,
			0,
			TSC2046E_ADDR_START | TSC2046E_ADDR_12BIT | TSC2046E_ADDR_DIFF | TSC2046E_ADDR_Z2_POS,
			0,
			TSC2046E_ADDR_START | TSC2046E_ADDR_12BIT | TSC2046E_ADDR_DIFF | TSC2046E_ADDR_X_POS,
			0,
			TSC2046E_ADDR_START | TSC2046E_ADDR_12BIT | TSC2046E_ADDR_DIFF | TSC2046E_ADDR_Y_POS,
			0, 0
		};

		spi_task_transceive(data, sizeof(data), XMC_SPI_CH_SLAVE_SELECT_3);

		uint16_t z1 = ((spi.data[1] << 5) | (spi.data[2] >> 3)) & 0xFFF;
		uint16_t z2 = ((spi.data[3] << 5) | (spi.data[4] >> 3)) & 0xFFF;
		uint16_t x =  ((spi.data[5] << 5) | (spi.data[6] >> 3)) & 0xFFF;
		uint16_t y =  ((spi.data[7] << 5) | (spi.data[8] >> 3)) & 0xFFF;

		int16_t pressure = 4096 - (x*z2/z1 - x);
		if((spi.data[1] == 0) && (spi.data[3] == 127)) {
			pressure = 0;
		}

		if(pressure < 0) {
			pressure = 1;
		}

		if(pressure != 0) {
			// To ignore spurious measurements, we only accept
			// new data if the pressure was > 0 at least two
			// times in a row.
			if(last_measure_ok) {
				if(y < 400) {
					tsc2046e.touch_y = 0;
				} else {
					tsc2046e.touch_y = (y-400)*63/3200;
				}
				tsc2046e.touch_y = 63 - MIN(tsc2046e.touch_y, 63);

				if(x < 200) {
					tsc2046e.touch_x = 0;
				} else {
					tsc2046e.touch_x = (x-200)*127/3600;
				}
				tsc2046e.touch_x = MIN(tsc2046e.touch_x, 127);

				tsc2046e.touch_pressure = 0;
				if((pressure > 0) && (pressure < 1200)) {
					tsc2046e.touch_pressure = 1;
				} else {
					tsc2046e.touch_pressure = (pressure-1200)/10;
				}

				tsc2046e.touch_time = system_timer_get_ms();

				XMC_GPIO_SetOutputLow(TSC2046E_LED_PIN);

				tsc2046e.gesture_x_end = tsc2046e.touch_x;
				tsc2046e.gesture_y_end = tsc2046e.touch_y;
				if(tsc2046e.gesture_num == 0) {
					tsc2046e.gesture_x_start = tsc2046e.touch_x;
					tsc2046e.gesture_y_start = tsc2046e.touch_y;
					tsc2046e.gesture_pressure_max = tsc2046e.touch_pressure;
					tsc2046e.gesture_time_start = system_timer_get_ms();
				} else {
					tsc2046e.gesture_pressure_max = MAX(tsc2046e.gesture_pressure_max, tsc2046e.touch_pressure);
				}

				tsc2046e.gesture_num++;
			}
			last_measure_ok = true;
		} else {
			if(tsc2046e.gesture_num > 0) {
				tsc2046e_task_new_gesture();
				tsc2046e.gesture_num = 0;
			}

			last_measure_ok = false;
			XMC_GPIO_SetOutputHigh(TSC2046E_LED_PIN);
		}
		coop_task_yield();
	}
}

void tsc2046e_init(void) {
	// Only configure select pin, everything else is already set up by uc1701 lcd controller

	// pin select configuration
	const XMC_GPIO_CONFIG_t pin_select_config = {
		.mode             = TSC2046E_SELECT_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure slave select pin
	XMC_GPIO_Init(TSC2046E_SELECT_PIN, &pin_select_config);

	// pin high configuration
	const XMC_GPIO_CONFIG_t pin_high_config = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure led
	XMC_GPIO_Init(TSC2046E_LED_PIN, &pin_high_config);


	const XMC_GPIO_CONFIG_t pin_config_input = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	XMC_GPIO_Init(TSC2046E_BUSY_PIN, &pin_config_input);
	XMC_GPIO_Init(TSC2046E_PENIRQ_PIN, &pin_config_input);

	coop_task_init(&tsc2046e_task, tsc2046e_task_tick);
}

void tsc2046e_tick(void) {
	coop_task_tick(&tsc2046e_task);
}
