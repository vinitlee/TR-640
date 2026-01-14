#include "TimerWindow.h"
#include "../context.h"

#include "../layers/BigTimeLayer.h"
#include "../layers/MiniClockLayer.h"
#include "../layers/ModeSwitchLayer.h"

#include "../res/colors.h"

static Window *s_window;

static BigTimeLayer *s_big_time_layer;
static ModeSwitchLayer *s_mode_indicator;
static MiniClockLayer *s_clock_layer;

static AppTimer *s_update_timer;
static int update_interval;

static AppContext *s_ctx;

static void timer_update_loop(void *context);
static void tick_timer_callback(struct tm *tick_time, TimeUnits units_changed);

// X---
static void next_update_timer()
{
    s_update_timer = app_timer_register(update_interval, timer_update_loop, NULL);
}
static void timer_update_loop(void *context)
{
    // Do stuff

    if (true)
    {
        next_update_timer();
    }
}
static void tick_timer_callback(struct tm *tick_time, TimeUnits units_changed)
{
    mini_clock_layer_update_with(s_clock_layer, tick_time);
}
// ----X

static void window_appear(Window *window)
{
    // Clock TickTimer
    tick_timer_service_subscribe(SECOND_UNIT, tick_timer_callback);
}

static void window_disappear(Window *window)
{
    tick_timer_service_unsubscribe();
    if (s_update_timer)
    {
        app_timer_cancel(s_update_timer);
    }
}

static void set_colors(GColor primary, GColor secondary, GColor accent, GColor inactive)
{
    big_time_layer_set_colors(s_big_time_layer, primary, accent, inactive);
    mini_clock_layer_set_text_color(s_clock_layer, primary);
    window_set_background_color(s_window, secondary);
    mode_switch_layer_set_colors(s_mode_indicator, accent, inactive);
}

static void window_load(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    s_big_time_layer = big_time_layer_create(bounds);

    big_time_layer_set_time(s_big_time_layer, 1300);
    big_time_layer_highlight(s_big_time_layer, BIG_TIME_POS_M);

    GRect mode_frame = GRect(0, 0, bounds.size.w, 16);
    grect_align(&mode_frame, &bounds, GAlignBottom, true);
    s_mode_indicator = mode_switch_layer_create(mode_frame);

    GRect clock_frame = GRect(0, 0, bounds.size.w, 20);
    grect_align(&clock_frame, &bounds, GAlignTop, true);
    s_clock_layer = mini_clock_layer_create(clock_frame);
    mini_clock_layer_set_font(s_clock_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
    mini_clock_layer_update(s_clock_layer);

    set_colors(COLOR_PRIMARY, COLOR_SECONDARY, COLOR_ACCENT, COLOR_INACTIVE);

    layer_add_child(window_layer, mini_clock_layer_get_layer(s_clock_layer));
    layer_add_child(window_layer, mode_switch_layer_get_layer(s_mode_indicator));
    layer_add_child(window_layer, s_big_time_layer);
}

static void window_unload(Window *window)
{
    big_time_layer_destroy(s_big_time_layer);
    mode_switch_layer_destroy(s_mode_indicator);
    mini_clock_layer_destroy(s_clock_layer);
}

void timer_window_push(AppContext *ctx)
{
    s_ctx = ctx;

    if (!s_window)
    {
        s_window = window_create();
        // window_set_background_color(s_window, PBL_IF_COLOR_ELSE(COLOR_SECONDARY, GColorBlack));
        window_set_window_handlers(s_window, (WindowHandlers){
                                                 .appear = window_appear,
                                                 .load = window_load,
                                                 .unload = window_unload,
                                                 .disappear = window_disappear});
    }
    window_stack_push(s_window, true);
}
