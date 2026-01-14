#include "control.h"

// FSM
const char *state_str(TRState state)
{
    switch (state)
    {
    case TR_STATE_RESET:
        return "STATE_RESET";
    case TR_STATE_RUNNING:
        return "STATE_RUNNING";
    case TR_STATE_PAUSED:
        return "STATE_PAUSED";
    case TR_STATE_EDIT_HR:
        return "STATE_EDIT_HR";
    case TR_STATE_EDIT_MIN:
        return "STATE_EDIT_MIN";
    case TR_STATE_EDIT_SEC:
        return "STATE_EDIT_SEC";
    default:
        return "Unknown";
    }
}

const char *event_str(TREvent event)
{
    switch (event)
    {
    case TR_EVENT_RESET:
        return "EVENT_RESET";
    case TR_EVENT_RUN:
        return "EVENT_RUN";
    case TR_EVENT_PAUSE:
        return "EVENT_PAUSE";
    case TR_EVENT_INC_HR:
        return "EVENT_INC_HR";
    case TR_EVENT_INC_MIN:
        return "EVENT_INC_MIN";
    case TR_EVENT_INC_SEC:
        return "EVENT_INC_SEC";
    case TR_EVENT_DEC_HR:
        return "EVENT_DEC_HR";
    case TR_EVENT_DEC_MIN:
        return "EVENT_DEC_MIN";
    case TR_EVENT_DEC_SEC:
        return "EVENT_DEC_SEC";
    case TR_EVENT_TOGGLE_AUTO:
        return "EVENT_TOGGLE_AUTO";
    case TR_EVENT_LIGHT_ON:
        return "EVENT_LIGHT_ON";
    case TR_EVENT_START_ALARM:
        return "EVENT_START_ALARM";
    case TR_EVENT_STOP_ALARM:
        return "EVENT_STOP_ALARM";
    default:
        return "Unknown";
    }
}

const char *click_event_str(TREvent event)
{
    switch (event)
    {
    case TR_CLICK_START:
        return "TR_CLICK_START";
    case TR_CLICK_LIGHT:
        return "TR_CLICK_LIGHT";
    case TR_CLICK_ADJUST:
        return "TR_CLICK_ADJUST";
    case TR_CLICK_MODE:
        return "TR_CLICK_MODE";
    default:
        return "Unknown";
    }
}