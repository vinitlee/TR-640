#pragma once

#include <pebble.h>

typedef enum {
    MODE_AUTO = 0,
    MODE_ONESHOT,
} ModeSwitchMode;

typedef struct ModeSwitchLayer ModeSwitchLayer;

ModeSwitchLayer *mode_switch_layer_create(GRect frame);
Layer *mode_switch_layer_get_layer(ModeSwitchLayer *mode_switch_layer);
void mode_switch_layer_destroy(ModeSwitchLayer *mode_switch_layer);
void mode_switch_layer_set_mode(ModeSwitchLayer *mode_switch_layer,ModeSwitchMode mode);
void mode_switch_layer_set_colors(ModeSwitchLayer *mode_switch_layer, GColor color_highlight, GColor color_inactive);