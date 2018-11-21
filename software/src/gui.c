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

void gui_draw_tab_additional_left(void) {
	gui_draw_pixel(GUI_TAB0_POS_X-1, GUI_TAB_POS_Y+1, true);
	gui_draw_pixel(GUI_TAB0_POS_X-1, GUI_TAB_POS_Y+2, true);
	gui_draw_pixel(GUI_TAB0_POS_X-2, GUI_TAB_POS_Y+3, true);
	gui_draw_pixel(GUI_TAB0_POS_X-2, GUI_TAB_POS_Y+4, true);
}

void gui_draw_tab_additional_right(void) {
	gui_draw_pixel(GUI_TAB3_POS_X+1, GUI_TAB_POS_Y+1, true);
	gui_draw_pixel(GUI_TAB3_POS_X+1, GUI_TAB_POS_Y+2, true);
	gui_draw_pixel(GUI_TAB3_POS_X+2, GUI_TAB_POS_Y+3, true);
	gui_draw_pixel(GUI_TAB3_POS_X+2, GUI_TAB_POS_Y+4, true);
}

void gui_draw_tab_open_lid(const uint8_t tab) {
	switch(tab) {
		case 0: gui_draw_line_horizontal(GUI_TAB0_POS_X+1, GUI_TAB1_POS_X-1, GUI_TAB_POS_Y, false); break;
		case 1: gui_draw_line_horizontal(GUI_TAB1_POS_X+1, GUI_TAB2_POS_X-1, GUI_TAB_POS_Y, false); break;
		case 2: gui_draw_line_horizontal(GUI_TAB2_POS_X+1, GUI_TAB3_POS_X-1, GUI_TAB_POS_Y, false); break;
	}
}

void gui_draw_tab(const uint8_t x, const uint8_t y, const int8_t index) {
	uint8_t xend = x + GUI_TAB_WIDTH;

	gui_draw_pixel(x+0, y+0, true); gui_draw_pixel(xend-0, y+0, true);
	gui_draw_pixel(x+1, y+1, true); gui_draw_pixel(xend-1, y+1, true);
	gui_draw_pixel(x+1, y+2, true); gui_draw_pixel(xend-1, y+2, true);
	gui_draw_pixel(x+2, y+3, true); gui_draw_pixel(xend-2, y+3, true);
	gui_draw_pixel(x+2, y+4, true); gui_draw_pixel(xend-2, y+4, true); 
	gui_draw_pixel(x+3, y+5, true); gui_draw_pixel(xend-3, y+5, true);
	gui_draw_pixel(x+3, y+6, true); gui_draw_pixel(xend-3, y+6, true);
	gui_draw_pixel(x+4, y+7, true); gui_draw_pixel(xend-4, y+7, true);
	gui_draw_pixel(x+5, y+8, true); gui_draw_pixel(xend-5, y+8, true);
	gui_draw_line_horizontal(x+5, xend-5, LCD_MAX_ROWS*8-1, true);

	if(index > -1) {
		const uint8_t text_length        = strnlen(gui.tab[index].text, GUI_TAB_TEXT_LENGTH_MAX);
		const uint8_t pixel_text_length  = text_length*6;
		const uint8_t pixel_text_mid_x   = x + GUI_TAB_WIDTH/2;
		const uint8_t pixel_text_start_x = MAX(0, ((int32_t)pixel_text_mid_x) - pixel_text_length/2 + 1);
		const uint8_t pixel_text_start_y = y + 1;

		gui.use_global_bounding_box     = true;
		gui.global_bounding_box_start_x = x    + 5 + 1;
		gui.global_bounding_box_end_x   = xend - 5 - 1;
		gui.global_bounding_box_start_y = y        + 1;
		gui.global_bounding_box_end_y   = LCD_MAX_ROWS*8-1 - 1;
		gui_draw_text(pixel_text_start_x, pixel_text_start_y, text_length, gui.tab[index].text);
		uartbb_printf("draw: %d -> %s (%d %d %d)\n\r", index, gui.tab[index].text, pixel_text_start_x, pixel_text_start_y, text_length);
		gui.use_global_bounding_box     = false;
	}
}

void gui_update_tabs(void) {
	memset(gui.tabs, -1, GUI_TAB_NUM_MAX);

	gui.tabs_count = 0;
	gui.tabs_current = 0;
	for(uint8_t i = 0; i < GUI_TAB_NUM_MAX; i++) {
		if(gui.tab[i].active) {
			if(gui.tab_current == i) {
				gui.tabs_current = gui.tabs_count;
			}

			gui.tabs[gui.tabs_count] = i;
			gui.tabs_count++;
		}
	}
}

void gui_draw_tabs(void) {
	if((gui.tab_current == -1) || (gui.tabs_count == 0)) {
		return;
	}

	int8_t tabs_shown[3] = {-1, -1, -1};



	if((gui.tabs_current == 0) || (gui.tabs_count <= 3)) {
		tabs_shown[0] = gui.tabs[0];
		tabs_shown[1] = gui.tabs[1];
		tabs_shown[2] = gui.tabs[2];
	} else if(gui.tabs_current == gui.tabs_count-1) {
		tabs_shown[0] = gui.tabs[gui.tabs_current - 2];
		tabs_shown[1] = gui.tabs[gui.tabs_current - 1];
		tabs_shown[2] = gui.tabs[gui.tabs_current    ];
	} else {
		tabs_shown[0] = gui.tabs[gui.tabs_current - 1];
		tabs_shown[1] = gui.tabs[gui.tabs_current    ];
		tabs_shown[2] = gui.tabs[gui.tabs_current + 1];
	}

	gui_draw_box(0, LCD_MAX_COLUMNS-1, GUI_TAB_POS_Y, LCD_MAX_ROWS*8-1, true, false);

	gui_draw_line_horizontal(0, LCD_MAX_COLUMNS-1, GUI_TAB_POS_Y, true);

	gui_draw_tab(GUI_TAB0_POS_X, GUI_TAB_POS_Y, tabs_shown[0]);
	gui_draw_tab(GUI_TAB1_POS_X, GUI_TAB_POS_Y, tabs_shown[1]);
	gui_draw_tab(GUI_TAB2_POS_X, GUI_TAB_POS_Y, tabs_shown[2]);

	if(gui.tabs_count > 3) {
		if(gui.tabs_current > 1) { 
			gui_draw_tab_additional_left();
		}
		if(gui.tabs_current < gui.tabs_count-2) {
			gui_draw_tab_additional_right();
		}
	}

	if(gui.tabs_count <= 3) {
		gui.tabs_open_lid = gui.tabs_current;
	} else {
		if(gui.tabs_current == 0) {
			gui.tabs_open_lid = 0;
		} else if(gui.tabs_current== gui.tabs_count-1) {
			gui.tabs_open_lid = 2;
		} else {
			gui.tabs_open_lid = 1;
		}
	}

	gui_draw_tab_open_lid(gui.tabs_open_lid);

	if(uc1701.automatic_draw) {
		uc1701.display_mask_changed = true;
	}
}

void gui_remove_all(const bool buttons, const bool slider, const bool tabs) {
	if(buttons) {
		for(uint8_t i = 0; i < GUI_BUTTON_NUM_MAX; i++) {
			gui.button[i].active = false;
		}
	}

	if(slider) {
		for(uint8_t i = 0; i < GUI_SLIDER_NUM_MAX; i++) {
			gui.slider[i].active = false;
		}
	}

	if(tabs) {
		for(uint8_t i = 0; i < GUI_TAB_NUM_MAX; i++) {
			gui.tab[i].active = false;
		}
		gui_update_tabs();
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

	// Check tabs
	if(gui.tabs_count > 0) {
		// Click
		if(gui.tab_change_tab_config & LCD_128X64_CHANGE_TAB_ON_CLICK) {
			if(gui.last_tab_touch_time + 250 < tsc2046e.touch_time) {
				if(tsc2046e.touch_y >= GUI_TAB_POS_Y) {
					gui.last_tab_touch_time = system_timer_get_ms();
					int8_t new_tabs_current = -1;
					if(tsc2046e.touch_x < GUI_TAB1_POS_X) {
						if(gui.tabs_open_lid == 1) {
							new_tabs_current = gui.tabs_current-1;
						} else if(gui.tabs_open_lid == 2) {
							new_tabs_current = gui.tabs_current-2;
						}
					} else if(tsc2046e.touch_x <= GUI_TAB2_POS_X) {
						if(gui.tabs_open_lid == 0) {
							new_tabs_current = gui.tabs_current+1;
						} else if(gui.tabs_open_lid == 2) {
							new_tabs_current = gui.tabs_current-1;
						}
					} else {
						if(gui.tabs_open_lid == 0) {
							new_tabs_current = gui.tabs_current+2;
						} else if(gui.tabs_open_lid == 1) {
							new_tabs_current = gui.tabs_current+1;
						}
					}
					if((new_tabs_current >= 0) && (new_tabs_current < gui.tabs_count) && (gui.tabs[new_tabs_current] != -1)) {
						gui.tabs_current = new_tabs_current;
						gui.tab_current = gui.tabs[new_tabs_current];
						redraw = true;
						if(gui.tab_clear_gui) {
							gui_remove_all(true, true, false);
						}
					} 
				}
			}
		}

		// Gesture
		if(gui.tab_change_tab_config & LCD_128X64_CHANGE_TAB_ON_SWIPE) {
			if(gui.last_tab_gesture_time + 250 < tsc2046e.gesture_api_time) {
				gui.last_tab_gesture_time = system_timer_get_ms();
				if(tsc2046e.gesture_api_gesture == LCD_128X64_GESTURE_RIGHT_TO_LEFT) {
					if((gui.tabs_current != (gui.tabs_count-1)) && (gui.tabs[gui.tabs_current+1] != -1)) {
						gui.tabs_current++;
						gui.tab_current = gui.tabs[gui.tabs_current];
						redraw = true;
						if(gui.tab_clear_gui) {
							gui_remove_all(true, true, false);
						}
					}
				} else if(tsc2046e.gesture_api_gesture == LCD_128X64_GESTURE_LEFT_TO_RIGHT) {
					if((gui.tabs_current != 0) && (gui.tabs[gui.tabs_current-1] != -1)) {
						gui.tabs_current--;
						gui.tab_current = gui.tabs[gui.tabs_current];
						redraw = true;
						if(gui.tab_clear_gui) {
							gui_remove_all(true, true, false);
						}
					}
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

	gui_draw_tabs();
}

void gui_init(void) {
	memset(&gui, 0, sizeof(GUI));

	gui.tab_current = -1;
	gui.tab_clear_gui = false;
	gui.tab_change_tab_config = LCD_128X64_CHANGE_TAB_ON_CLICK_AND_SWIPE;
}

void gui_tick(void) {
	gui_touch_check();
}