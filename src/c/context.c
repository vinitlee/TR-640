#include "context.h"

void context_init(AppContext *ctx)
{
    ctx->state = TR_STATE_RESET;
    ctx->auto_mode = true;
    ctx->light_on = false;
    ctx->beeping = false;
    ctx->time_set = 5 * SECONDS_PER_MINUTE;
    ctx->time_at_start = 0;
    ctx->time_left = 0;
    ctx->realtime_started = time(NULL);
}