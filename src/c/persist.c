#include "persist.h"

void persist_save_state(AppContext *s_ctx)
{
    PersistedState ps = {
        .total_hr = s_ctx->total_hr,
        .total_min = s_ctx->total_min,
        .total_sec = s_ctx->total_sec,
        .current_time = s_ctx->current_time,
        .auto_mode = s_ctx->auto_mode,
        .state = s_ctx->state,
    };
    persist_write_int(PKEY_VERSION, PERSIST_VERSION_NUM);
    persist_write_data(PKEY_APP_STATE, &ps, sizeof(ps));
}

void persist_load_state(AppContext *s_ctx)
{
    if (!persist_exists(PKEY_APP_STATE))
    {
        return;
    }

    int ver = persist_read_int(PKEY_VERSION);
    if (ver != PERSIST_VERSION_NUM)
    {
        return;
    }

    PersistedState ps;
    int read_ps_size = persist_read_data(PKEY_APP_STATE, &ps, sizeof(ps));
    if (read_ps_size == sizeof(ps))
    {
        s_ctx->total_hr = ps.total_hr;
        s_ctx->total_min = ps.total_min;
        s_ctx->total_sec = ps.total_sec;
        s_ctx->current_time = ps.current_time;
        s_ctx->auto_mode = ps.auto_mode;
        s_ctx->state = ps.state;
    }
}