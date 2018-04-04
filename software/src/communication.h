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

#define FID_CALLBACK_TOUCH_POSITION 11
#define FID_CALLBACK_TOUCH_GESTURE 15

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
	uint16_t x_start;
	uint16_t y_start;
	uint16_t x_end;
	uint16_t y_end;
	uint32_t age;
} __attribute__((__packed__)) TouchGesture_Callback;


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

// Callbacks
bool handle_touch_position_callback(void);
bool handle_touch_gesture_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 2
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_touch_position_callback, \
	handle_touch_gesture_callback, \


#endif
