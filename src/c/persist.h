#pragma once

#include "control.h"

enum {
    PKEY_VERSION = 1,
    PKEY_APP_STATE = 2
};

typedef struct {
    TRState state;
    int total_hr;
    int total_min;
    int total_sec;
    int current_time;
    bool auto_mode;
} PersistedState;