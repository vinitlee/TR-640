#pragma once
#include "control.h"
#include "context.h"

#define PERSIST_VERSION_NUM 1

enum
{
    PKEY_VERSION = 1,
    PKEY_APP_STATE = 2
};

typedef struct
{
    TRState state;
    int total_hr;
    int total_min;
    int total_sec;
    int current_time;
    bool auto_mode;
} PersistedState;

void persist_save_state(AppContext *s_ctx);
void persist_load_state(AppContext *s_ctx);
