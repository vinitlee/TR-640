#pragma once

#include <pebble.h>

typedef Layer TimeLayer;

typedef enum
{
    TIME_LAYER_NONE = 0,
    TIME_LAYER_HRS,
    TIME_LAYER_MINS,
    TIME_LAYER_SECS,
} TimeLayerPosition;

typedef struct
{
    GFont font;
    GColor foreground_color;
    GColor background_color;
    GColor highlight_color;
    int baseline;
    char h_buff[3];
    char m_buff[3];
    char s_buff[3];
    GRect h_rect;
    GRect m_rect;
    GRect s_rect;
    GColor h_color;
    GColor m_color;
    GColor s_color;
    TimeLayerPosition highlight;
} TimeLayerData;

TimeLayer *time_layer_create(GRect frame);
void time_layer_destroy(TimeLayer *time_layer);
void time_layer_set_font(TimeLayer *time_layer, GFont font, int offset);
void time_layer_set_hrs(TimeLayer *time_layer, int val);
void time_layer_set_mins(TimeLayer *time_layer, int val);
void time_layer_set_secs(TimeLayer *time_layer, int val);
void time_layer_zero(TimeLayer *time_layer);
void time_layer_highlight(TimeLayer *time_layer, TimeLayerPosition pos);
