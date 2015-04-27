#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Ticks.h"

TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
//DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);
