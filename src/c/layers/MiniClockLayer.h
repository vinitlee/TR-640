#pragma once

#include <pebble.h>

typedef struct MiniClockLayer MiniClockLayer;

MiniClockLayer *mini_clock_layer_create(GRect frame);
Layer *mini_clock_layer_get_layer(MiniClockLayer *mini_clock_layer);
void mini_clock_layer_destroy(MiniClockLayer *mini_clock_layer);
void mini_clock_layer_update(MiniClockLayer *mini_clock_layer);
void mini_clock_layer_update_with(MiniClockLayer *mini_clock_layer, struct tm *t);
void mini_clock_layer_set_font(MiniClockLayer *mini_clock_layer, GFont font);
void mini_clock_layer_set_text_color(MiniClockLayer *mini_clock_layer, GColor color);
void mini_clock_layer_set_background_color(MiniClockLayer *mini_clock_layer, GColor color);