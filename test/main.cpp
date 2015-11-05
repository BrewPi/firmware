#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Platform.h"
#include "ConfigDefault.h"
#include "Ticks.h"

TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);
//DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);


// delay ms miliseconds and return current time afterwards

ticks_millis_t delay(int ms) {
    ticks_millis_t newTime;
    if(ms == 0){
        newTime = ticks.millis(); // ensure that millis() is read at least once
    }
    for (int i = 0; i < ms; i++) {
        newTime = ticks.millis();
    }
    return newTime;
}

// delay random nr of milliseconds with maximum max

ticks_millis_t random_delay(int max) {
    int ms = rand() % (max - 1);
    return delay(ms);
}
