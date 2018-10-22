#include "TicksWiring.h"
#include "Ticks.h"
#include "application.h"

ticks_seconds_t
TicksWiring::seconds()
{
    return ::millis() / 1000;
}
ticks_millis_t
TicksWiring::millis()
{
    return ::millis();
}
ticks_micros_t
TicksWiring::micros()
{
    return ::micros();
}

void
TicksWiring::delayMillis(const duration_millis_t& duration)
{
    ::delay(duration);
}
