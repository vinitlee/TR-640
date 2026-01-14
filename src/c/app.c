#include <pebble.h>
#include "persist.h"
#include "context.h"

#include "models/ViewModel.h"
#include "windows/TimerWindow.h"

static AppContext s_ctx;

static void init()
{
    // Load default values into AppContext
    context_init(&s_ctx);
    // Load any persisted state into AppContext
    persist_load_state(&s_ctx);
    // (CHANGE) Push TimerWindow to stack
    timer_window_push(&s_ctx);
}
static void deinit()
{
    // Save persisting state on exit
    persist_save_state(&s_ctx);
}

int main(void)
{
    init();
    app_event_loop();
    deinit();
}