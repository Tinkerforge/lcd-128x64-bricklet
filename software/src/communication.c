/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include <string.h>

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/hal/system_timer/system_timer.h"

#include "configs/config_tsc2046e.h"

#include "uc1701.h"
#include "tsc2046e.h"
#include "gui.h"

#include "font.inc"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_WRITE_PIXELS_LOW_LEVEL: return write_pixels_low_level(message);
		case FID_READ_PIXELS_LOW_LEVEL: return read_pixels_low_level(message, response);
		case FID_CLEAR_DISPLAY: return clear_display(message);
		case FID_SET_DISPLAY_CONFIGURATION: return set_display_configuration(message);
		case FID_GET_DISPLAY_CONFIGURATION: return get_display_configuration(message, response);
		case FID_WRITE_LINE: return write_line(message);
		case FID_DRAW_BUFFERED_FRAME: return draw_buffered_frame(message);
		case FID_GET_TOUCH_POSITION: return get_touch_position(message, response);
		case FID_SET_TOUCH_POSITION_CALLBACK_CONFIGURATION: return set_touch_position_callback_configuration(message);
		case FID_GET_TOUCH_POSITION_CALLBACK_CONFIGURATION: return get_touch_position_callback_configuration(message, response);
		case FID_GET_TOUCH_GESTURE: return get_touch_gesture(message, response);
		case FID_SET_TOUCH_GESTURE_CALLBACK_CONFIGURATION: return set_touch_gesture_callback_configuration(message);
		case FID_GET_TOUCH_GESTURE_CALLBACK_CONFIGURATION: return get_touch_gesture_callback_configuration(message, response);
		case FID_DRAW_LINE: return draw_line(message);
		case FID_DRAW_BOX: return draw_box(message);
		case FID_DRAW_TEXT: return draw_text(message);
		case FID_SET_GUI_BUTTON: return set_gui_button(message);
		case FID_GET_GUI_BUTTON: return get_gui_button(message, response);
		case FID_REMOVE_GUI_BUTTON: return remove_gui_button(message);
		case FID_SET_GUI_BUTTON_PRESSED_CALLBACK_CONFIGURATION: return set_gui_button_pressed_callback_configuration(message);
		case FID_GET_GUI_BUTTON_PRESSED_CALLBACK_CONFIGURATION: return get_gui_button_pressed_callback_configuration(message, response);
		case FID_GET_GUI_BUTTON_PRESSED: return get_gui_button_pressed(message, response);
		case FID_SET_GUI_SLIDER: return set_gui_slider(message);
		case FID_GET_GUI_SLIDER: return get_gui_slider(message, response);
		case FID_REMOVE_GUI_SLIDER: return remove_gui_slider(message);
		case FID_SET_GUI_SLIDER_VALUE_CALLBACK_CONFIGURATION: return set_gui_slider_value_callback_configuration(message);
		case FID_GET_GUI_SLIDER_VALUE_CALLBACK_CONFIGURATION: return get_gui_slider_value_callback_configuration(message, response);
		case FID_GET_GUI_SLIDER_VALUE: return get_gui_slider_value(message, response);
		case FID_SET_GUI_TAB_CONFIGURATION: return set_gui_tab_configuration(message);
		case FID_GET_GUI_TAB_CONFIGURATION: return get_gui_tab_configuration(message, response);
		case FID_SET_GUI_TAB_TEXT: return set_gui_tab_text(message);
		case FID_GET_GUI_TAB_TEXT: return get_gui_tab_text(message, response);
		case FID_SET_GUI_TAB_ICON: return set_gui_tab_icon(message);
		case FID_GET_GUI_TAB_ICON: return get_gui_tab_icon(message, response);
		case FID_REMOVE_GUI_TAB: return remove_gui_tab(message);
		case FID_SET_GUI_TAB_CURRENT: return set_gui_tab_current(message);
		case FID_SET_GUI_TAB_CURRENT_CALLBACK_CONFIGURATION: return set_gui_tab_current_callback_configuration(message);
		case FID_GET_GUI_TAB_CURRENT_CALLBACK_CONFIGURATION: return get_gui_tab_current_callback_configuration(message, response);
		case FID_GET_GUI_TAB_CURRENT: return get_gui_tab_current(message, response);
		case FID_SET_GUI_GRAPH_CONFIGURATION: return set_gui_graph_configuration(message);
		case FID_GET_GUI_GRAPH_CONFIGURATION: return get_gui_graph_configuration(message, response);
		case FID_SET_GUI_GRAPH_DATA_LOW_LEVEL: return set_gui_graph_data_low_level(message);
		case FID_GET_GUI_GRAPH_DATA_LOW_LEVEL: return get_gui_graph_data_low_level(message, response);
		case FID_REMOVE_GUI_GRAPH: return remove_gui_graph(message);
		case FID_REMOVE_ALL_GUI: return remove_all_gui(message);
		case FID_SET_TOUCH_LED_CONFIG: return set_touch_led_config(message);
		case FID_GET_TOUCH_LED_CONFIG: return get_touch_led_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse write_pixels_low_level(const WritePixelsLowLevel *data) {
	if((data->x_start > data->x_end) || (data->y_start > data->y_end)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	const uint16_t length = MIN(data->pixels_length - data->pixels_chunk_offset, 448);
	const uint8_t columns = data->x_end - data->x_start + 1;

	uint8_t column = data->x_start + (data->pixels_chunk_offset % columns);
	uint8_t row = data->y_start + (data->pixels_chunk_offset / columns);

	uint16_t counter = 0;
	for(; row <= data->y_end; row++) {
		for(; column <= data->x_end; column++) {
			const uint8_t index = counter / 8;
			const uint8_t bit = counter % 8;
			const uint8_t display_bit = row % 8;
			if(data->pixels_chunk_data[index] & (1 << bit)) {
				if(!(uc1701.display_user[row/8][column] & (1 << display_bit))) {
					uc1701.display_user[row/8][column] |= (1 << display_bit);
				}
			} else {
				if((uc1701.display_user[row/8][column] & (1 << display_bit))) {
					uc1701.display_user[row/8][column] &= ~(1 << display_bit);
				}
			}

			counter++;
			if(counter == length) {
				break;
			}
		}

		if(counter == length) {
			break;
		}
		column = data->x_start;
	}

	if(data->pixels_chunk_offset + length >= data->pixels_length) {
		if(uc1701.automatic_draw) {
			uc1701.display_user_changed = true;
		}
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse read_pixels_low_level(const ReadPixelsLowLevel *data, ReadPixelsLowLevel_Response *response) {
	if((data->x_start > data->x_end) || (data->y_start > data->y_end)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((data->x_end >= LCD_MAX_COLUMNS) || (data->y_end >= (LCD_MAX_ROWS*8))) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	memset(response->pixels_chunk_data, 0, 60);

	response->header.length = sizeof(ReadPixelsLowLevel_Response);
	response->pixels_length = (data->x_end - data->x_start + 1)*(data->y_end - data->y_start + 1);
	response->pixels_chunk_offset = uc1701.read_chunk_offset;

	const uint8_t columns = data->x_end - data->x_start + 1;

	uint8_t column = data->x_start + (response->pixels_chunk_offset % columns);
	uint8_t row = data->y_start + (response->pixels_chunk_offset / columns);

	uint16_t counter = 0;
	for(; row <= data->y_end; row++) {
		const uint16_t row_index = row / 8;
		const uint16_t row_bit   = row % 8;
		for(; column <= data->x_end; column++) {
			const uint16_t data_index = counter / 8;
			const uint16_t data_bit   = counter % 8;
			if(uc1701.display_user[row_index][column] & (1 << row_bit)) {
				response->pixels_chunk_data[data_index] |= (1 << data_bit);
			}

			counter++;
			if(counter == 60*8) {
				break;
			}
		}
		if(counter == 60*8) {
			break;
		}

		column = data->x_start;
	}

	uc1701.read_chunk_offset += counter;
	if(uc1701.read_chunk_offset == response->pixels_length) {
		uc1701.read_chunk_offset = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse clear_display(const ClearDisplay *data) {
	for(uint8_t row = 0; row < LCD_MAX_ROWS; row++) {
		for(uint8_t column = 0; column < LCD_MAX_COLUMNS; column++) {
			uc1701.display_user[row][column] = 0;
		}
	}

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_display_configuration(const SetDisplayConfiguration *data) {
	if((data->contrast > 63) || (data->backlight > 100)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(uc1701.display_configuration_contrast != data->contrast) {
		uc1701.new_contrast  = true;
		uc1701.display_configuration_contrast = data->contrast;
	}

	if(uc1701.display_configuration_backlight != data->backlight) {
		uc1701.new_backlight = true;
		uc1701.display_configuration_backlight = data->backlight;
	}

	if(uc1701.display_configuration_invert != data->invert) {
		uc1701.new_invert    = true;
		uc1701.display_configuration_invert = data->invert;
	}

	uc1701.automatic_draw = data->automatic_draw;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display_configuration(const GetDisplayConfiguration *data, GetDisplayConfiguration_Response *response) {
	response->header.length  = sizeof(GetDisplayConfiguration_Response);
	response->contrast       = uc1701.display_configuration_contrast;
	response->backlight      = uc1701.display_configuration_backlight;
	response->invert         = uc1701.display_configuration_invert;
	response->automatic_draw = uc1701.automatic_draw;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse write_line(const WriteLine *data) {
	if((data->line > 7) || (data->position > 21)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	for(uint8_t i = 0; i < 21 - data->position; i++) {
		if(data->text[i] == 0) {
			if(uc1701.automatic_draw) {
				uc1701.display_user_changed = true;
			}
			return HANDLE_MESSAGE_RESPONSE_EMPTY;
		}

		for(uint8_t j = 0; j < 6; j++) {
			uint8_t new_data = ((j == 5) ? 0 : font[(uint8_t)data->text[i]*5 + j]);
			uint8_t column = (data->position+i)*6 + j;
			if(column >= LCD_MAX_COLUMNS) {
				if(uc1701.automatic_draw) {
					uc1701.display_user_changed = true;
				}
				return HANDLE_MESSAGE_RESPONSE_EMPTY;
			}

			if(uc1701.display_user[data->line][column] != new_data) {
				uc1701.display_user[data->line][column] = new_data;
			}
		}
	}

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse draw_buffered_frame(const DrawBufferedFrame *data) {
	if(data->force_complete_redraw) {
		uc1701.redraw_all = true;
	}

	uc1701.display_user_changed = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_touch_position(const GetTouchPosition *data, GetTouchPosition_Response *response) {
	response->header.length = sizeof(GetTouchPosition_Response);
	response->pressure      = tsc2046e.touch_pressure;
	if(response->pressure == 0) {
		response->x         = 0;
		response->y         = 0;
	} else {
		response->x         = tsc2046e.touch_x;
		response->y         = tsc2046e.touch_y;
	}

	response->age           = system_timer_get_ms() - tsc2046e.touch_time;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_touch_position_callback_configuration(const SetTouchPositionCallbackConfiguration *data) {
	tsc2046e.position_period              = data->period;
	tsc2046e.position_value_has_to_change = data->value_has_to_change;
	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_touch_position_callback_configuration(const GetTouchPositionCallbackConfiguration *data, GetTouchPositionCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetTouchPositionCallbackConfiguration_Response);
	response->period              = tsc2046e.position_period;
	response->value_has_to_change = tsc2046e.position_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_touch_gesture(const GetTouchGesture *data, GetTouchGesture_Response *response) {
	response->header.length = sizeof(GetTouchGesture_Response);
	response->gesture       = tsc2046e.gesture_api_gesture;
	response->pressure_max  = tsc2046e.gesture_api_pressure_max;
	response->x_start       = tsc2046e.gesture_api_x_start;
	response->y_start       = tsc2046e.gesture_api_y_start;
	response->x_end         = tsc2046e.gesture_api_x_end;
	response->y_end         = tsc2046e.gesture_api_y_end;
	response->duration      = tsc2046e.gesture_api_duration;
	response->age           = system_timer_get_ms() - tsc2046e.gesture_api_time;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_touch_gesture_callback_configuration(const SetTouchGestureCallbackConfiguration *data) {
	tsc2046e.gesture_period              = data->period;
	tsc2046e.gesture_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_touch_gesture_callback_configuration(const GetTouchGestureCallbackConfiguration *data, GetTouchGestureCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetTouchGestureCallbackConfiguration_Response);
	response->period              = tsc2046e.gesture_period;
	response->value_has_to_change = tsc2046e.gesture_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse draw_line(const DrawLine *data) {
	if((data->position_x_start >= LCD_MAX_COLUMNS) ||
	   (data->position_x_end >= LCD_MAX_COLUMNS) ||
	   (data->position_y_start >= LCD_MAX_ROWS*8) ||
	   (data->position_y_end >= LCD_MAX_ROWS*8)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.draw_to_user_display = true;
	gui_draw_line(data->position_x_start, data->position_y_start, data->position_x_end, data->position_y_end, data->color);
	gui.draw_to_user_display = false;

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse draw_box(const DrawBox *data) {
	if((data->position_x_start >= LCD_MAX_COLUMNS) ||
	   (data->position_x_end >= LCD_MAX_COLUMNS) ||
	   (data->position_y_start >= LCD_MAX_ROWS*8) ||
	   (data->position_y_end >= LCD_MAX_ROWS*8)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.draw_to_user_display = true;
	gui_draw_box(data->position_x_start, data->position_y_start, data->position_x_end, data->position_y_end, data->fill, data->color);
	gui.draw_to_user_display = false;

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse draw_text(const DrawText *data) {
	if((data->position_x >= LCD_MAX_COLUMNS) ||
	   (data->position_y >= LCD_MAX_ROWS*8)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(data->font > LCD_128X64_FONT_24X32) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	uint8_t char_width = 6;
	switch(data->font) {
		case LCD_128X64_FONT_6X8:   char_width = 6;  break;
		case LCD_128X64_FONT_6X16:  char_width = 6;  break;
		case LCD_128X64_FONT_6X24:  char_width = 6;  break;
		case LCD_128X64_FONT_6X32:  char_width = 6;  break;
		case LCD_128X64_FONT_12X16: char_width = 12; break;
		case LCD_128X64_FONT_12X24: char_width = 12; break;
		case LCD_128X64_FONT_12X32: char_width = 12; break;
		case LCD_128X64_FONT_18X24: char_width = 18; break;
		case LCD_128X64_FONT_18X32: char_width = 18; break;
		case LCD_128X64_FONT_24X32: char_width = 24; break;
	}

	uint8_t max_chars = ((LCD_MAX_COLUMNS - data->position_x + char_width - 1) / char_width) + 1;

	gui.draw_to_user_display = true;
	gui_draw_text(data->position_x, data->position_y, MIN(max_chars, strnlen(data->text, 22)), data->text, data->font, data->color);
	gui.draw_to_user_display = false;

	if(uc1701.automatic_draw) {
		uc1701.display_user_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_gui_button(const SetGUIButton *data) {
	if((data->position_x + data->width >= LCD_MAX_COLUMNS)   || 
	   (data->position_y + data->height >= (LCD_MAX_ROWS*8)) || 
	   (data->index >= GUI_BUTTON_NUM_MAX)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.button[data->index].active      = true;
	gui.button[data->index].position_x  = data->position_x;
	gui.button[data->index].position_y  = data->position_y;
	gui.button[data->index].width       = data->width;
	gui.button[data->index].height      = data->height;
	strncpy(gui.button[data->index].text, data->text, GUI_BUTTON_TEXT_LENGTH_MAX);

	gui_redraw();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_button(const GetGUIButton *data, GetGUIButton_Response *response) {
	if(data->index > GUI_BUTTON_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetGUIButton_Response);
	memset(response->text, 0, GUI_BUTTON_TEXT_LENGTH_MAX);

	if(gui.button[data->index].active) {
		response->active      = true;
		response->position_x  = gui.button[data->index].position_x;
		response->position_y  = gui.button[data->index].position_y;
		response->width       = gui.button[data->index].width;
		response->height      = gui.button[data->index].height;
		strncpy(response->text, gui.button[data->index].text, GUI_BUTTON_TEXT_LENGTH_MAX);
	} else {
		response->active      = false;
		response->position_x  = 0;
		response->position_y  = 0;
		response->width       = 0;
		response->height      = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse remove_gui_button(const RemoveGUIButton *data) {
	if(data->index == 255) {
		gui_remove_all(true, false, false, false);
	}

	if(data->index > GUI_BUTTON_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.button[data->index].active = false;
	gui_redraw();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_gui_button_pressed_callback_configuration(const SetGUIButtonPressedCallbackConfiguration *data) {
	gui.button_cb_period              = data->period;
	gui.button_cb_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_button_pressed_callback_configuration(const GetGUIButtonPressedCallbackConfiguration *data, GetGUIButtonPressedCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetGUIButtonPressedCallbackConfiguration_Response);
	response->period              = gui.button_cb_period;
	response->value_has_to_change = gui.button_cb_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_gui_button_pressed(const GetGUIButtonPressed *data, GetGUIButtonPressed_Response *response) {
	if(data->index > GUI_BUTTON_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetGUIButtonPressed_Response);
	response->pressed       = gui.button[data->index].pressed;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_slider(const SetGUISlider *data) {
	if((data->index >= GUI_SLIDER_NUM_MAX) ||
	   (data->direction > LCD_128X64_DIRECTION_VERTICAL) ||
	   (data->value > (data->length - GUI_SLIDER_KNOB_LENGTH))) { 
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((data->direction == LCD_128X64_DIRECTION_HORIZONTAL) && 
	   ((data->position_x + data->length >= LCD_MAX_COLUMNS) ||
	    (data->position_y + GUI_SLIDER_KNOB_WIDTH >= (LCD_MAX_ROWS*8)))) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((data->direction == LCD_128X64_DIRECTION_VERTICAL) && 
	   ((data->position_y + data->length >= (LCD_MAX_ROWS*8)) ||
	    (data->position_x + GUI_SLIDER_KNOB_WIDTH >= LCD_MAX_COLUMNS))) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.slider[data->index].active     = true;
	gui.slider[data->index].position_x = data->position_x;
	gui.slider[data->index].position_y = data->position_y;
	gui.slider[data->index].length     = data->length;
	gui.slider[data->index].direction  = data->direction;
	gui.slider[data->index].value      = data->value;

	gui_redraw();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_slider(const GetGUISlider *data, GetGUISlider_Response *response) {
	if(data->index > GUI_SLIDER_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetGUISlider_Response);
	response->active        = gui.slider[data->index].active;
	response->position_x    = gui.slider[data->index].position_x;
	response->position_y    = gui.slider[data->index].position_y;
	response->length        = gui.slider[data->index].length;
	response->direction     = gui.slider[data->index].direction;
	response->value         = gui.slider[data->index].value;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse remove_gui_slider(const RemoveGUISlider *data) {
	if(data->index == 255) {
		gui_remove_all(false, true, false, false);
	}

	if(data->index > GUI_SLIDER_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.slider[data->index].active = false;
	gui_redraw();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_gui_slider_value_callback_configuration(const SetGUISliderValueCallbackConfiguration *data) {
	gui.slider_cb_period              = data->period;
	gui.slider_cb_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_slider_value_callback_configuration(const GetGUISliderValueCallbackConfiguration *data, GetGUISliderValueCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetGUISliderValueCallbackConfiguration_Response);
	response->period              = gui.slider_cb_period;
	response->value_has_to_change = gui.slider_cb_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_gui_slider_value(const GetGUISliderValue *data, GetGUISliderValue_Response *response) {
	if(data->index > GUI_SLIDER_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetGUISliderValue_Response);
	response->value         = gui.slider[data->index].value;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_tab_configuration(const SetGUITabConfiguration *data) {
	if((data->change_tab_config < LCD_128X64_CHANGE_TAB_ON_CLICK) || (data->change_tab_config > LCD_128X64_CHANGE_TAB_ON_CLICK_AND_SWIPE)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.tab_change_tab_config = data->change_tab_config;
	gui.tab_clear_gui         = data->clear_gui;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_tab_configuration(const GetGUITabConfiguration *data, GetGUITabConfiguration_Response *response) {
	response->header.length     = sizeof(GetGUITabConfiguration_Response);
	response->change_tab_config = gui.tab_change_tab_config;
	response->clear_gui         = gui.tab_clear_gui;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_tab_text(const SetGUITabText *data) {
	if(data->index > GUI_TAB_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.tab[data->index].active_text = true;
	gui.tab[data->index].active_icon = false;
	strncpy(gui.tab[data->index].text, data->text, GUI_TAB_TEXT_LENGTH_MAX);

	if(gui.tab_current == -1) {
		gui.tab_current = data->index;
	}

	gui_update_tabs();
	gui_redraw();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_tab_text(const GetGUITabText *data, GetGUITabText_Response *response) {
	if(data->index > GUI_TAB_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetGUITabText_Response);
	memset(response->text, 0, GUI_BUTTON_TEXT_LENGTH_MAX);

	if(gui.tab[data->index].active_text) {
		response->active = true;
		strncpy(response->text, gui.tab[data->index].text, GUI_TAB_TEXT_LENGTH_MAX);
	} else {
		response->active = false;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_tab_icon(const SetGUITabIcon *data) {
	if(data->index > GUI_TAB_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.tab[data->index].active_icon = true;
	gui.tab[data->index].active_text = false;
	memcpy(gui.tab[data->index].icon, data->icon, 28*6/8);

	if(gui.tab_current == -1) {
		gui.tab_current = data->index;
	}

	gui_update_tabs();
	gui_redraw();	

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_tab_icon(const GetGUITabIcon *data, GetGUITabIcon_Response *response) {
	if(data->index > GUI_TAB_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->header.length = sizeof(GetGUITabIcon_Response);
	memset(response->icon, 0, 28*6/8);

	if(gui.tab[data->index].active_icon) {
		response->active = true;
		memcpy(response->icon, gui.tab[data->index].icon, 28*6/8);
	} else {
		response->active = false;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse remove_gui_tab(const RemoveGUITab *data) {
	if(data->index == 255) {
		gui_remove_all(false, false, false, true);
	}

	if(data->index > GUI_TAB_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.tab[data->index].active_icon = false;
	gui.tab[data->index].active_text = false;
	if(gui.tab_current == data->index) {
		gui.tab_current = -1;
		for(uint8_t i = 0; i < GUI_TAB_NUM_MAX; i++) {
			if(gui.tab[i].active_icon || gui.tab[i].active_text) {
				gui.tab_current = i;
				break;
			}
		}
	}

	gui_update_tabs();
	gui_redraw();

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_tab_current(const SetGUITabCurrent *data) {
	if(data->index > GUI_TAB_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(gui.tab[data->index].active_icon || gui.tab[data->index].active_text) {
		gui.tab_current = data->index;
		gui_update_tabs();
		gui_redraw();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_gui_tab_current_callback_configuration(const SetGUITabCurrentCallbackConfiguration *data) {
	gui.tab_cb_period              = data->period;
	gui.tab_cb_value_has_to_change = data->value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_tab_current_callback_configuration(const GetGUITabCurrentCallbackConfiguration *data, GetGUITabCurrentCallbackConfiguration_Response *response) {
	response->header.length       = sizeof(GetGUITabCurrentCallbackConfiguration_Response);
	response->period              = gui.tab_cb_period;
	response->value_has_to_change = gui.tab_cb_value_has_to_change;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_gui_tab_current(const GetGUITabCurrent *data, GetGUITabCurrent_Response *response) {
	response->header.length = sizeof(GetGUITabCurrent_Response);
	response->index         = gui.tab_current;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_graph_configuration(const SetGUIGraphConfiguration *data) {
	if(data->index > GUI_GRAPH_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(gui.graph[data->index].graph_type > LCD_128X64_GRAPH_TYPE_BAR) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((gui.graph[data->index].position_x >= LCD_MAX_COLUMNS) ||
	   (gui.graph[data->index].position_y >= LCD_MAX_ROWS*8)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((gui.graph[data->index].width  > GUI_GRAPH_DATA_LENGTH_MAX) ||
	   (gui.graph[data->index].height > 63)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.graph[data->index].active     = true;
	gui.graph[data->index].graph_type = data->graph_type;
	gui.graph[data->index].position_x = data->position_x;
	gui.graph[data->index].position_y = data->position_y;
	gui.graph[data->index].width      = data->width;
	gui.graph[data->index].height     = data->height;
	strncpy(gui.graph[data->index].text_x, data->text_x, GUI_GRAPH_TEXT_LENGTH_MAX);
	strncpy(gui.graph[data->index].text_y, data->text_y, GUI_GRAPH_TEXT_LENGTH_MAX);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_graph_configuration(const GetGUIGraphConfiguration *data, GetGUIGraphConfiguration_Response *response) {
	if(data->index > GUI_GRAPH_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	memset(response->text_x, 0, GUI_GRAPH_TEXT_LENGTH_MAX);
	memset(response->text_y, 0, GUI_GRAPH_TEXT_LENGTH_MAX);

	response->header.length = sizeof(GetGUIGraphConfiguration_Response);
	response->active        = gui.graph[data->index].active;
	response->graph_type    = gui.graph[data->index].graph_type;
	response->position_x    = gui.graph[data->index].position_x;
	response->position_y    = gui.graph[data->index].position_y;
	response->width         = gui.graph[data->index].width;
	response->height        = gui.graph[data->index].height;
	strncpy(response->text_x, gui.graph[data->index].text_x, GUI_GRAPH_TEXT_LENGTH_MAX);
	strncpy(response->text_y, gui.graph[data->index].text_y, GUI_GRAPH_TEXT_LENGTH_MAX);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_gui_graph_data_low_level(const SetGUIGraphDataLowLevel *data) {
	static uint8_t first_chunk[GUI_GRAPH_NUM_MAX][GUI_GRAPH_DATA_LENGTH_MAX/2] = {{0}};

	if(data->index > GUI_GRAPH_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(gui.graph[data->index].data_length > GUI_GRAPH_DATA_LENGTH_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	bool ready = false;

	if(data->data_chunk_offset == 0) {
		if(data->data_length > GUI_GRAPH_DATA_LENGTH_MAX/2) {
			memcpy(first_chunk[data->index], data->data_chunk_data, GUI_GRAPH_DATA_LENGTH_MAX/2);
		} else {
			memcpy(gui.graph[data->index].data, data->data_chunk_data, GUI_GRAPH_DATA_LENGTH_MAX/2);
			ready = true;
		}
	} else {
		memcpy(gui.graph[data->index].data, first_chunk[data->index], GUI_GRAPH_DATA_LENGTH_MAX/2);
		memcpy(gui.graph[data->index].data + GUI_GRAPH_DATA_LENGTH_MAX/2, data->data_chunk_data, GUI_GRAPH_DATA_LENGTH_MAX/2);
		ready = true;
	}

	if(ready) {
		memset(gui.graph[data->index].data + data->data_length, 0, GUI_GRAPH_DATA_LENGTH_MAX - data->data_length);
		gui.graph[data->index].data_length = data->data_length;
		gui_redraw();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_gui_graph_data_low_level(const GetGUIGraphDataLowLevel *data, GetGUIGraphDataLowLevel_Response *response) {
	if(data->index > GUI_GRAPH_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	static bool first[GUI_GRAPH_NUM_MAX] = {true, true, true, true};

	response->header.length = sizeof(GetGUIGraphDataLowLevel_Response);
	response->data_length   = gui.graph[data->index].data_length; 

	if(first[data->index]) {
		response->data_chunk_offset = 0;
	} else {
		response->data_chunk_offset = GUI_GRAPH_DATA_LENGTH_MAX/2;
	}

	memcpy(response->data_chunk_data, gui.graph[data->index].data + response->data_chunk_offset, GUI_GRAPH_DATA_LENGTH_MAX/2);

	if(gui.graph[data->index].data_length > GUI_GRAPH_DATA_LENGTH_MAX/2) {
		first[data->index] = !first[data->index];
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse remove_gui_graph(const RemoveGUIGraph *data) {
	if(data->index == 255) {
		gui_remove_all(false, false, true, false);
	}

	if(data->index > GUI_GRAPH_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(gui.graph[data->index].active) {
		gui_redraw();
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse remove_all_gui(const RemoveAllGUI *data) {
	gui_remove_all(true, true, true, true);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_touch_led_config(const SetTouchLEDConfig *data) {
	if(data->config > LCD_128X64_TOUCH_LED_CONFIG_SHOW_TOUCH) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	tsc2046e.led_state.config = data->config == LCD_128X64_TOUCH_LED_CONFIG_SHOW_TOUCH ? LED_FLICKER_CONFIG_EXTERNAL : data->config;
	// Set LED according to value
	if((tsc2046e.led_state.config == LED_FLICKER_CONFIG_OFF) || (tsc2046e.led_state.config == LED_FLICKER_CONFIG_EXTERNAL)) {
		if(tsc2046e.use_old_led_pin) {
			XMC_GPIO_SetOutputHigh(TSC2046E_LED_PIN_OLD);
		} else {
			XMC_GPIO_SetOutputHigh(TSC2046E_LED_PIN);
		}
	} else {
		if(tsc2046e.use_old_led_pin) {
			XMC_GPIO_SetOutputLow(TSC2046E_LED_PIN_OLD);
		} else {
			XMC_GPIO_SetOutputLow(TSC2046E_LED_PIN);
		}
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_touch_led_config(const GetTouchLEDConfig *data, GetTouchLEDConfig_Response *response) {
	response->header.length = sizeof(GetTouchLEDConfig_Response);
	response->config        = tsc2046e.led_state.config == LED_FLICKER_CONFIG_EXTERNAL ? LCD_128X64_TOUCH_LED_CONFIG_SHOW_TOUCH : tsc2046e.led_state.config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_touch_position_callback(void) {
	static bool is_buffered = false;
	static TouchPosition_Callback cb;
	static uint16_t last_pressure = 0;
	static uint16_t last_x = 0;
	static uint16_t last_y = 0;
	static uint32_t last_time = 0;

	if(!is_buffered) {
		if(tsc2046e.position_period == 0 ||
		    !system_timer_is_time_elapsed_ms(last_time, tsc2046e.position_period)) {
			return false;
		}

		if(tsc2046e.position_value_has_to_change && last_pressure == tsc2046e.touch_pressure && last_x == tsc2046e.touch_x && last_y == tsc2046e.touch_y) {
			return false;
		}

		// Never trigger callback by pressure of 1
		if(tsc2046e.touch_pressure == 1) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(TouchPosition_Callback), FID_CALLBACK_TOUCH_POSITION);
		cb.pressure = tsc2046e.touch_pressure;
		cb.x        = tsc2046e.touch_x;
		cb.y        = tsc2046e.touch_y;
		cb.age      = system_timer_get_ms() - tsc2046e.touch_time;

		last_pressure = tsc2046e.touch_pressure;
		last_x        = tsc2046e.touch_x;
		last_y        = tsc2046e.touch_y;

		last_time = system_timer_get_ms();
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(TouchPosition_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_touch_gesture_callback(void) {
	static bool is_buffered = false;
	static TouchGesture_Callback cb;
	static uint8_t last_gesture;
	static uint32_t last_duration;
	static uint16_t last_pressure_max;
	static uint16_t last_x_start;
	static uint16_t last_y_start;
	static uint16_t last_x_end;
	static uint16_t last_y_end;
	static uint32_t last_time = 0;

	if(!is_buffered) {
		if(tsc2046e.gesture_period == 0 ||
		    !system_timer_is_time_elapsed_ms(last_time, tsc2046e.gesture_period)) {
			return false;
		}

		if(tsc2046e.gesture_value_has_to_change                    &&
		   last_gesture       == tsc2046e.gesture_api_gesture      &&
		   last_pressure_max  == tsc2046e.gesture_api_pressure_max &&
		   last_x_start       == tsc2046e.gesture_api_x_start      &&
		   last_y_start       == tsc2046e.gesture_api_y_start      &&
		   last_x_end         == tsc2046e.gesture_api_x_end        &&
		   last_y_end         == tsc2046e.gesture_api_y_end        &&
		   last_duration      == tsc2046e.gesture_api_duration) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(TouchGesture_Callback), FID_CALLBACK_TOUCH_GESTURE);

		cb.gesture      = tsc2046e.gesture_api_gesture;
		cb.pressure_max = tsc2046e.gesture_api_pressure_max;
		cb.x_start      = tsc2046e.gesture_api_x_start;
		cb.y_start      = tsc2046e.gesture_api_y_start;
		cb.x_end        = tsc2046e.gesture_api_x_end;
		cb.y_end        = tsc2046e.gesture_api_y_end;
		cb.duration     = tsc2046e.gesture_api_duration;
		cb.age          = system_timer_get_ms() - tsc2046e.gesture_api_time;

		last_gesture      = tsc2046e.gesture_api_gesture;
		last_pressure_max = tsc2046e.gesture_api_pressure_max;
		last_x_start      = tsc2046e.gesture_api_x_start;
		last_y_start      = tsc2046e.gesture_api_y_start;
		last_x_end        = tsc2046e.gesture_api_x_end;
		last_y_end        = tsc2046e.gesture_api_y_end;
		last_duration     = tsc2046e.gesture_api_duration;

		last_time = system_timer_get_ms();
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(TouchGesture_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_gui_button_pressed_callback(void) {
	static bool is_buffered = false;
	static GUIButtonPressed_Callback cb;
	static bool last_pressed[GUI_BUTTON_NUM_MAX] = {false};
	static uint32_t last_time[GUI_BUTTON_NUM_MAX] = {0};
	static uint8_t last_index = 0;

	if(!is_buffered) {
		last_index = (last_index+1) % GUI_BUTTON_NUM_MAX;
		if((gui.button_cb_period == 0) || !system_timer_is_time_elapsed_ms(last_time[last_index], gui.button_cb_period)) {
			return false;
		}

		if(gui.button_cb_value_has_to_change && (last_pressed[last_index] == gui.button[last_index].pressed)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(GUIButtonPressed_Callback), FID_CALLBACK_GUI_BUTTON_PRESSED);
		cb.index   = last_index;
		cb.pressed = gui.button[last_index].pressed;

		last_pressed[last_index] = cb.pressed;
		last_time[last_index] = system_timer_get_ms();
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(GUIButtonPressed_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_gui_slider_value_callback(void) {
	static bool is_buffered = false;
	static GUISliderValue_Callback cb;
	static uint8_t last_value[GUI_SLIDER_NUM_MAX] = {0};
	static uint32_t last_time[GUI_SLIDER_NUM_MAX] = {0};
	static uint8_t last_index = 0;

	if(!is_buffered) {
		last_index = (last_index+1) % GUI_SLIDER_NUM_MAX;
		if((gui.slider_cb_period == 0) || !system_timer_is_time_elapsed_ms(last_time[last_index], gui.slider_cb_period)) {
			return false;
		}

		if(gui.slider_cb_value_has_to_change && (last_value[last_index] == gui.slider[last_index].value)) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(GUISliderValue_Callback), FID_CALLBACK_GUI_SLIDER_VALUE);
		cb.index = last_index;
		cb.value = gui.slider[last_index].value;

		last_value[last_index] = cb.value;
		last_time[last_index] = system_timer_get_ms();
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(GUISliderValue_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_gui_tab_current_callback(void) {
	static bool is_buffered = false;
	static GUITabCurrent_Callback cb;
	static uint32_t last_time = 0;
	static int8_t last_current = 0;

	if(!is_buffered) {
		if((gui.tab_cb_period == 0) || !system_timer_is_time_elapsed_ms(last_time, gui.tab_cb_period)) {
			return false;
		}

		if(gui.tab_cb_value_has_to_change && (last_current == gui.tab_current)) {
			return false;
		}

		last_current = gui.tab_current;
		if(last_current == -1) {
			return false;
		}

		tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(GUITabCurrent_Callback), FID_CALLBACK_GUI_TAB_CURRENT);
		cb.index = gui.tab_current;

		last_time = system_timer_get_ms();
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(GUITabCurrent_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
