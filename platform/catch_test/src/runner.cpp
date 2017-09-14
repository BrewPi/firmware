#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "runner.h"
#include "Platform.h"
#include "Ticks.h"

ExternalTicks ticks;
NoOpDelay wait;

// delay ms milliseconds and return current time afterwards
ticks_millis_t delay(int ms) {
    ticks.incMillis(ms);
    return ticks.millis();
}

// delay random nr of milliseconds with maximum max

ticks_millis_t random_delay(int max) {
    int ms = rand() % (max - 1);
    return delay(ms);
}

std::ostream * output = &std::cout;
