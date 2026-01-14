#pragma once

#include <pebble.h>

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
    TR_CLICK_DUMMY,
} TRClick;

typedef struct
{
    TRState state;
    bool auto_mode;
    bool light_on;
    bool beeping;
    int total_hr;
    int total_min;
    int total_sec;
    int current_time;
    AppTimer *timer;
} AppContext;

const char *state_str(TRState state);
const char *event_str(TREvent event);