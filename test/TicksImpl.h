#pragma once
#include <stdint.h>

// #include "TicksWiring.h"

// Determine the type of Ticks needed
// TICKS_IMPL_CONFIG is the code string passed to the constructor of the Ticks implementation

#if BREWPI_SIMULATE				
/** For simulation, by the simulator - each step in the simulator advances the time by one second. */
typedef ExternalTicks TicksImpl;
#define TICKS_IMPL_CONFIG		// no configuration of ExternalTicks necessary

#elif BREWPI_EMULATE
typedef MockTicks TicksImpl;
#define TICKS_IMPL_CONFIG 1
#endif	// BREWPI_EMULATE

extern TicksImpl ticks;

#if BREWPI_EMULATE
typedef NoOpDelay DelayImpl;
#define DELAY_IMPL_CONFIG 1
#else
typedef HardwareDelay DelayImpl;
#define DELAY_IMPL_CONFIG
#endif

extern DelayImpl wait;
