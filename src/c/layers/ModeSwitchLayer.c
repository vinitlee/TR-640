#include "ModeSwitchLayer.h"

struct ModeSwitchLayer
{
    TextLayer *text_layer;
    GColor color_highlight;
    GColor color_inactive;
    ModeSwitchMode mode;
};

static void mode_switch_layer_update(ModeSwitchLayer *self)
{
    text_layer_set_text_color(self->text_layer, (self->mode == MODE_AUTO) ? self->color_highlight : self->color_inactive);
}

ModeSwitchLayer *mode_switch_layer_create(GRect frame)
{
    ModeSwitchLayer *self = calloc(1, sizeof(ModeSwitchLayer));

    self->text_layer = text_layer_create(grect_inset(frame, GEdgeInsets4(-4, 0, 4, 0)));
    text_layer_set_text(self->text_layer, "AUTO");
    text_layer_set_font(self->text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
    text_layer_set_background_color(self->text_layer, GColorClear);
    text_layer_set_text_alignment(self->text_layer, GTextAlignmentCenter);

    self->color_highlight = GColorMagenta;
    self->color_inactive = GColorDarkGray;
    self->mode = MODE_AUTO;

    mode_switch_layer_update(self);

    return self;
}
Layer *mode_switch_layer_get_layer(ModeSwitchLayer *self)
{
    return text_layer_get_layer(self->text_layer);
}
void mode_switch_layer_destroy(ModeSwitchLayer *self)
{
    if (self->text_layer)
    {
        text_layer_destroy(self->text_layer);
    }
}
void mode_switch_layer_set_mode(ModeSwitchLayer *self, ModeSwitchMode mode)
{
    self->mode = mode;
    mode_switch_layer_update(self);
}
void mode_switch_layer_set_colors(ModeSwitchLayer *self, GColor color_highlight, GColor color_inactive)
{
    self->color_highlight = color_highlight;
    self->color_inactive = color_inactive;
    mode_switch_layer_update(self);
}