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

#define GUI_BUTTON_NUM_MAX 12
#define GUI_BUTTON_TEXT_LENGTH_MAX 16

#define GUI_SLIDER_NUM_MAX 6
#define GUI_SLIDER_KNOB_WIDTH 17
#define GUI_SLIDER_KNOB_LENGTH 8

#define GUI_TAB_NUM_MAX 10
#define GUI_TAB_TEXT_LENGTH_MAX 5

#define GUI_TAB_POS_Y  54
#define GUI_TAB0_POS_X 2
#define GUI_TAB1_POS_X 43
#define GUI_TAB2_POS_X 84
#define GUI_TAB3_POS_X 125
#define GUI_TAB_WIDTH  41

#define GUI_GRAPH_NUM_MAX 4
#define GUI_GRAPH_TEXT_LENGTH_MAX 4
#define GUI_GRAPH_DATA_LENGTH_MAX 118

typedef struct {
    bool active;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t width;
	uint8_t height;
	char text[GUI_BUTTON_TEXT_LENGTH_MAX];
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
    bool active_text;
    bool active_icon;
    char text[GUI_TAB_TEXT_LENGTH_MAX];
    uint8_t icon[168/8];
} GUITab;

typedef struct {
    bool active;
    uint8_t graph_type;
	uint8_t position_x;
	uint8_t position_y;
	uint8_t width;
	uint8_t height;
	char text_x[GUI_GRAPH_TEXT_LENGTH_MAX];
	char text_y[GUI_GRAPH_TEXT_LENGTH_MAX];
	uint8_t data[GUI_GRAPH_DATA_LENGTH_MAX];
    uint8_t data_length;
} GUIGraph;

typedef struct {
    GUIButton button[GUI_BUTTON_NUM_MAX];
	uint32_t button_cb_period;
	bool button_cb_value_has_to_change;

    GUISlider slider[GUI_SLIDER_NUM_MAX];
	uint32_t slider_cb_period;
	bool slider_cb_value_has_to_change;

    GUITab tab[GUI_TAB_NUM_MAX];
    int8_t tab_current;
	uint32_t tab_cb_period;
	bool tab_cb_value_has_to_change;

    uint8_t tab_change_tab_config;
    bool tab_clear_gui;
    
    int8_t tabs[GUI_TAB_NUM_MAX]; // tab indexes from left to right
    uint8_t tabs_count;
    uint8_t tabs_current;
    uint8_t tabs_open_lid;

    bool use_global_bounding_box;
    uint8_t global_bounding_box_start_x;
    uint8_t global_bounding_box_end_x;
    uint8_t global_bounding_box_start_y;
    uint8_t global_bounding_box_end_y;

    uint32_t last_tab_gesture_time;
    uint32_t last_tab_touch_time;

    GUIGraph graph[GUI_GRAPH_NUM_MAX];

    bool draw_to_user_display;
} GUI;

extern GUI gui;

void gui_draw_text(const uint8_t column, const uint8_t row, const uint8_t text_length, const char *text, const uint8_t font_type, const bool color);
void gui_draw_box(const uint8_t x_start, const uint8_t y_start, const uint8_t x_end, const uint8_t y_end, const bool fill, const bool color);
void gui_draw_line(const uint8_t x_start, const uint8_t y_start, const uint8_t x_end, const uint8_t y_end, const bool color);

void gui_remove_all(const bool buttons, const bool slider, const bool graphs, const bool tabs);
void gui_update_tabs(void);
void gui_redraw(void);
void gui_draw_button(const uint8_t index);
void gui_draw_slider(const uint8_t index);
void gui_init(void);
void gui_tick(void);

#endif