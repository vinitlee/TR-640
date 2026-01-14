#include <pebble.h>
#include "../layers/BigTimeLayer.h"
#include "../layers/ModeSwitchLayer.h"

typedef struct
{
    time_t displayed_time;
    ModeSwitchMode mode;
    BigTimeLayerPosition focus_position;

} ViewModel;