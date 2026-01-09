#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

typedef enum
{
  TR_STATE_RESET = 0,
  TR_STATE_RUNNING,
  TR_STATE_PAUSED,
  TR_STATE_EDIT_HR,
  TR_STATE_EDIT_MIN,
  TR_STATE_EDIT_SEC,
} TRState;

typedef enum
{
  TR_EVENT_RESET = 0,
  TR_EVENT_RUN,
  TR_EVENT_PAUSE,
  TR_EVENT_INC_HR,
  TR_EVENT_INC_MIN,
  TR_EVENT_INC_SEC,
  TR_EVENT_DEC_HR,
  TR_EVENT_DEC_MIN,
  TR_EVENT_DEC_SEC,
  TR_EVENT_TOGGLE_AUTO,
  TR_EVENT_LIGHT_ON,
  TR_EVENT_START_ALARM,
  TR_EVENT_STOP_ALARM,
} TREvent;

typedef enum
{
  TR_CLICK_START = 0,
  TR_CLICK_LIGHT,
  TR_CLICK_ADJUST,
  TR_CLICK_MODE,
} TRClick;

typedef struct
{
  TRState state;
  bool auto_mode;
  bool light_on;
  bool beeping;
} AppContext;

static AppContext s_ctx;

static void sm_change_state(TRState next)
{
  TRState prev = s_ctx.state;
  if (prev == next)
    return;
  sm_on_exit(prev);
  s_ctx.state = next;
  sm_on_enter(next);

  APP_LOG(APP_LOG_LEVEL_INFO, "%s -> %s", state_str(prev), state_str(next));
}

static const char *state_str(TRState state)
{
  switch (state)
  {
  case TR_STATE_RESET:
    return "Reset";
  case TR_STATE_RUNNING:
    return "Running";
  case TR_STATE_PAUSED:
    return "Paused";
  case TR_STATE_EDIT_HR:
    return "EditHrs";
  case TR_STATE_EDIT_MIN:
    return "EditMins";
  case TR_STATE_EDIT_SEC:
    return "EditSecs";
  default:
    return "Unknown";
  }
}

static void sm_dispatch(TREvent event)
{
  switch (event)
  {
  case TR_EVENT_RESET:
    break;
  case TR_EVENT_RUN:
    break;
  case TR_EVENT_PAUSE:
    break;
  case TR_EVENT_INC_HR:
    break;
  case TR_EVENT_INC_MIN:
    break;
  case TR_EVENT_INC_SEC:
    break;
  case TR_EVENT_DEC_HR:
    break;
  case TR_EVENT_DEC_MIN:
    break;
  case TR_EVENT_DEC_SEC:
    break;
  case TR_EVENT_TOGGLE_AUTO:
    break;
  case TR_EVENT_LIGHT_ON:
    break;
  case TR_EVENT_START_ALARM:
    break;
  case TR_EVENT_STOP_ALARM:
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
    break;
  case TR_STATE_RUNNING:
    break;
  case TR_STATE_PAUSED:
    break;
  case TR_STATE_EDIT_HR:
    break;
  case TR_STATE_EDIT_MIN:
    break;
  case TR_STATE_EDIT_SEC:
    break;
  default:
    break;
  }
}

static void sm_on_exit(state)
{
  switch (state)
  {
  case TR_STATE_RESET:
    break;
  case TR_STATE_RUNNING:
    break;
  case TR_STATE_PAUSED:
    break;
  case TR_STATE_EDIT_HR:
    break;
  case TR_STATE_EDIT_MIN:
    break;
  case TR_STATE_EDIT_SEC:
    break;
  default:
    break;
  }
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

  sm_change_state(next)
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
    break;
  }
}

static TRState sm_on_adjust(TRState s)
{
  switch (s)
  {
  case TR_STATE_RESET:
    sm_change_state(TR_STATE_EDIT_HR);
    break;
  case TR_STATE_RUNNING:
    sm_change_state(TR_STATE_RESET);
    break;
  case TR_STATE_PAUSED:
    sm_change_state(TR_STATE_RESET);
    break;
  case TR_STATE_EDIT_HR:
    sm_change_state(TR_STATE_RESET);
    break;
  case TR_STATE_EDIT_MIN:
    sm_change_state(TR_STATE_RESET);
    break;
  case TR_STATE_EDIT_SEC:
    sm_change_state(TR_STATE_RESET);
    break;
  default:
    break;
  }
}

static TRState sm_on_mode(TRState s)
{
  switch (s)
  {
  case TR_STATE_RESET:
    break;
  case TR_STATE_RUNNING:
    break;
  case TR_STATE_PAUSED:
    break;
  case TR_STATE_EDIT_HR:
    break;
  case TR_STATE_EDIT_MIN:
    break;
  case TR_STATE_EDIT_SEC:
    break;
  default:
    break;
  }
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

static void prv_window_load(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  text_layer_set_text(s_text_layer, "Press a button");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void prv_window_unload(Window *window)
{
  text_layer_destroy(s_text_layer);
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
