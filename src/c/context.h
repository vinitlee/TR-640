#pragma once

#include <pebble.h>
#include "control.h"

typedef struct
{
    TRState state;
    bool auto_mode;
    bool light_on;
    bool beeping;
    int time_set;
    int time_at_start;
    int time_left;
    time_t realtime_started;
    // AppTimer *timer;
} AppContext;

void context_init(AppContext *ctx);