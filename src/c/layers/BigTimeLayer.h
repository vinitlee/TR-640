#pragma once

#include <pebble.h>

typedef Layer BigTimeLayer;

typedef enum
{
    BIG_TIME_POS_NONE = 0,
    BIG_TIME_POS_H,
    BIG_TIME_POS_M,
    BIG_TIME_POS_S,
} BigTimeLayerPosition;

typedef struct BigTimeLayerData BigTimeLayerData;

BigTimeLayer *big_time_layer_create(GRect frame);
void big_time_layer_destroy(BigTimeLayer *big_time_layer);
void big_time_layer_set_font(BigTimeLayer *big_time_layer, GFont font, int baseline);
void big_time_layer_set_time(BigTimeLayer *big_time_layer, time_t t);
void big_time_layer_zero(BigTimeLayer *big_time_layer);
void big_time_layer_set_colors(BigTimeLayer *big_time_layer, GColor color_text, GColor color_highlight, GColor color_inactive);
void big_time_layer_highlight(BigTimeLayer *big_time_layer, BigTimeLayerPosition pos);
void big_time_layer_clear_highlight(BigTimeLayer *big_time_layer);