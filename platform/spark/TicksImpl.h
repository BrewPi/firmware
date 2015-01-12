#pragma once

#include "TicksWiring.h"

typedef HardwareTicks TicksImpl;
typedef HardwareDelay DelayImpl;
#define TICKS_IMPL_CONFIG
#define DELAY_IMPL_CONFIG

extern TicksImpl ticks;
extern DelayImpl wait;