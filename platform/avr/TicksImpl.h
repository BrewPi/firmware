/*
 * TicksImpl.h
 *
 * Created: 07/01/2015 05:04:13
 *  Author: mat
 */ 


#ifndef TICKSIMPL_H_
#define TICKSIMPL_H_

typedef uint16_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint16_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;

#include "TicksWiring.h"

// Determine the type of Ticks needed
// TICKS_IMPL_CONFIG is the code string passed to the constructor of the Ticks implementation

#if BREWPI_SIMULATE				
/** For simulation, by the simulator - each step in the simulator advances the time by one second. */    
	typedef ExternalTicks TicksImpl;
	#define TICKS_IMPL_CONFIG		// no configuration of ExternalTicks necessary

#elif BREWPI_EMULATE
/** When debugging in AVR studio (and running normal brewpi - not the simulator), use a simple MockTicks that increments 100
	millis each time it's called. */	
	typedef MockTicks TicksImpl;
	#define TICKS_IMPL_CONFIG 100	
	
#else // use regular hardware timer/delay
	typedef HardwareTicks TicksImpl;
	#define TICKS_IMPL_CONFIG
#endif	// BREWPI_EMULATE

extern TicksImpl TICKS;

// Determine the type of delay required.
// For emulation, don't delay, since time in the emulator is not real time, so the delay is meaningless.
// For regular code, use the arduino delay function.

#if BREWPI_EMULATE || !defined(ARDUINO)
typedef NoOpDelay DelayImpl;		// for emulation (avr debugger), don't bother delaying, it takes ages.
#define DELAY_IMPL_CONFIG
#else
typedef HardwareDelay DelayImpl;
#define DELAY_IMPL_CONFIG
#endif

extern DelayImpl wait;




#endif /* TICKSIMPL_H_ */