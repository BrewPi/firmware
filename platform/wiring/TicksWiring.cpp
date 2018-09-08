#include "TicksWiring.h"
#include "Ticks.h"
#include "application.h"

ticks_seconds_t TicksWiring::seconds() { return ::millis()/1000; }
ticks_millis_t TicksWiring::millis() { return ::millis(); }
ticks_micros_t TicksWiring::micros() { return ::micros(); }


void DelayWiring::millis(uint16_t millis) { ::delay(millis); }
void DelayWiring::microseconds(uint32_t micros) { ::delayMicroseconds(micros); }
void DelayWiring::seconds(uint16_t seconds) { millis(seconds*1000); }

