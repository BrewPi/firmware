#pragma once

#include "Platform.h"
#include "TicksWiring.h"

// we want to use the ticks on the device as the physical time
typedef HardwareTicks TicksImpl;
typedef HardwareDelay DelayImpl;
#define TICKS_IMPL_CONFIG
#define DELAY_IMPL_CONFIG

#include "TicksDeclaration.h"

// the application may name the actual ticks something else (e.g. baseticks.)
extern TicksImpl TICKS;
extern DelayImpl wait;

// allow the application to inject other definitions
#ifdef TICKS_POST_INCLUDE
#include TICKS_POST_INCLUDE
#endif
