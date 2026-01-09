#include <pebble.h>
#include "control.h"
#include "layers/TimeLayer.h"

static Window *s_window;
// static TextLayer *s_time_text_layer;
static TextLayer *s_auto_text_layer;
static AppContext s_ctx;
static AppTimer *s_alarm_timer;
static TimeLayer *s_time_layer;

static GColor s_background_color;
static GColor s_backlight_color;

static const uint32_t const segments[] = {50, 50, 80};
VibePattern vibe_alarm = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
};

static char s_time_buffer[16];

static void sm_dispatch(TREvent event);

static void dpt_countdown_loop(void *context);
static void dpt_reset_timer(void);
static void dpt_adjust_hr(int delta);
static void dpt_adjust_min(int delta);
static void dpt_adjust_sec(int delta);
static void dpt_light_off(void *context);
static void dpt_light_on(void);
static void dpt_sound_alarm(int duration);
static void dpt_stop_alarm(void *context);

static void sm_dispatch(TREvent event);
static void sm_dispatch(TREvent event);
static void sm_on_enter(TRState state);
static void sm_on_exit(TRState state);
static void sm_change_state(TRState next);
static void sm_handle_button(TRClick click);
static void sm_button_start(ClickRecognizerRef recognizer, void *context);
static void sm_button_light(ClickRecognizerRef recognizer, void *context);
static void sm_button_adjust(ClickRecognizerRef recognizer, void *context);
static void sm_button_mode(ClickRecognizerRef recognizer, void *context);
static void sm_init_context();

static void prv_click_config_provider(void *context);

static void ui_update_display(void)
{
  int hrs;
  int mins;
  int secs;
  if (s_ctx.state == TR_STATE_RUNNING || s_ctx.state == TR_STATE_PAUSED)
  {
    hrs = s_ctx.current_time / 3600;
    mins = (s_ctx.current_time % 3600) / 60;
    secs = (s_ctx.current_time % 60);
  }
  else
  {
    hrs = s_ctx.total_hr;
    mins = s_ctx.total_min;
    secs = s_ctx.total_sec;
  }
  // snprintf(s_time_buffer, sizeof(s_time_buffer), "%02d:%02d:%02d", hrs, mins, secs);
  // text_layer_set_text(s_time_text_layer, s_time_buffer);
  time_layer_set_hrs(s_time_layer, hrs);
  time_layer_set_mins(s_time_layer, mins);
  time_layer_set_secs(s_time_layer, secs);

  if (s_ctx.auto_mode)
  {
    text_layer_set_text_color(s_auto_text_layer, GColorRed);
  }
  else
  {
    text_layer_set_text_color(s_auto_text_layer, GColorLightGray);
  }
}

static void dpt_countdown_loop(void *context)
{
  if (s_ctx.state == TR_STATE_RUNNING)
  {
    s_ctx.current_time -= 1;
    APP_LOG(APP_LOG_LEVEL_INFO, "T %d", s_ctx.current_time);
    ui_update_display();
    if (s_ctx.current_time == 0)
    {
      sm_dispatch(TR_EVENT_START_ALARM);
      sm_dispatch(TR_EVENT_RESET);
      if (!s_ctx.auto_mode)
        sm_change_state(TR_STATE_RESET);
    }
    s_ctx.timer = app_timer_register(1000, dpt_countdown_loop, NULL);
  }
}

static void dpt_reset_timer()
{
  // s_ctx.current_time = 1000;
  s_ctx.current_time = 60 * 60 * s_ctx.total_hr + 60 * s_ctx.total_min + s_ctx.total_sec;
  APP_LOG(APP_LOG_LEVEL_INFO, "Ri %d:%d:%d", s_ctx.total_hr, s_ctx.total_min, s_ctx.total_sec);
  APP_LOG(APP_LOG_LEVEL_INFO, "R %d", s_ctx.current_time);
}

static void dpt_adjust_hr(int delta)
{
  s_ctx.total_hr += delta;
  if (s_ctx.total_hr < 0)
  {
    s_ctx.total_hr = 0;
  }
  s_ctx.total_hr = s_ctx.total_hr % 24;
}

static void dpt_adjust_min(int delta)
{
  s_ctx.total_min += delta;
  if (s_ctx.total_min < 0)
  {
    s_ctx.total_min = 0;
  }
  s_ctx.total_min = s_ctx.total_min % 60;
}

static void dpt_adjust_sec(int delta)
{
  s_ctx.total_sec += delta;
  if (s_ctx.total_sec < 0)
  {
    s_ctx.total_sec = 0;
  }
  s_ctx.total_sec = s_ctx.total_sec % 60;
}

static void dpt_light_on()
{
  light_enable(true);
  window_set_background_color(s_window, s_backlight_color);
  app_timer_register(2000, dpt_light_off, NULL);
}

static void dpt_light_off(void *context)
{
  light_enable(false);
  window_set_background_color(s_window, s_background_color);
}

static void alarm_tick(void *context)
{
  if (!s_ctx.beeping)
    return;
  // vibes_enqueue_custom_pattern(vibe_alarm);
  vibes_double_pulse();

  s_alarm_timer = app_timer_register(300, alarm_tick, NULL);
}

static void dpt_sound_alarm(int duration)
{
  s_ctx.beeping = true;
  app_timer_register(duration, dpt_stop_alarm, NULL);
  alarm_tick(NULL);
}

static void dpt_stop_alarm(void *context)
{
  s_ctx.beeping = false;
  if (s_alarm_timer)
  {
    app_timer_cancel(s_alarm_timer);
    s_alarm_timer = NULL;
  }
  vibes_cancel();
}

static void sm_dispatch(TREvent event)
{
  APP_LOG(APP_LOG_LEVEL_INFO, ">> %s", event_str(event));

  switch (event)
  {
  case TR_EVENT_RESET:
    dpt_reset_timer();
    break;
  case TR_EVENT_RUN:
    dpt_countdown_loop(NULL);
    break;
  case TR_EVENT_PAUSE:
    break;
  case TR_EVENT_INC_HR:
    dpt_adjust_hr(1);
    ui_update_display();
    break;
  case TR_EVENT_INC_MIN:
    dpt_adjust_min(1);
    ui_update_display();
    break;
  case TR_EVENT_INC_SEC:
    dpt_adjust_sec(1);
    ui_update_display();
    break;
  case TR_EVENT_DEC_HR:
    dpt_adjust_hr(-1);
    ui_update_display();
    break;
  case TR_EVENT_DEC_MIN:
    dpt_adjust_min(-1);
    ui_update_display();
    break;
  case TR_EVENT_DEC_SEC:
    dpt_adjust_sec(-1);
    ui_update_display();
    break;
  case TR_EVENT_TOGGLE_AUTO:
    s_ctx.auto_mode = !s_ctx.auto_mode;
    ui_update_display();
    break;
  case TR_EVENT_LIGHT_ON:
    dpt_light_on();
    break;
  case TR_EVENT_START_ALARM:
    dpt_sound_alarm(5000);
    break;
  case TR_EVENT_STOP_ALARM:
    dpt_stop_alarm(NULL);
    break;
  default:
    break;
  }
}

static void sm_on_enter(TRState state)
{
  switch (state)
  {
  case TR_STATE_RESET:
    ui_update_display();
    break;
  case TR_STATE_RUNNING:
    dpt_countdown_loop(NULL);
    break;
  case TR_STATE_PAUSED:
    break;
  case TR_STATE_EDIT_HR:
    time_layer_highlight(s_time_layer, TIME_LAYER_HRS);
    break;
  case TR_STATE_EDIT_MIN:
    time_layer_highlight(s_time_layer, TIME_LAYER_MINS);
    break;
  case TR_STATE_EDIT_SEC:
    time_layer_highlight(s_time_layer, TIME_LAYER_SECS);
    break;
  default:
    break;
  }
}

static void sm_on_exit(TRState state)
{
  switch (state)
  {
  case TR_STATE_RESET:
    sm_dispatch(TR_EVENT_RESET);
    break;
  case TR_STATE_RUNNING:
    break;
  case TR_STATE_PAUSED:
    break;
  case TR_STATE_EDIT_HR:
    time_layer_highlight(s_time_layer, TIME_LAYER_NONE);
    break;
  case TR_STATE_EDIT_MIN:
    time_layer_highlight(s_time_layer, TIME_LAYER_NONE);
    break;
  case TR_STATE_EDIT_SEC:
    time_layer_highlight(s_time_layer, TIME_LAYER_NONE);
    break;
  default:
    break;
  }
}

static TRState sm_on_start(TRState s)
{
  switch (s)
  {
  case TR_STATE_RESET:
    return TR_STATE_RUNNING;
  case TR_STATE_RUNNING:
    return TR_STATE_PAUSED;
  case TR_STATE_PAUSED:
    return TR_STATE_RUNNING;
  case TR_STATE_EDIT_HR:
    sm_dispatch(TR_EVENT_INC_HR);
    return s;
  case TR_STATE_EDIT_MIN:
    sm_dispatch(TR_EVENT_INC_MIN);
    return s;
  case TR_STATE_EDIT_SEC:
    sm_dispatch(TR_EVENT_INC_SEC);
    return s;
  default:
    return s;
  }
}

static TRState sm_on_light(TRState s)
{
  switch (s)
  {
  case TR_STATE_RESET:
  case TR_STATE_RUNNING:
  case TR_STATE_PAUSED:
    sm_dispatch(TR_EVENT_LIGHT_ON);
    return s;
  case TR_STATE_EDIT_HR:
  case TR_STATE_EDIT_MIN:
  case TR_STATE_EDIT_SEC:
    sm_dispatch(TR_EVENT_TOGGLE_AUTO);
    return s;
  default:
    return s;
  }
}

static TRState sm_on_adjust(TRState s)
{
  switch (s)
  {
  case TR_STATE_RESET:
    return TR_STATE_EDIT_HR;
  case TR_STATE_RUNNING:
    return TR_STATE_RESET;
  case TR_STATE_PAUSED:
    return TR_STATE_RESET;
  case TR_STATE_EDIT_HR:
    return TR_STATE_RESET;
  case TR_STATE_EDIT_MIN:
    return TR_STATE_RESET;
  case TR_STATE_EDIT_SEC:
    return TR_STATE_RESET;
  default:
    return s;
  }
}

static TRState sm_on_mode(TRState s)
{
  switch (s)
  {
  case TR_STATE_RESET:
  case TR_STATE_RUNNING:
  case TR_STATE_PAUSED:
    return s;
  case TR_STATE_EDIT_HR:
    return TR_STATE_EDIT_MIN;
  case TR_STATE_EDIT_MIN:
    return TR_STATE_EDIT_SEC;
  case TR_STATE_EDIT_SEC:
    return TR_STATE_EDIT_HR;
  default:
    return s;
  }
}

static void sm_change_state(TRState next)
{
  TRState prev = s_ctx.state;
  if (prev == next)
    return;
  sm_on_exit(prev);
  s_ctx.state = next;
  sm_on_enter(next);

  window_set_click_config_provider(s_window, prv_click_config_provider);

  APP_LOG(APP_LOG_LEVEL_INFO, "%s -> %s", state_str(prev), state_str(next));
}

static void sm_handle_button(TRClick click)
{
  if (s_ctx.beeping)
  {
    sm_dispatch(TR_EVENT_STOP_ALARM);
    return;
  }

  TRState next;

  switch (click)
  {
  case TR_CLICK_START:
    next = sm_on_start(s_ctx.state);
    break;
  case TR_CLICK_LIGHT:
    next = sm_on_light(s_ctx.state);
    break;
  case TR_CLICK_ADJUST:
    next = sm_on_adjust(s_ctx.state);
    break;
  case TR_CLICK_MODE:
    next = sm_on_mode(s_ctx.state);
    break;
  default:
    break;
  }

  sm_change_state(next);
}

static void sm_button_start(ClickRecognizerRef recognizer, void *context)
{
  sm_handle_button(TR_CLICK_START);
}

static void sm_button_light(ClickRecognizerRef recognizer, void *context)
{
  sm_handle_button(TR_CLICK_LIGHT);
}

static void sm_button_adjust(ClickRecognizerRef recognizer, void *context)
{
  sm_handle_button(TR_CLICK_ADJUST);
}

static void sm_button_mode(ClickRecognizerRef recognizer, void *context)
{
  sm_handle_button(TR_CLICK_MODE);
}

static bool sm_state_allows_repeat(TRState state)
{
  return s_ctx.state == TR_STATE_EDIT_HR ||
         s_ctx.state == TR_STATE_EDIT_MIN ||
         s_ctx.state == TR_STATE_EDIT_SEC;
}

static void prv_click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_SELECT, sm_button_mode);
  window_single_click_subscribe(BUTTON_ID_UP, sm_button_light);
  window_single_click_subscribe(BUTTON_ID_BACK, sm_button_adjust);

  if (sm_state_allows_repeat(s_ctx.state))
  {
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 150, sm_button_start);
  }
  else
  {
    window_single_click_subscribe(BUTTON_ID_DOWN, sm_button_start);
  }
}

static void sm_init_context()
{
  s_ctx.state = TR_STATE_RESET;
  s_ctx.auto_mode = true;
  s_ctx.light_on = false;
  s_ctx.beeping = false;
  s_ctx.total_hr = 0;
  s_ctx.total_min = 5;
  s_ctx.total_sec = 0;
  s_ctx.current_time = 0;
}

static void init_colors()
{
  s_background_color = GColorWhite;
  s_backlight_color = GColorOrange;
}

static void prv_window_load(Window *window)
{
  sm_init_context();
  init_colors();

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // s_time_text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 42));
  // text_layer_set_text_alignment(s_time_text_layer, GTextAlignmentCenter);
  // text_layer_set_font(s_time_text_layer, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
  // text_layer_set_background_color(s_time_text_layer, GColorClear);
  // layer_add_child(window_layer, text_layer_get_layer(s_time_text_layer));

  s_auto_text_layer = text_layer_create(GRect(8, 8, bounds.size.w, 20));
  text_layer_set_text(s_auto_text_layer, "AUTO");
  text_layer_set_text_alignment(s_auto_text_layer, GTextAlignmentLeft);
  text_layer_set_background_color(s_auto_text_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_auto_text_layer));

  s_time_layer = time_layer_create(grect_crop(bounds, 8));
  layer_add_child(window_layer, s_time_layer);

  ui_update_display();
}

static void prv_window_unload(Window *window)
{
  // text_layer_destroy(s_time_text_layer);
  text_layer_destroy(s_auto_text_layer);
  time_layer_destroy(s_time_layer);
}

static void prv_init(void)
{
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers){
                                           .load = prv_window_load,
                                           .unload = prv_window_unload,
                                       });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

static void prv_deinit(void)
{
  window_destroy(s_window);
}

int main(void)
{
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}
