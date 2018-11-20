/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * gui.c: Implementation of simple GUI elements
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

#include "gui.h"

#include "uc1701.h"
#include "tsc2046e.h"
#include "communication.h"

#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/hal/system_timer/system_timer.h"

extern const uint8_t font[];

GUI gui;

void gui_draw_pixel(const uint8_t column, const uint8_t row, const bool color) {
	if(gui.use_global_bounding_box) {
		if((gui.global_bounding_box_start_x > column) ||
		   (gui.global_bounding_box_end_x   < column) ||
		   (gui.global_bounding_box_start_y > row)    ||
		   (gui.global_bounding_box_end_y   < row)) {
		   return;
		}
	}
	
	if((column >= LCD_MAX_COLUMNS) || (row >= LCD_MAX_ROWS*8)) {
		return;
	}

	const uint8_t display_bit = row % 8;
	if(color) {
		if(!(uc1701.display[row/8][column] & (1 << display_bit))) {
			uc1701.display[row/8][column] |= (1 << display_bit);
			uc1701.display_mask[row/8][column] |= (1 << display_bit);
		}
	} else {
		if((uc1701.display[row/8][column] & (1 << display_bit))) {
			uc1701.display[row/8][column] &= ~(1 << display_bit);
			uc1701.display_mask[row/8][column] |= (1 << display_bit);
		}
	}
}

void gui_draw_text(const uint8_t column, const uint8_t row, const uint8_t text_length, const char *text) {
	for(uint8_t i = 0; i < text_length; i++) {
		for(uint8_t j = 0; j < 6; j++) {
			const uint8_t data = ((j == 5) ? 0 : font[(uint8_t)text[i]*5 + j]);
			for(uint8_t k = 0; k < 8; k++) {
				const bool    pixel_value  = data & (1 << k);
				const uint8_t pixel_column = j+column + i*6;
				const uint8_t pixel_row    = k+row;

				gui_draw_pixel(pixel_column, pixel_row, pixel_value);
			}
		}
	}
}

void gui_draw_box(const uint8_t x_start, const uint8_t x_end, const uint8_t y_start, const uint8_t y_end, const bool fill, const bool color) {
	for(uint8_t x = x_start; x <= x_end; x++) {
		for(uint8_t y = y_start; y <= y_end; y++) {
			if(fill) {
				gui_draw_pixel(x, y, color);
			} else {
				if((x == x_start) || (y == y_start) ||
				   (x == x_end)   || (y == y_end)) {
					gui_draw_pixel(x, y, color);
				} else {
					gui_draw_pixel(x, y, !color);
				}
			}
		}
	}
}

void gui_draw_line(const uint8_t x_start, const uint8_t x_end, const uint8_t y_start, const uint8_t y_end, const bool color) {
	// https://stackoverflow.com/questions/35970872/function-to-draw-a-straight-line-in-c
}

void gui_draw_line_vertical(const uint8_t y_start, const uint8_t y_end, const uint8_t x_start, const bool color) {
	for(uint8_t y = y_start; y <= y_end; y++) {
		gui_draw_pixel(x_start, y, color);
	}	
}

void gui_draw_line_horizontal(const uint8_t x_start, const uint8_t x_end, const uint8_t y_start, const bool color) {
	for(uint8_t x = x_start; x <= x_end; x++) {
		gui_draw_pixel(x, y_start, color);
	}
}

void gui_draw_button(uint8_t index) {
	const uint8_t x_start = gui.button[index].position_x;
	const uint8_t x_end   = gui.button[index].position_x + gui.button[index].width - 1;
	const uint8_t y_start = gui.button[index].position_y;
	const uint8_t y_end   = gui.button[index].position_y + gui.button[index].height - 1;

	gui_draw_box(x_start, x_end, y_start, y_end, false, true);
	if(gui.button[index].pressed) {
		gui_draw_box(x_start+1, x_end-1, y_start+1, y_end-1, false, true);
	}

	const uint8_t text_length        = strnlen(gui.button[index].text, GUI_BUTTON_TEXT_LENGTH_MAX);
	const uint8_t pixel_text_length  = text_length*6;
	const uint8_t pixel_text_mid_x   = x_start + gui.button[index].width/2;
	const uint8_t pixel_text_mid_y   = y_start + gui.button[index].height/2;
	const uint8_t pixel_text_start_x = MAX(0, ((int32_t)pixel_text_mid_x) - pixel_text_length/2);
	const uint8_t pixel_text_start_y = MAX(0, ((int32_t)pixel_text_mid_y) - 8/2);

	gui.use_global_bounding_box     = true;
	gui.global_bounding_box_start_x = x_start + 1;
	gui.global_bounding_box_end_x   = x_end   - 1;
	gui.global_bounding_box_start_y = y_start + 1;
	gui.global_bounding_box_end_y   = y_end   - 1;
	gui_draw_text(pixel_text_start_x, pixel_text_start_y, text_length, gui.button[index].text);
	gui.use_global_bounding_box     = false;
	
	if(uc1701.automatic_draw) {
		uc1701.display_mask_changed = true;
	}
}

void gui_draw_slider(uint8_t index) {
	if(gui.slider[index].direction == LCD_128X64_DIRECTION_HORIZONTAL) {
		const uint8_t slider_start_x = gui.slider[index].position_x;
		const uint8_t slider_end_x   = gui.slider[index].position_x + gui.slider[index].length - 1;
		const uint8_t slider_start_y = gui.slider[index].position_y;
		const uint8_t slider_end_y   = gui.slider[index].position_y + GUI_SLIDER_KNOB_WIDTH - 1;
		gui_draw_box(slider_start_x, slider_end_x, slider_start_y, slider_end_y, true, false);
		gui_draw_box(slider_start_x+2, slider_end_x-2, slider_start_y+GUI_SLIDER_KNOB_WIDTH/2-1, slider_end_y-GUI_SLIDER_KNOB_WIDTH/2+1, false, true);

		const uint8_t slider_knob_start_x = gui.slider[index].position_x + gui.slider[index].value;
		const uint8_t slider_knob_end_x   = gui.slider[index].position_x + gui.slider[index].value + GUI_SLIDER_KNOB_LENGTH - 1;
		const uint8_t slider_knob_start_y = gui.slider[index].position_y;
		const uint8_t slider_knob_end_y   = gui.slider[index].position_y + GUI_SLIDER_KNOB_WIDTH - 1;
		gui_draw_box(slider_knob_start_x, slider_knob_end_x, slider_knob_start_y, slider_knob_end_y, false, true);
		if(gui.slider[index].pressed) {
			gui_draw_box(slider_knob_start_x+1, slider_knob_end_x-1, slider_knob_start_y+1, slider_knob_end_y-1, false, true);
		}
		gui_draw_line_vertical(slider_knob_start_y+3, slider_knob_end_y-3, slider_knob_start_x + GUI_SLIDER_KNOB_LENGTH/2, true);
		gui_draw_line_vertical(slider_knob_start_y+3, slider_knob_end_y-3, slider_knob_start_x + GUI_SLIDER_KNOB_LENGTH/2-1, true);
	} else {
		const uint8_t slider_start_x = gui.slider[index].position_x;
		const uint8_t slider_end_x   = gui.slider[index].position_x + GUI_SLIDER_KNOB_WIDTH - 1;
		const uint8_t slider_start_y = gui.slider[index].position_y;
		const uint8_t slider_end_y   = gui.slider[index].position_y + gui.slider[index].length - 1;
		gui_draw_box(slider_start_x, slider_end_x, slider_start_y, slider_end_y, true, false);
		gui_draw_box(slider_start_x+GUI_SLIDER_KNOB_WIDTH/2-1, slider_end_x-GUI_SLIDER_KNOB_WIDTH/2+1, slider_start_y+2, slider_end_y-2, false, true);

		const uint8_t slider_knob_start_x = gui.slider[index].position_x;
		const uint8_t slider_knob_end_x   = gui.slider[index].position_x + GUI_SLIDER_KNOB_WIDTH - 1;
		const uint8_t slider_knob_start_y = gui.slider[index].position_y + gui.slider[index].value;
		const uint8_t slider_knob_end_y   = gui.slider[index].position_y  + gui.slider[index].value + GUI_SLIDER_KNOB_LENGTH - 1;
		gui_draw_box(slider_knob_start_x, slider_knob_end_x, slider_knob_start_y, slider_knob_end_y, false, true);
		if(gui.slider[index].pressed) {
			gui_draw_box(slider_knob_start_x+1, slider_knob_end_x-1, slider_knob_start_y+1, slider_knob_end_y-1, false, true);
		}
		gui_draw_line_horizontal(slider_knob_start_x+3, slider_knob_end_x-3, slider_knob_start_y + GUI_SLIDER_KNOB_LENGTH/2, true);
		gui_draw_line_horizontal(slider_knob_start_x+3, slider_knob_end_x-3, slider_knob_start_y + GUI_SLIDER_KNOB_LENGTH/2-1, true);
	}

	if(uc1701.automatic_draw) {
		uc1701.display_mask_changed = true;
	}
}

void gui_touch_check(void) {
	bool redraw = false;
	const bool touch = !system_timer_is_time_elapsed_ms(tsc2046e.touch_time, 50);

	// Check button
	for(uint8_t i = 0; i < GUI_BUTTON_NUM_MAX; i++) {
		if(gui.button[i].active) {
			if(touch &&
			   (gui.button[i].position_x <= tsc2046e.touch_x) &&
			   ((gui.button[i].position_x + gui.button[i].width) >= tsc2046e.touch_x) &&
			   (gui.button[i].position_y <= tsc2046e.touch_y) &&
			   ((gui.button[i].position_y + gui.button[i].height) >= tsc2046e.touch_y)) {
				if(!gui.button[i].pressed) {
					gui.button[i].pressed = true;
					redraw = true;
				}
			} else {
				if(gui.button[i].pressed) {
					gui.button[i].pressed = false;
					redraw = true;
				}
			}
		}
	}

	// Check slider
	for(uint8_t i = 0; i < GUI_SLIDER_NUM_MAX; i++) {
		if(gui.slider[i].active) {
			if(touch) {
				if(gui.slider[i].direction == LCD_128X64_DIRECTION_HORIZONTAL) {
					const uint8_t slider_start_x = gui.slider[i].position_x;
					const uint8_t slider_end_x   = gui.slider[i].position_x + gui.slider[i].length;
					const uint8_t slider_start_y = gui.slider[i].position_y;
					const uint8_t slider_end_y   = gui.slider[i].position_y + GUI_SLIDER_KNOB_WIDTH;
					if((slider_start_x <= tsc2046e.touch_x) &&
					(slider_end_x   >= tsc2046e.touch_x) &&
					(slider_start_y <= tsc2046e.touch_y) &&
					(slider_end_y   >= tsc2046e.touch_y)) {
						uint8_t new_value = BETWEEN(0,
													tsc2046e.touch_x - gui.slider[i].position_x - GUI_SLIDER_KNOB_LENGTH/2,
													gui.slider[i].length - GUI_SLIDER_KNOB_LENGTH);
						if((new_value != gui.slider[i].value) || !gui.slider[i].pressed) {
							gui.slider[i].pressed = true;
							gui.slider[i].value = new_value;
							redraw = true;
						}
					}
				} else {
					const uint8_t slider_start_x = gui.slider[i].position_x;
					const uint8_t slider_end_x   = gui.slider[i].position_x + GUI_SLIDER_KNOB_WIDTH;
					const uint8_t slider_start_y = gui.slider[i].position_y;
					const uint8_t slider_end_y   = gui.slider[i].position_y + gui.slider[i].length;
					if((slider_start_x <= tsc2046e.touch_x) &&
					(slider_end_x   >= tsc2046e.touch_x) &&
					(slider_start_y <= tsc2046e.touch_y) &&
					(slider_end_y   >= tsc2046e.touch_y)) {
						uint8_t new_value = BETWEEN(0,
													tsc2046e.touch_y - gui.slider[i].position_y - GUI_SLIDER_KNOB_LENGTH/2,
													gui.slider[i].length - GUI_SLIDER_KNOB_LENGTH);
						if((new_value != gui.slider[i].value) || !gui.slider[i].pressed) {
							gui.slider[i].pressed = true;
							gui.slider[i].value = new_value;
							redraw = true;
						}
					}
				}
			} else {
				if(gui.slider[i].pressed) {
					gui.slider[i].pressed = false;
					redraw = true;
				}
			}
		}
	}

	if(redraw) {
		gui_redraw();
	}
}

void gui_redraw(void) {
	for(uint8_t i = 0; i < GUI_SLIDER_NUM_MAX; i++) {
		if(gui.slider[i].active) {
			gui_draw_slider(i);
		}
	}

	for(uint8_t i = 0; i < GUI_BUTTON_NUM_MAX; i++) {
		if(gui.button[i].active) {
			gui_draw_button(i);
		}
	}
}

void gui_init(void) {
	memset(&gui, 0, sizeof(GUI));
}

void gui_tick(void) {
	gui_touch_check();
}