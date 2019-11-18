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
	if(gui.draw_to_user_display) {
		if(color) {
			if(!(uc1701.display_user[row/8][column] & (1 << display_bit))) {
				uc1701.display_user[row/8][column] |= (1 << display_bit);
			}
		} else {
			if((uc1701.display_user[row/8][column] & (1 << display_bit))) {
				uc1701.display_user[row/8][column] &= ~(1 << display_bit);
			}
		}
	} else {
		if(color) {
			if(!(uc1701.display_gui[row/8][column] & (1 << display_bit))) {
				uc1701.display_gui[row/8][column] |= (1 << display_bit);
			}
		} else {
			if((uc1701.display_gui[row/8][column] & (1 << display_bit))) {
				uc1701.display_gui[row/8][column] &= ~(1 << display_bit);
			}
		}
	}
}

void gui_draw_text(const uint8_t column, const uint8_t row, const uint8_t text_length, const char *text, const uint8_t font_type, const bool color) {
	uint8_t col_mul = 1;
	uint8_t row_mul = 1;
	switch(font_type) {
		case LCD_128X64_FONT_6X8:   col_mul = 1; row_mul = 1; break;
		case LCD_128X64_FONT_6X16:  col_mul = 1; row_mul = 2; break;
		case LCD_128X64_FONT_6X24:  col_mul = 1; row_mul = 3; break;
		case LCD_128X64_FONT_6X32:  col_mul = 1; row_mul = 4; break;
		case LCD_128X64_FONT_12X16: col_mul = 2; row_mul = 2; break;
		case LCD_128X64_FONT_12X24: col_mul = 2; row_mul = 3; break;
		case LCD_128X64_FONT_12X32: col_mul = 2; row_mul = 4; break;
		case LCD_128X64_FONT_18X24: col_mul = 3; row_mul = 3; break;
		case LCD_128X64_FONT_18X32: col_mul = 3; row_mul = 4; break;
		case LCD_128X64_FONT_24X32: col_mul = 4; row_mul = 4; break;
	}

	for(uint8_t i = 0; i < text_length; i++) {
		for(uint8_t j = 0; j < 6; j++) {
			const uint8_t data = ((j == 5) ? 0 : font[(uint8_t)text[i]*5 + j]);
			for(uint8_t k = 0; k < 8; k++) {
				const bool    pixel_value  = (data & (1 << k)) ? color : !color;
				const uint8_t pixel_column = j*col_mul + column + i*6*col_mul;
				const uint8_t pixel_row    = k*row_mul+row;

				if((row_mul == 1) && (col_mul == 1)) {
					gui_draw_pixel(pixel_column, pixel_row, pixel_value);
				} else {
					gui_draw_box(pixel_column, pixel_row, pixel_column+col_mul-1, pixel_row+row_mul-1, true, pixel_value);
				}
			}
		}
	}
}

void gui_draw_box(const uint8_t x_start, const uint8_t y_start, const uint8_t x_end, const uint8_t y_end, const bool fill, const bool color) {
	const uint8_t x0 = MIN(x_start, x_end);
	const uint8_t y0 = MIN(y_start, y_end);
	const uint8_t x1 = MAX(x_start, x_end);
	const uint8_t y1 = MAX(y_start, y_end);

	for(uint8_t x = x0; x <= x1; x++) {
		for(uint8_t y = y0; y <= y1; y++) {
			if(fill) {
				gui_draw_pixel(x, y, color);
			} else {
				if((x == x0) || (y == y0) ||
				   (x == x1) || (y == y1)) {
					gui_draw_pixel(x, y, color);
				} else {
					gui_draw_pixel(x, y, !color);
				}
			}
		}
	}
}

void gui_draw_line_low(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const bool color) {
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t yi = 1;
	if(dy < 0) {
		yi = -1;
		dy = -dy;
	}

	int16_t D = 2*dy - dx;
	int16_t y = y0;

	for(uint8_t x = x0; x <= x1; x++) {
		gui_draw_pixel(x, y, color);
		if(D > 0) {
			y = y + yi;
			D = D - 2*dx;
		}
		D = D + 2*dy;
	}
}

void gui_draw_line_high(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const bool color) {
	int16_t dx = x1 - x0;
	int16_t dy = y1 - y0;
	int16_t xi = 1;
	if(dx < 0) {
		xi = -1;
		dx = -dx;
	}

	int16_t D = 2*dx - dy;
	int16_t x = x0;

	for(uint8_t y = y0; y <= y1; y++) {
		gui_draw_pixel(x, y, color);
		if(D > 0) {
			x = x + xi;
			D = D - 2*dy;
		}
		D = D + 2*dx;
	}
}

// See https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void gui_draw_line(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const bool color) {
	if(ABS(((int32_t)y1) - y0) < ABS(((int32_t)x1) - x0)) {
		if(x0 > x1) {
			gui_draw_line_low(x1, y1, x0, y0, color);
		} else {
			gui_draw_line_low(x0, y0, x1, y1, color);
		}
	} else {
		if(y0 > y1) {
			gui_draw_line_high(x1, y1, x0, y0, color);
		} else {
			gui_draw_line_high(x0, y0, x1, y1, color);
		}
	}
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

void gui_draw_button(const uint8_t index) {
	const uint8_t x_start = gui.button[index].position_x;
	const uint8_t y_start = gui.button[index].position_y;
	const uint8_t x_end   = gui.button[index].position_x + gui.button[index].width - 1;
	const uint8_t y_end   = gui.button[index].position_y + gui.button[index].height - 1;

	gui_draw_box(x_start, y_start, x_end, y_end, false, true);
	if(gui.button[index].pressed) {
		gui_draw_box(x_start+1, y_start+1, x_end-1, y_end-1, false, true);
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
	gui_draw_text(pixel_text_start_x, pixel_text_start_y, text_length, gui.button[index].text, 0, true);
	gui.use_global_bounding_box     = false;
	
	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}
}

void gui_draw_slider(const uint8_t index) {
	if(gui.slider[index].direction == LCD_128X64_DIRECTION_HORIZONTAL) {
		const uint8_t slider_start_x = gui.slider[index].position_x;
		const uint8_t slider_start_y = gui.slider[index].position_y;
		const uint8_t slider_end_x   = gui.slider[index].position_x + gui.slider[index].length - 1;
		const uint8_t slider_end_y   = gui.slider[index].position_y + GUI_SLIDER_KNOB_WIDTH - 1;
		gui_draw_box(slider_start_x, slider_start_y, slider_end_x, slider_end_y, true, false);
		gui_draw_box(slider_start_x+2, slider_start_y+GUI_SLIDER_KNOB_WIDTH/2-1, slider_end_x-2, slider_end_y-GUI_SLIDER_KNOB_WIDTH/2+1, false, true);

		const uint8_t slider_knob_start_x = gui.slider[index].position_x + gui.slider[index].value;
		const uint8_t slider_knob_start_y = gui.slider[index].position_y;
		const uint8_t slider_knob_end_x   = gui.slider[index].position_x + gui.slider[index].value + GUI_SLIDER_KNOB_LENGTH - 1;
		const uint8_t slider_knob_end_y   = gui.slider[index].position_y + GUI_SLIDER_KNOB_WIDTH - 1;
		gui_draw_box(slider_knob_start_x, slider_knob_start_y, slider_knob_end_x, slider_knob_end_y, false, true);
		if(gui.slider[index].pressed) {
			gui_draw_box(slider_knob_start_x+1, slider_knob_start_y+1, slider_knob_end_x-1, slider_knob_end_y-1, false, true);
		}
		gui_draw_line_vertical(slider_knob_start_y+3, slider_knob_end_y-3, slider_knob_start_x + GUI_SLIDER_KNOB_LENGTH/2, true);
		gui_draw_line_vertical(slider_knob_start_y+3, slider_knob_end_y-3, slider_knob_start_x + GUI_SLIDER_KNOB_LENGTH/2-1, true);
	} else {
		const uint8_t slider_start_x = gui.slider[index].position_x;
		const uint8_t slider_start_y = gui.slider[index].position_y;
		const uint8_t slider_end_x   = gui.slider[index].position_x + GUI_SLIDER_KNOB_WIDTH - 1;
		const uint8_t slider_end_y   = gui.slider[index].position_y + gui.slider[index].length - 1;
		gui_draw_box(slider_start_x, slider_start_y, slider_end_x, slider_end_y, true, false);
		gui_draw_box(slider_start_x+GUI_SLIDER_KNOB_WIDTH/2-1, slider_start_y+2, slider_end_x-GUI_SLIDER_KNOB_WIDTH/2+1, slider_end_y-2, false, true);

		const uint8_t slider_knob_start_x = gui.slider[index].position_x;
		const uint8_t slider_knob_start_y = gui.slider[index].position_y + gui.slider[index].value;
		const uint8_t slider_knob_end_x   = gui.slider[index].position_x + GUI_SLIDER_KNOB_WIDTH - 1;
		const uint8_t slider_knob_end_y   = gui.slider[index].position_y  + gui.slider[index].value + GUI_SLIDER_KNOB_LENGTH - 1;
		gui_draw_box(slider_knob_start_x, slider_knob_start_y, slider_knob_end_x, slider_knob_end_y, false, true);
		if(gui.slider[index].pressed) {
			gui_draw_box(slider_knob_start_x+1, slider_knob_start_y+1, slider_knob_end_x-1, slider_knob_end_y-1, false, true);
		}
		gui_draw_line_horizontal(slider_knob_start_x+3, slider_knob_end_x-3, slider_knob_start_y + GUI_SLIDER_KNOB_LENGTH/2, true);
		gui_draw_line_horizontal(slider_knob_start_x+3, slider_knob_end_x-3, slider_knob_start_y + GUI_SLIDER_KNOB_LENGTH/2-1, true);
	}

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
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
		if(gui.tab[index].active_text) {
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
			gui_draw_text(pixel_text_start_x, pixel_text_start_y, text_length, gui.tab[index].text, 0, true);
			gui.use_global_bounding_box     = false;
		} else if(gui.tab[index].active_icon) {
			for(uint8_t column = 0; column < 28; column++) {
				for(uint8_t row = 0; row < 6; row++) {
					const uint8_t icon_index = (row*28 + column) / 8;
					const uint8_t icon_bit   = (row*28 + column) % 8;
					gui_draw_pixel(column+x+7, row+y+2, gui.tab[index].icon[icon_index] & (1 << icon_bit));
				}
			}
		}
	}
}

void gui_update_tabs(void) {
	memset(gui.tabs, -1, GUI_TAB_NUM_MAX);

	gui.tabs_count = 0;
	gui.tabs_current = 0;
	for(uint8_t i = 0; i < GUI_TAB_NUM_MAX; i++) {
		if(gui.tab[i].active_text || gui.tab[i].active_icon) {
			if(gui.tab_selected == i) {
				gui.tabs_current = gui.tabs_count;
			}

			gui.tabs[gui.tabs_count] = i;
			gui.tabs_count++;
		}
	}
}

void gui_draw_tabs(void) {
	if((gui.tab_selected == -1) || (gui.tabs_count == 0)) {
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

	gui_draw_box(0, GUI_TAB_POS_Y, LCD_MAX_COLUMNS-1, LCD_MAX_ROWS*8-1, true, false);

	gui_draw_line_horizontal(0, LCD_MAX_COLUMNS-1, GUI_TAB_POS_Y, true);

	gui_draw_tab(GUI_TAB0_POS_X, GUI_TAB_POS_Y, tabs_shown[0]);
	if(gui.tabs_count > 1) {
		gui_draw_tab(GUI_TAB1_POS_X, GUI_TAB_POS_Y, tabs_shown[1]);
	}
	if(gui.tabs_count > 2) {
		gui_draw_tab(GUI_TAB2_POS_X, GUI_TAB_POS_Y, tabs_shown[2]);
	}

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
		uc1701.display_user_changed = true;
	}
}

void gui_draw_graph(const uint8_t index) {
	gui_draw_box(gui.graph[index].position_x, 
				 gui.graph[index].position_y, 
	             gui.graph[index].position_x + gui.graph[index].width, 
				 gui.graph[index].position_y + gui.graph[index].height, 
				 true, false);

	// Draw axis
	gui_draw_line_vertical(gui.graph[index].position_y, 
	                       gui.graph[index].position_y + gui.graph[index].height - 1, 
						   gui.graph[index].position_x, 
						   true);

	gui_draw_line_horizontal(gui.graph[index].position_x + 1, 
	                         gui.graph[index].position_x + gui.graph[index].width, 
						     gui.graph[index].position_y + gui.graph[index].height, 
						     true);

	const uint8_t start_x = gui.graph[index].position_x + 1;
	const uint8_t start_y = gui.graph[index].position_y + gui.graph[index].height - 1;

	// Draw dots, line or bar graph
	switch(gui.graph[index].graph_type) {
		case LCD_128X64_GRAPH_TYPE_DOT: {
			for(uint8_t i = 0; i < gui.graph[index].width; i++) {
				uint8_t value = gui.graph[index].data[i]*(gui.graph[index].height-1)/255;
				gui_draw_pixel(start_x + i, start_y - value, true);
			}
			break;
		}

		case LCD_128X64_GRAPH_TYPE_LINE: {
			for(uint8_t i = 0; i < gui.graph[index].width-1; i++) {
				int32_t value0 = gui.graph[index].data[i]*(gui.graph[index].height-1)/255;
				int32_t value1 = gui.graph[index].data[i+1]*(gui.graph[index].height-1)/255;
				if(ABS(value0 - value1) < 2) {
					gui_draw_pixel(start_x + i, start_y - value0, true);
				} else {
					if(value0 > value1) {
						gui_draw_line_vertical(start_y - value0, start_y - (value1+1), start_x + i, true);
					} else {
						gui_draw_line_vertical(start_y - (value1-1), start_y - value0, start_x + i, true);
					}
				}
			}

			uint8_t last_index = gui.graph[index].width-1;
			uint8_t value = gui.graph[index].data[last_index]*(gui.graph[index].height-1)/255;
			gui_draw_pixel(start_x + last_index, start_y - value, true);
			break;
		}

		case LCD_128X64_GRAPH_TYPE_BAR: {
			for(uint8_t i = 0; i < gui.graph[index].width; i+= 2) {
				int32_t value = gui.graph[index].data[i/2]*(gui.graph[index].height-1)/255;
				gui_draw_line_vertical(start_y - value, start_y, start_x + i, true);
			}
			break;
		}
	}

	// Draw axis caption
	const uint8_t text_length_x = strnlen(gui.graph[index].text_x, GUI_GRAPH_TEXT_LENGTH_MAX);
	const uint8_t text_length_y = strnlen(gui.graph[index].text_y, GUI_GRAPH_TEXT_LENGTH_MAX);

	if(text_length_x > 0) {
		gui_draw_text(gui.graph[index].position_x + gui.graph[index].width - text_length_x*6 + 1, start_y + 1 - 8, text_length_x, gui.graph[index].text_x, 0, true);
	}
	if(text_length_y > 0) {
		gui_draw_text(gui.graph[index].position_x + 2, gui.graph[index].position_y, text_length_y, gui.graph[index].text_y, 0, true);
	}

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}
}

void gui_remove_all(const bool buttons, const bool slider, const bool graphs, const bool tabs) {
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

	if(graphs) {
		for(uint8_t i = 0; i < GUI_GRAPH_NUM_MAX; i++) {
			gui.graph[i].active = false;
		}
	}

	if(tabs) {
		for(uint8_t i = 0; i < GUI_TAB_NUM_MAX; i++) {
			gui.tab[i].active_text = false;
			gui.tab[i].active_icon = false;
		}
		gui_update_tabs();
		gui.tab_selected = -1;
	}

	uc1701.display_gui_changed = true;
	gui_redraw();
	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}
}

bool gui_point_in_slider(const uint8_t x, const uint8_t y) {
	for(uint8_t i = 0; i < GUI_SLIDER_NUM_MAX; i++) {
		if(!gui.slider[i].active) {
			continue;
		}

		uint8_t slider_start_x;
		uint8_t slider_end_x;
		uint8_t slider_start_y;
		uint8_t slider_end_y;

		// Get bounding box of slider
		if(gui.slider[i].direction == LCD_128X64_DIRECTION_HORIZONTAL) {
			slider_start_x = gui.slider[i].position_x;
			slider_end_x   = gui.slider[i].position_x + gui.slider[i].length - 1;
			slider_start_y = gui.slider[i].position_y;
			slider_end_y   = gui.slider[i].position_y + GUI_SLIDER_KNOB_WIDTH - 1;
		} else {
			slider_start_x = gui.slider[i].position_x;
			slider_end_x   = gui.slider[i].position_x + GUI_SLIDER_KNOB_WIDTH - 1;
			slider_start_y = gui.slider[i].position_y;
			slider_end_y   = gui.slider[i].position_y + gui.slider[i].length - 1;
		}
		if((slider_start_x <= x) &&
		   (slider_end_x   >= x) &&
		   (slider_start_y <= y) &&
		   (slider_end_y   >= y)) {
			return true;
		}
	}

	return false;
}

void gui_touch_check(void) {
	// Do gui touch check only every fifth loop.
	// If we have lots of stuff to draw it can happen that we check again before the
	// touch was detected, resulting in false-positive touch releases.
	static uint8_t count = 0;
	count++;
	if(count < 5) {
		return;
	}
	count = 0;

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
			if((gui.last_tab_touch_time + 250 < tsc2046e.touch_time) && !touch) {
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
						gui.tab_selected = gui.tabs[new_tabs_current];
						redraw = true;
						if(gui.tab_clear_gui) {
							gui_remove_all(true, true, true, false);
						}
					} 
				}
			}
		}

		// Gesture
		if(gui.tab_change_tab_config & LCD_128X64_CHANGE_TAB_ON_SWIPE) {
			if(gui.last_tab_gesture_time + 250 < tsc2046e.gesture_api_time) {
				// Make sure this gesture is not made during moving of a slider.
				// In this case we ignore it.
				if(!gui_point_in_slider(tsc2046e.gesture_api_x_start, tsc2046e.gesture_api_y_start) &&
				   !gui_point_in_slider(tsc2046e.gesture_api_x_end, tsc2046e.gesture_api_y_end)) {
					gui.last_tab_gesture_time = system_timer_get_ms();
					if(tsc2046e.gesture_api_gesture == LCD_128X64_GESTURE_RIGHT_TO_LEFT) {
						if((gui.tabs_current != (gui.tabs_count-1)) && (gui.tabs[gui.tabs_current+1] != -1)) {
							gui.tabs_current++;
							gui.tab_selected = gui.tabs[gui.tabs_current];
							redraw = true;
							if(gui.tab_clear_gui) {
								gui_remove_all(true, true, true, false);
							}
						}
					} else if(tsc2046e.gesture_api_gesture == LCD_128X64_GESTURE_LEFT_TO_RIGHT) {
						if((gui.tabs_current != 0) && (gui.tabs[gui.tabs_current-1] != -1)) {
							gui.tabs_current--;
							gui.tab_selected = gui.tabs[gui.tabs_current];
							redraw = true;
							if(gui.tab_clear_gui) {
								gui_remove_all(true, true, true, false);
							}
						}
					}
				}
			}
		}
	}

	if(redraw) {
		uc1701.display_gui_changed = true;
		gui_redraw();
	}
}

void gui_redraw(void) {
	memset(uc1701.display_gui, 0, LCD_MAX_COLUMNS*LCD_MAX_ROWS);

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

	for(uint8_t i = 0; i < GUI_GRAPH_NUM_MAX; i++) {
		if(gui.graph[i].active) {
			gui_draw_graph(i);
		}
	}

	gui_draw_tabs();
}

void gui_init(void) {
	memset(&gui, 0, sizeof(GUI));

	gui.tab_selected = -1;
	gui.tab_clear_gui = false;
	gui.tab_change_tab_config = LCD_128X64_CHANGE_TAB_ON_CLICK_AND_SWIPE;
}

void gui_tick(void) {
	gui_touch_check();
}
