#include "MiniClockLayer.h"

struct MiniClockLayer
{
    TextLayer *text_layer;
    char buffer[12]; // HH:MM:SS AM
};

MiniClockLayer *mini_clock_layer_create(GRect frame)
{
    MiniClockLayer *self = calloc(1, sizeof(MiniClockLayer));

    self->text_layer = text_layer_create(frame);
    memset(self->buffer, 0, sizeof(self->buffer));
    text_layer_set_background_color(self->text_layer, GColorClear);
    text_layer_set_text_color(self->text_layer, GColorBlack);
    text_layer_set_text_alignment(self->text_layer, GTextAlignmentCenter);
    return self;
}

Layer *mini_clock_layer_get_layer(MiniClockLayer *mini_clock_layer)
{
    return text_layer_get_layer(mini_clock_layer->text_layer);
}

void mini_clock_layer_destroy(MiniClockLayer *mini_clock_layer)
{
    if (mini_clock_layer->text_layer)
    {
        text_layer_destroy(mini_clock_layer->text_layer);
    }
}

void mini_clock_layer_update(MiniClockLayer *mini_clock_layer)
{
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    mini_clock_layer_update_with(mini_clock_layer, tick_time);
}

void mini_clock_layer_update_with(MiniClockLayer *mini_clock_layer, struct tm *t)
{
    strftime(mini_clock_layer->buffer, sizeof(mini_clock_layer->buffer), clock_is_24h_style() ? "%H:%M:%S" : "%I:%M:%S %p", t);
    text_layer_set_text(mini_clock_layer->text_layer, mini_clock_layer->buffer);
}

void mini_clock_layer_set_font(MiniClockLayer *mini_clock_layer, GFont font)
{
    text_layer_set_font(mini_clock_layer->text_layer, font);
}

void mini_clock_layer_set_text_alignment(MiniClockLayer *mini_clock_layer, GTextAlignment text_alignment)
{
    text_layer_set_text_alignment(mini_clock_layer->text_layer, text_alignment);
}

void mini_clock_layer_set_text_color(MiniClockLayer *mini_clock_layer, GColor color)
{
    text_layer_set_text_color(mini_clock_layer->text_layer, color);
}

void mini_clock_layer_set_background_color(MiniClockLayer *mini_clock_layer, GColor color)
{
    text_layer_set_background_color(mini_clock_layer->text_layer, color);
}
