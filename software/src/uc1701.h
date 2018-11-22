/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * uc1701.h: Driver for UC1701 matrix lcd controller
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

#ifndef UC1701_H
#define UC1701_H

#include <stdint.h>
#include <stdbool.h>

#define LCD_MAX_ROWS 8
#define LCD_MAX_COLUMNS 128

typedef struct {
	bool initialize;
	bool reset;
	bool new_invert;
	bool new_backlight;
	bool new_contrast;

	uint8_t display_user[LCD_MAX_ROWS][LCD_MAX_COLUMNS];
	uint8_t display_user_save[LCD_MAX_ROWS][LCD_MAX_COLUMNS];
	uint8_t display_gui[LCD_MAX_ROWS][LCD_MAX_COLUMNS];

	uint8_t display_write[LCD_MAX_ROWS][LCD_MAX_COLUMNS];
	uint8_t display_mask_write[LCD_MAX_ROWS][LCD_MAX_COLUMNS];

	bool display_user_changed;
	bool display_gui_changed;

	uint8_t display_configuration_contrast;
	uint8_t display_configuration_backlight;
	bool display_configuration_invert;

	bool redraw_all;
	bool automatic_draw;

	uint16_t read_chunk_offset;
} UC1701;

extern UC1701 uc1701;

void uc1701_init(void);
void uc1701_tick(void);

#endif
