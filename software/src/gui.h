/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * gui.h: Implementation of simple GUI elements
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

#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <stdbool.h>

#define GUI_BUTTON_NUM_MAX 10
#define GUI_BUTTON_TEXT_LENGTH_MAX 16

#define GUI_SLIDER_NUM_MAX 10
#define GUI_SLIDER_KNOB_WIDTH 17
#define GUI_SLIDER_KNOB_LENGTH 8

typedef struct {
    bool active;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t width;
	uint8_t height;
	char text[GUI_BUTTON_TEXT_LENGTH_MAX+1];
    bool pressed;
} GUIButton;

typedef struct {
    bool active;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t length;
    uint8_t direction;
    uint8_t value;
    bool pressed;
} GUISlider;

typedef struct {
    GUIButton button[GUI_BUTTON_NUM_MAX];
	uint32_t button_cb_period;
	bool button_cb_value_has_to_change;

    GUISlider slider[GUI_SLIDER_NUM_MAX];
	uint32_t slider_cb_period;
	bool slider_cb_value_has_to_change;

    bool use_global_bounding_box;
    uint8_t global_bounding_box_start_x;
    uint8_t global_bounding_box_end_x;
    uint8_t global_bounding_box_start_y;
    uint8_t global_bounding_box_end_y;
} GUI;

extern GUI gui;

void gui_draw_button(uint8_t index);
void gui_draw_slider(uint8_t index);
void gui_init(void);
void gui_tick(void);

#endif