/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * tsc2046e.h: Driver for TSC2046E touch screen controller
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

#ifndef TSC2046E_H
#define TSC2046E_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/utility/led_flicker.h"

#define TSC2046E_GESTURE_LIST_LENGTH 1024

typedef struct {
	uint16_t touch_x;
	uint16_t touch_y;
	uint16_t touch_pressure;
	uint32_t touch_time;

	uint32_t position_period;
	bool     position_value_has_to_change;

	uint16_t gesture_x_start;
	uint16_t gesture_y_start;
	uint16_t gesture_x_end;
	uint16_t gesture_y_end;
	uint16_t gesture_pressure_max;
	uint32_t gesture_num;
	uint32_t gesture_time_start;

	uint32_t gesture_period;
	bool     gesture_value_has_to_change;

	uint16_t gesture_api_gesture;
	uint16_t gesture_api_x_start;
	uint16_t gesture_api_y_start;
	uint16_t gesture_api_x_end;
	uint16_t gesture_api_y_end;
	uint16_t gesture_api_pressure_max;
	uint32_t gesture_api_duration;
	uint32_t gesture_api_time;

	bool use_old_led_pin;
	LEDFlickerState led_state;
} TSC2046E;

extern TSC2046E tsc2046e;

void tsc2046e_init(void);
void tsc2046e_tick(void);

#endif
