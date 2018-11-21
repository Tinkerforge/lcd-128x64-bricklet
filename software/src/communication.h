/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define LCD_128X64_GESTURE_LEFT_TO_RIGHT 0
#define LCD_128X64_GESTURE_RIGHT_TO_LEFT 1
#define LCD_128X64_GESTURE_TOP_TO_BOTTOM 2
#define LCD_128X64_GESTURE_BOTTOM_TO_TOP 3

#define LCD_128X64_DIRECTION_HORIZONTAL 0
#define LCD_128X64_DIRECTION_VERTICAL 1

#define LCD_128X64_CHANGE_TAB_ON_CLICK 1
#define LCD_128X64_CHANGE_TAB_ON_SWIPE 2
#define LCD_128X64_CHANGE_TAB_ON_CLICK_AND_SWIPE 3

#define LCD_128X64_BOOTLOADER_MODE_BOOTLOADER 0
#define LCD_128X64_BOOTLOADER_MODE_FIRMWARE 1
#define LCD_128X64_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define LCD_128X64_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define LCD_128X64_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define LCD_128X64_BOOTLOADER_STATUS_OK 0
#define LCD_128X64_BOOTLOADER_STATUS_INVALID_MODE 1
#define LCD_128X64_BOOTLOADER_STATUS_NO_CHANGE 2
#define LCD_128X64_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define LCD_128X64_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define LCD_128X64_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define LCD_128X64_STATUS_LED_CONFIG_OFF 0
#define LCD_128X64_STATUS_LED_CONFIG_ON 1
#define LCD_128X64_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define LCD_128X64_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_WRITE_PIXELS_LOW_LEVEL 1
#define FID_READ_PIXELS_LOW_LEVEL 2
#define FID_CLEAR_DISPLAY 3
#define FID_SET_DISPLAY_CONFIGURATION 4
#define FID_GET_DISPLAY_CONFIGURATION 5
#define FID_WRITE_LINE 6
#define FID_DRAW_BUFFERED_FRAME 7
#define FID_GET_TOUCH_POSITION 8
#define FID_SET_TOUCH_POSITION_CALLBACK_CONFIGURATION 9
#define FID_GET_TOUCH_POSITION_CALLBACK_CONFIGURATION 10
#define FID_GET_TOUCH_GESTURE 12
#define FID_SET_TOUCH_GESTURE_CALLBACK_CONFIGURATION 13
#define FID_GET_TOUCH_GESTURE_CALLBACK_CONFIGURATION 14
#define FID_SET_GUI_BUTTON 16
#define FID_GET_GUI_BUTTON 17
#define FID_REMOVE_GUI_BUTTON 18
#define FID_SET_GUI_BUTTON_PRESSED_CALLBACK_CONFIGURATION 19
#define FID_GET_GUI_BUTTON_PRESSED_CALLBACK_CONFIGURATION 20
#define FID_GET_GUI_BUTTON_PRESSED 21
#define FID_SET_GUI_SLIDER 23
#define FID_GET_GUI_SLIDER 24
#define FID_REMOVE_GUI_SLIDER 25
#define FID_SET_GUI_SLIDER_VALUE_CALLBACK_CONFIGURATION 26
#define FID_GET_GUI_SLIDER_VALUE_CALLBACK_CONFIGURATION 27
#define FID_GET_GUI_SLIDER_VALUE 28
#define FID_SET_GUI_TAB_CONFIGURATION 30
#define FID_GET_GUI_TAB_CONFIGURATION 31
#define FID_SET_GUI_TAB_TEXT 32
#define FID_GET_GUI_TAB_TEXT 33
#define FID_REMOVE_GUI_TAB 34
#define FID_SET_GUI_TAB_CURRENT 35
#define FID_SET_GUI_TAB_CURRENT_CALLBACK_CONFIGURATION 36
#define FID_GET_GUI_TAB_CURRENT_CALLBACK_CONFIGURATION 37
#define FID_GET_GUI_TAB_CURRENT 38

#define FID_CALLBACK_TOUCH_POSITION 11
#define FID_CALLBACK_TOUCH_GESTURE 15
#define FID_CALLBACK_GUI_BUTTON_PRESSED 22
#define FID_CALLBACK_GUI_SLIDER_VALUE 29
#define FID_CALLBACK_GUI_TAB_CURRENT 39

typedef struct {
	TFPMessageHeader header;
	uint8_t x_start;
	uint8_t y_start;
	uint8_t x_end;
	uint8_t y_end;
	uint16_t pixels_length;
	uint16_t pixels_chunk_offset;
	uint8_t pixels_chunk_data[448/8];
} __attribute__((__packed__)) WritePixelsLowLevel;

typedef struct {
	TFPMessageHeader header;
	uint8_t x_start;
	uint8_t y_start;
	uint8_t x_end;
	uint8_t y_end;
} __attribute__((__packed__)) ReadPixelsLowLevel;

typedef struct {
	TFPMessageHeader header;
	uint16_t pixels_length;
	uint16_t pixels_chunk_offset;
	uint8_t pixels_chunk_data[480/8];
} __attribute__((__packed__)) ReadPixelsLowLevel_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) ClearDisplay;

typedef struct {
	TFPMessageHeader header;
	uint8_t contrast;
	uint8_t backlight;
	bool invert;
	bool automatic_draw;
} __attribute__((__packed__)) SetDisplayConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDisplayConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t contrast;
	uint8_t backlight;
	bool invert;
	bool automatic_draw;
} __attribute__((__packed__)) GetDisplayConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t line;
	uint8_t position;
	char text[22];
} __attribute__((__packed__)) WriteLine;

typedef struct {
	TFPMessageHeader header;
	bool force_complete_redraw;
} __attribute__((__packed__)) DrawBufferedFrame;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetTouchPosition;

typedef struct {
	TFPMessageHeader header;
	uint16_t pressure;
	uint16_t x;
	uint16_t y;
	uint32_t age;
} __attribute__((__packed__)) GetTouchPosition_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetTouchPositionCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetTouchPositionCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetTouchPositionCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t pressure;
	uint16_t x;
	uint16_t y;
	uint32_t age;
} __attribute__((__packed__)) TouchPosition_Callback;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetTouchGesture;

typedef struct {
	TFPMessageHeader header;
	uint8_t gesture;
	uint32_t duration;
	uint16_t pressure_max;
	uint16_t x_start;
	uint16_t y_start;
	uint16_t x_end;
	uint16_t y_end;
	uint32_t age;
} __attribute__((__packed__)) GetTouchGesture_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetTouchGestureCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetTouchGestureCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetTouchGestureCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t gesture;
	uint32_t duration;
	uint16_t pressure_max;
	uint16_t x_start;
	uint16_t y_start;
	uint16_t x_end;
	uint16_t y_end;
	uint32_t age;
} __attribute__((__packed__)) TouchGesture_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t width;
	uint8_t height;
	char text[16];
} __attribute__((__packed__)) SetGUIButton;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) GetGUIButton;

typedef struct {
	TFPMessageHeader header;
	bool active;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t width;
	uint8_t height;
	char text[16];
} __attribute__((__packed__)) GetGUIButton_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) RemoveGUIButton;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetGUIButtonPressedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetGUIButtonPressedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetGUIButtonPressedCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) GetGUIButtonPressed;

typedef struct {
	TFPMessageHeader header;
	bool pressed;
} __attribute__((__packed__)) GetGUIButtonPressed_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
	bool pressed;
} __attribute__((__packed__)) GUIButtonPressed_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t length;
	uint8_t direction;
	uint8_t value;
} __attribute__((__packed__)) SetGUISlider;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) GetGUISlider;

typedef struct {
	TFPMessageHeader header;
	bool active;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t length;
	uint8_t direction;
	uint8_t value;
} __attribute__((__packed__)) GetGUISlider_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) RemoveGUISlider;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetGUISliderValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetGUISliderValueCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetGUISliderValueCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) GetGUISliderValue;

typedef struct {
	TFPMessageHeader header;
	uint8_t value;
} __attribute__((__packed__)) GetGUISliderValue_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
	uint8_t value;
} __attribute__((__packed__)) GUISliderValue_Callback;

typedef struct {
	TFPMessageHeader header;
	uint8_t change_tab_config;
	bool clear_gui;
} __attribute__((__packed__)) SetGUITabConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetGUITabConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t change_tab_config;
	bool clear_gui;
} __attribute__((__packed__)) GetGUITabConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
	char text[8];
} __attribute__((__packed__)) SetGUITabText;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) GetGUITabText;

typedef struct {
	TFPMessageHeader header;
	bool active;
	char text[8];
} __attribute__((__packed__)) GetGUITabText_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) RemoveGUITab;

typedef struct {
	TFPMessageHeader header;
	uint8_t index;
} __attribute__((__packed__)) SetGUITabCurrent;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) SetGUITabCurrentCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetGUITabCurrentCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
	bool value_has_to_change;
} __attribute__((__packed__)) GetGUITabCurrentCallbackConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetGUITabCurrent;

typedef struct {
	TFPMessageHeader header;
	int8_t index;
} __attribute__((__packed__)) GetGUITabCurrent_Response;

typedef struct {
	TFPMessageHeader header;
	int8_t index;
} __attribute__((__packed__)) GUITabCurrent_Callback;


// Function prototypes
BootloaderHandleMessageResponse write_pixels_low_level(const WritePixelsLowLevel *data);
BootloaderHandleMessageResponse read_pixels_low_level(const ReadPixelsLowLevel *data, ReadPixelsLowLevel_Response *response);
BootloaderHandleMessageResponse clear_display(const ClearDisplay *data);
BootloaderHandleMessageResponse set_display_configuration(const SetDisplayConfiguration *data);
BootloaderHandleMessageResponse get_display_configuration(const GetDisplayConfiguration *data, GetDisplayConfiguration_Response *response);
BootloaderHandleMessageResponse write_line(const WriteLine *data);
BootloaderHandleMessageResponse draw_buffered_frame(const DrawBufferedFrame *data);
BootloaderHandleMessageResponse get_touch_position(const GetTouchPosition *data, GetTouchPosition_Response *response);
BootloaderHandleMessageResponse set_touch_position_callback_configuration(const SetTouchPositionCallbackConfiguration *data);
BootloaderHandleMessageResponse get_touch_position_callback_configuration(const GetTouchPositionCallbackConfiguration *data, GetTouchPositionCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse get_touch_gesture(const GetTouchGesture *data, GetTouchGesture_Response *response);
BootloaderHandleMessageResponse set_touch_gesture_callback_configuration(const SetTouchGestureCallbackConfiguration *data);
BootloaderHandleMessageResponse get_touch_gesture_callback_configuration(const GetTouchGestureCallbackConfiguration *data, GetTouchGestureCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse set_gui_button(const SetGUIButton *data);
BootloaderHandleMessageResponse get_gui_button(const GetGUIButton *data, GetGUIButton_Response *response);
BootloaderHandleMessageResponse remove_gui_button(const RemoveGUIButton *data);
BootloaderHandleMessageResponse set_gui_button_pressed_callback_configuration(const SetGUIButtonPressedCallbackConfiguration *data);
BootloaderHandleMessageResponse get_gui_button_pressed_callback_configuration(const GetGUIButtonPressedCallbackConfiguration *data, GetGUIButtonPressedCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse get_gui_button_pressed(const GetGUIButtonPressed *data, GetGUIButtonPressed_Response *response);
BootloaderHandleMessageResponse set_gui_slider(const SetGUISlider *data);
BootloaderHandleMessageResponse get_gui_slider(const GetGUISlider *data, GetGUISlider_Response *response);
BootloaderHandleMessageResponse remove_gui_slider(const RemoveGUISlider *data);
BootloaderHandleMessageResponse set_gui_slider_value_callback_configuration(const SetGUISliderValueCallbackConfiguration *data);
BootloaderHandleMessageResponse get_gui_slider_value_callback_configuration(const GetGUISliderValueCallbackConfiguration *data, GetGUISliderValueCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse get_gui_slider_value(const GetGUISliderValue *data, GetGUISliderValue_Response *response);
BootloaderHandleMessageResponse set_gui_tab_configuration(const SetGUITabConfiguration *data);
BootloaderHandleMessageResponse get_gui_tab_configuration(const GetGUITabConfiguration *data, GetGUITabConfiguration_Response *response);
BootloaderHandleMessageResponse set_gui_tab_text(const SetGUITabText *data);
BootloaderHandleMessageResponse get_gui_tab_text(const GetGUITabText *data, GetGUITabText_Response *response);
BootloaderHandleMessageResponse remove_gui_tab(const RemoveGUITab *data);
BootloaderHandleMessageResponse set_gui_tab_current(const SetGUITabCurrent *data);
BootloaderHandleMessageResponse set_gui_tab_current_callback_configuration(const SetGUITabCurrentCallbackConfiguration *data);
BootloaderHandleMessageResponse get_gui_tab_current_callback_configuration(const GetGUITabCurrentCallbackConfiguration *data, GetGUITabCurrentCallbackConfiguration_Response *response);
BootloaderHandleMessageResponse get_gui_tab_current(const GetGUITabCurrent *data, GetGUITabCurrent_Response *response);

// Callbacks
bool handle_touch_position_callback(void);
bool handle_touch_gesture_callback(void);
bool handle_gui_button_pressed_callback(void);
bool handle_gui_slider_value_callback(void);
bool handle_gui_tab_current_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 5
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_touch_position_callback, \
	handle_touch_gesture_callback, \
	handle_gui_button_pressed_callback, \
	handle_gui_slider_value_callback, \
	handle_gui_tab_current_callback, \


#endif
