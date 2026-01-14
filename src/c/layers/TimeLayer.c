#include "TimeLayer.h"

static void time_layer_highlight_match(TimeLayerData *data, GContext *ctx, TimeLayerPosition A, TimeLayerPosition B)
{
    if (A == B)
    {
        graphics_context_set_text_color(ctx, data->highlight_color);
    }
    else
    {
        graphics_context_set_text_color(ctx, data->foreground_color);
    }
}

static void time_layer_update_geometry(TimeLayer *time_layer)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    GRect bounds = layer_get_bounds(time_layer);

    GSize layout_size = graphics_text_layout_get_content_size("00", data->font, bounds, GTextOverflowModeFill, GTextAlignmentCenter);
    GSize text_size = {.w = bounds.size.w, .h = layout_size.h};
    GPoint bounds_center = grect_center_point(&bounds);

    GRect mins_rect = GRect(0, bounds_center.y - text_size.h / 2 + data->baseline, text_size.w, text_size.h);
    GRect hrs_rect = GRect(mins_rect.origin.x, mins_rect.origin.y - text_size.h, mins_rect.size.w, mins_rect.size.h);
    GRect secs_rect = GRect(mins_rect.origin.x, mins_rect.origin.y + text_size.h, mins_rect.size.w, mins_rect.size.h);

    data->h_rect = hrs_rect;
    data->m_rect = mins_rect;
    data->s_rect = secs_rect;
}

static void time_layer_update_proc(TimeLayer *time_layer, GContext *ctx)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "TimeLayer:time_layer_update_proc");

    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    GRect bounds = layer_get_bounds(time_layer);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "TimeLayer:time_layer_update_proc:data->highlight=%d", data->highlight);

    time_layer_highlight_match(data, ctx, TIME_LAYER_HRS, data->highlight);
    graphics_draw_text(ctx, data->h_buff, data->font, data->h_rect, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
    time_layer_highlight_match(data, ctx, TIME_LAYER_MINS, data->highlight);
    graphics_draw_text(ctx, data->m_buff, data->font, data->m_rect, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
    time_layer_highlight_match(data, ctx, TIME_LAYER_SECS, data->highlight);
    graphics_draw_text(ctx, data->s_buff, data->font, data->s_rect, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

TimeLayer *time_layer_create(GRect frame)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "TimeLayer:time_layer_create");

    TimeLayer *time_layer = layer_create_with_data(frame, sizeof(TimeLayerData));
    layer_set_update_proc(time_layer, time_layer_update_proc);
    layer_mark_dirty(time_layer);

    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    data->font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_EUROSTYLE_BIT_42));
    data->foreground_color = GColorBlack;
    data->background_color = GColorWhite;
    data->highlight_color = GColorRed;
    time_layer_zero(time_layer);
    time_layer_update_geometry(time_layer);

    layer_mark_dirty(time_layer);

    return time_layer;
}

void time_layer_destroy(TimeLayer *time_layer)
{
    if (time_layer)
    {
        layer_destroy(time_layer);
    }
}

void time_layer_set_font(TimeLayer *time_layer, GFont font, int baseline)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    data->font = font;
    data->baseline = baseline;
    time_layer_update_geometry(time_layer);

    layer_mark_dirty(time_layer);
}

void time_layer_set_hrs(TimeLayer *time_layer, int val)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    int hrs = val % 24;
    if (hrs < 0)
        hrs = 0;
    snprintf(data->h_buff, sizeof(data->h_buff), "%02d", hrs);

    layer_mark_dirty(time_layer);
}
void time_layer_set_mins(TimeLayer *time_layer, int val)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);

    int mins = val % 60;
    if (mins < 0)
        mins = 0;
    snprintf(data->m_buff, sizeof(data->m_buff), "%02d", mins);

    if (val >= 60)
        time_layer_set_hrs(time_layer, val / 60);

    layer_mark_dirty(time_layer);
}
void time_layer_set_secs(TimeLayer *time_layer, int val)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);

    int secs = val % 60;
    if (secs < 0)
        secs = 0;
    snprintf(data->s_buff, sizeof(data->s_buff), "%02d", secs);

    if (val >= 60)
        time_layer_set_mins(time_layer, val / 60);

    layer_mark_dirty(time_layer);
}
void time_layer_set_colors(TimeLayer *time_layer, GColor foreground, GColor background, GColor highlight)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    data->foreground_color = foreground;
    data->background_color = background;
    data->highlight_color = highlight;

    layer_mark_dirty(time_layer);
}
void time_layer_zero(TimeLayer *time_layer)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);

    snprintf(data->h_buff, sizeof(data->h_buff), "%02d", 0);
    snprintf(data->m_buff, sizeof(data->m_buff), "%02d", 0);
    snprintf(data->s_buff, sizeof(data->s_buff), "%02d", 0);

    layer_mark_dirty(time_layer);
}

void time_layer_highlight(TimeLayer *time_layer, TimeLayerPosition pos)
{
    TimeLayerData *data = (TimeLayerData *)layer_get_data(time_layer);
    data->highlight = pos;
    layer_mark_dirty(time_layer);
}