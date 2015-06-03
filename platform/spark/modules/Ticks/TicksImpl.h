#pragma once

#include "Platform.h"
#include "TicksWiring.h"

typedef HardwareTicks TicksImpl;
typedef HardwareDelay DelayImpl;
#define TICKS_IMPL_CONFIG
#define DELAY_IMPL_CONFIG

#include "TicksDeclaration.h"

extern TicksImpl TICKS;
extern DelayImpl wait;

