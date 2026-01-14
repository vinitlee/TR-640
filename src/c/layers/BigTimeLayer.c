#include "BigTimeLayer.h"

struct BigTimeLayerData
{
    GFont font;
    int baseline_offset;
    GRect rect_h;
    GRect rect_m;
    GRect rect_s;
    int val_h;
    int val_m;
    int val_s;
    char buff_h[3];
    char buff_m[3];
    char buff_s[3];
    GColor color_text;
    GColor color_highlight;
    GColor color_inactive;
    BigTimeLayerPosition highlight_pos;
};

static void big_time_layer_update_geometry(BigTimeLayer *big_time_layer)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    GRect bounds = layer_get_bounds(big_time_layer);

    GSize layout_size = graphics_text_layout_get_content_size("00", data->font, bounds, GTextOverflowModeFill, GTextAlignmentCenter);
    GSize text_size = {.w = bounds.size.w, .h = layout_size.h};
    GPoint bounds_center = grect_center_point(&bounds);

    GRect mins_rect = GRect(0, bounds_center.y - text_size.h / 2 + data->baseline_offset, text_size.w, text_size.h);
    GRect hrs_rect = GRect(mins_rect.origin.x, mins_rect.origin.y - text_size.h, mins_rect.size.w, mins_rect.size.h);
    GRect secs_rect = GRect(mins_rect.origin.x, mins_rect.origin.y + text_size.h, mins_rect.size.w, mins_rect.size.h);

    data->rect_h = hrs_rect;
    data->rect_m = mins_rect;
    data->rect_s = secs_rect;
}

static GColor get_position_color(BigTimeLayerData *data, BigTimeLayerPosition pos)
{
    if (data->highlight_pos == BIG_TIME_POS_NONE)
    {
        switch (pos)
        {
        case BIG_TIME_POS_H:
            if (data->val_h == 0)
                return data->color_inactive;
            break;
        case BIG_TIME_POS_M:
            if (data->val_h == 0 && data->val_m == 0)
                return data->color_inactive;
            break;
        case BIG_TIME_POS_S:
            if (data->val_h == 0 && data->val_m == 0 && data->val_s == 0)
                return data->color_inactive;
            break;
        default:
            break;
        }
    }
    else if (data->highlight_pos == pos)
    {
        return data->color_highlight;
    }

    return data->color_text;
}

static void big_time_layer_update_proc(BigTimeLayer *big_time_layer, GContext *ctx)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    GRect bounds = layer_get_bounds(big_time_layer);

    graphics_context_set_text_color(ctx, get_position_color(data, BIG_TIME_POS_H));
    graphics_draw_text(ctx, data->buff_h, data->font, data->rect_h, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
    graphics_context_set_text_color(ctx, get_position_color(data, BIG_TIME_POS_M));
    graphics_draw_text(ctx, data->buff_m, data->font, data->rect_m, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
    graphics_context_set_text_color(ctx, get_position_color(data, BIG_TIME_POS_S));
    graphics_draw_text(ctx, data->buff_s, data->font, data->rect_s, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

BigTimeLayer *big_time_layer_create(GRect frame)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "BigTimeLayer:big_time_layer_create");

    BigTimeLayer *big_time_layer = layer_create_with_data(frame, sizeof(BigTimeLayerData));
    layer_set_update_proc(big_time_layer, big_time_layer_update_proc);
    layer_mark_dirty(big_time_layer);

    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    data->font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_EUROSTYLE_BIT_42));
    data->baseline_offset = 0;
    data->highlight_pos = BIG_TIME_POS_NONE;
    big_time_layer_set_colors(big_time_layer, GColorBlack, GColorRed, GColorLightGray);
    big_time_layer_zero(big_time_layer);
    big_time_layer_update_geometry(big_time_layer);

    layer_mark_dirty(big_time_layer);

    return big_time_layer;
}

void big_time_layer_destroy(BigTimeLayer *big_time_layer)
{
    if (big_time_layer)
    {
        layer_destroy(big_time_layer);
    }
}

void big_time_layer_set_font(BigTimeLayer *big_time_layer, GFont font, int baseline)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    data->font = font;
    data->baseline_offset = baseline;
    big_time_layer_update_geometry(big_time_layer);

    layer_mark_dirty(big_time_layer);
}
void big_time_layer_set_time(BigTimeLayer *big_time_layer, time_t t)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);

    data->val_h = t / SECONDS_PER_HOUR;
    data->val_m = (t % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
    data->val_s = (t % SECONDS_PER_MINUTE);

    snprintf(data->buff_h, sizeof(data->buff_h), "%02d", data->val_h);
    snprintf(data->buff_m, sizeof(data->buff_m), "%02d", data->val_m);
    snprintf(data->buff_s, sizeof(data->buff_s), "%02d", data->val_s);

    layer_mark_dirty(big_time_layer);
}
void big_time_layer_zero(BigTimeLayer *big_time_layer)
{
    big_time_layer_set_time(big_time_layer, 0);
}
void big_time_layer_set_colors(BigTimeLayer *big_time_layer, GColor color_text, GColor color_highlight, GColor color_inactive)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    data->color_text = color_text;
    data->color_highlight = color_highlight;
    data->color_inactive = color_inactive;
    layer_mark_dirty(big_time_layer);
}
void big_time_layer_highlight(BigTimeLayer *big_time_layer, BigTimeLayerPosition pos)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    data->highlight_pos = pos;
    layer_mark_dirty(big_time_layer);
}
void big_time_layer_clear_highlight(BigTimeLayer *big_time_layer)
{
    BigTimeLayerData *data = (BigTimeLayerData *)layer_get_data(big_time_layer);
    data->highlight_pos = BIG_TIME_POS_NONE;
}