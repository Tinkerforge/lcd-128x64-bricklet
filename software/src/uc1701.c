/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * uc1701.c: Driver for UC1701 matrix lcd controller
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

#include "uc1701.h"

#include "configs/config_uc1701.h"

#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/hal/ccu4_pwm/ccu4_pwm.h"
#include "bricklib2/os/coop_task.h"

#include "spi.h"

#include "cie1931.h"

CoopTask uc1701_task;
UC1701 uc1701;

const uint8_t uc1701_init_commands[] = {
	0xE2, // System Reset
	0x40, // Set display start line to 0
	0xA1, // Set SEG Direction
	0xC0, // Set COM Direction
	0xA2, // Set Bias = 1/9
	0x2C, // Boost ON
	0x2E, // Voltage Regular On
	0x2F, // Voltage Follower On
	0xF8, // Set booster ratio to
	0x00, // 4x
	0x26, // Set Resistor Ratio = 6
	0x81, // Contrast level to
	0x15, // 21
	0xAC, // Set Static indicator off
	0x00,
	0XA6, // Disable inverse
	0xAF, // Set Display Enable
	// sleep 100 ms
	0xA5, // display all points
	// sleep 200 ms
	0xA4, // normal display
};

void uc1701_init_spi(void) {
	// SELECT pin configuration
	const XMC_GPIO_CONFIG_t select_pin_config = {
		.mode             = UC1701_SELECT_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Configure slave select pin
	XMC_GPIO_Init(UC1701_SELECT_PIN, &select_pin_config);

	spi_init();
}

void uc1701_task_write_data(const uint8_t *data, const uint32_t length) {
	XMC_GPIO_SetOutputHigh(UC1701_CD_PIN);
	spi_task_transceive(data, length, XMC_SPI_CH_SLAVE_SELECT_4);
}

void uc1701_task_write_command(const uint8_t *command, const uint32_t length) {
	XMC_GPIO_SetOutputLow(UC1701_CD_PIN);
	spi_task_transceive(command, length, XMC_SPI_CH_SLAVE_SELECT_4);
}

// row 0-7 (8 pixel each), column 0-127
void uc1701_task_set_cursor(uint8_t row, uint8_t column) {
	column += 4;

	uint8_t set_cursor_command[3] = {
		0xB0 + row,
		0x10 + ((column & 0xF0) >> 4),
		(column & 0x0F)
	};

	uc1701_task_write_command(set_cursor_command, 3);
}

inline void uc1701_set_byte(uint8_t column, uint8_t row_byte, uint8_t value, uint8_t data[LCD_MAX_ROWS][LCD_MAX_COLUMNS]) {
	data[row_byte][column] = value;
}

inline uint8_t uc1701_get_byte(uint8_t column, uint8_t row_byte, uint8_t data[LCD_MAX_ROWS][LCD_MAX_COLUMNS]) {
	return data[row_byte][column];
}

void uc1701_task_tick(void) {
	while(true) {
		if(uc1701.reset) {
			uc1701.reset = false;

			XMC_GPIO_SetOutputLow(UC1701_RST_PIN);
			coop_task_sleep_ms(2);
			XMC_GPIO_SetOutputHigh(UC1701_RST_PIN);
			coop_task_sleep_ms(6);
		}

		if(uc1701.initialize) {
			uc1701.initialize = false;

			uc1701_task_write_command(uc1701_init_commands, sizeof(uc1701_init_commands)-2);
			coop_task_sleep_ms(100);
			uc1701_task_write_command(&uc1701_init_commands[sizeof(uc1701_init_commands)-2], 1);
			coop_task_sleep_ms(200);
			uc1701_task_write_command(&uc1701_init_commands[sizeof(uc1701_init_commands)-1], 1);
		}

		if(uc1701.new_invert) {
			uc1701.new_invert = false;

			uint8_t invert_data = uc1701.display_configuration_invert ? 0xA7 : 0xA6;
			uc1701_task_write_command(&invert_data, 1);
		}

		if(uc1701.new_contrast) {
			uc1701.new_contrast = false;

			uint8_t contrast_data[2] = {0x81, uc1701.display_configuration_contrast};
			uc1701_task_write_command(contrast_data, 2);
		}

		if(uc1701.new_backlight) {
			uc1701.new_backlight = false;

			ccu4_pwm_set_duty_cycle(0, 65535 - cie1931[uc1701.display_configuration_backlight]);
		}

		if(uc1701.display_user_changed || uc1701.display_gui_changed) {
			// If only the gui changed (this means a button was pressed or slider was moved)
			// We update the old saved user display with the updated GUI and write it to the display
			// If there is a user change (new GUI element or other drawing from user),
			// we first copy the new user data to the user save buffer
			if(uc1701.display_user_changed) {
				memcpy(uc1701.display_user_save, uc1701.display_user, LCD_MAX_ROWS*LCD_MAX_COLUMNS);
			}

			for(uint8_t row = 0; row < LCD_MAX_ROWS; row++) {
				for(uint8_t column = 0; column < LCD_MAX_COLUMNS; column++) {
					uint8_t byte_new   = uc1701_get_byte(column, row, uc1701.display_user_save) | uc1701_get_byte(column, row, uc1701.display_gui);
					uint8_t byte_write = uc1701_get_byte(column, row, uc1701.display_write);

					if(byte_new != byte_write) {
						uc1701_set_byte(column, row, byte_new, uc1701.display_write);
						uc1701_set_byte(column, row, byte_new ^ byte_write, uc1701.display_mask_write);
					}
				}
			}

			uc1701.display_user_changed = false;
			uc1701.display_gui_changed = false;
			if(uc1701.redraw_all) {
				memset(uc1701.display_mask_write, 0xFF, LCD_MAX_ROWS*LCD_MAX_COLUMNS);
				uc1701.redraw_all = false;
			}

			for(uint8_t row = 0; row < LCD_MAX_ROWS; row++) {
				bool start_found = false;
				uint8_t column_start = 0;
				uint8_t column_end = 0;
				for(uint8_t column = 0; column  < LCD_MAX_COLUMNS; column++) {
					if(uc1701.display_mask_write[row][column] != 0) {
						if(!start_found) {
							start_found = true;
							column_start = column;
						}
						column_end = column;
					}
				}

				if(start_found) {
					uc1701_task_set_cursor(row, column_start);
					uc1701_task_write_data(&uc1701.display_write[row][column_start], column_end - column_start + 1);
				}
			}

			memset(uc1701.display_mask_write, 0, LCD_MAX_ROWS*LCD_MAX_COLUMNS);
		}

		coop_task_yield();
	}
}

void uc1701_init(void) {
	memset(&uc1701, 0, sizeof(UC1701));
	uc1701_init_spi();

	const XMC_GPIO_CONFIG_t pin_config_output_high = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	const XMC_GPIO_CONFIG_t pin_config_output_low = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_LOW
	};

	XMC_GPIO_Init(UC1701_RST_PIN, &pin_config_output_high);
	XMC_GPIO_Init(UC1701_CD_PIN, &pin_config_output_high);
	XMC_GPIO_Init(UC1701_BACKLIGHT_PIN, &pin_config_output_high);

	uc1701.reset                = true;
	uc1701.initialize           = true;
	uc1701.display_user_changed = true;
	uc1701.display_gui_changed  = true;
	uc1701.redraw_all           = true;
	uc1701.new_contrast         = true;
	uc1701.new_backlight        = true;

	uc1701.display_configuration_contrast  = 14;
	uc1701.display_configuration_backlight = 100;
	uc1701.display_configuration_invert    = false;
	uc1701.automatic_draw                  = true;

	ccu4_pwm_init(XMC_GPIO_PORT1, 0, 0, 0xFFFE);
	ccu4_pwm_set_duty_cycle(0, 0);

	coop_task_init(&uc1701_task, uc1701_task_tick);
}

void uc1701_tick(void) {
	coop_task_tick(&uc1701_task);
}
