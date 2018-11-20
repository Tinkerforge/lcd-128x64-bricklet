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
			uc1701.display_mask_changed = true;
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
			if(uc1701.display[row_index][column] & (1 << row_bit)) {
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
			uc1701.display_mask[row][column] = 0xFF;
			uc1701.display[row][column] = 0;
		}
	}

	if(uc1701.automatic_draw) {
		uc1701.display_mask_changed = true;
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
				uc1701.display_mask_changed = true;
			}
			return HANDLE_MESSAGE_RESPONSE_EMPTY;
		}

		for(uint8_t j = 0; j < 6; j++) {
			uint8_t new_data = ((j == 5) ? 0 : font[(uint8_t)data->text[i]*5 + j]);
			uint8_t column = (data->position+i)*6 + j;
			if(column >= LCD_MAX_COLUMNS) {
				if(uc1701.automatic_draw) {
					uc1701.display_mask_changed = true;
				}
				return HANDLE_MESSAGE_RESPONSE_EMPTY;
			}

			if(uc1701.display[data->line][column] != new_data) {
				uc1701.display_mask[data->line][column] = (uc1701.display[data->line][column] ^ new_data);
				uc1701.display[data->line][column] = new_data;
			}
		}
	}

	if(uc1701.automatic_draw) {
		uc1701.display_mask_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse draw_buffered_frame(const DrawBufferedFrame *data) {
	if(data->force_complete_redraw) {
		memset(uc1701.display_mask, 0xFF, LCD_MAX_COLUMNS*LCD_MAX_ROWS);
	}

	uc1701.display_mask_changed = true;

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
	memcpy(gui.button[data->index].text, data->text, GUI_BUTTON_TEXT_LENGTH_MAX);

	gui_draw_button(data->index);

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
	if(data->index > GUI_BUTTON_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.button[data->index].active = false;

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

	gui_draw_slider(data->index);

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
	if(data->index > GUI_SLIDER_NUM_MAX) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	gui.slider[data->index].active = false;

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

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
