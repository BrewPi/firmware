#include "catch.hpp"
#include "ActuatorPwm.h"
#include "Ticks.h"
#include <cstring>


TEST_CASE("Test class initalization", "[ActuatorPWM]") {

    Actuator * v = new ValueActuator();
    ActuatorPwm act(v, 0);
    // intToTemp is a macro to initialize temperatures in Celsius

    SECTION("PWM value is initialized to 0") {
        REQUIRE(act.getPwm() == 0);
    }
    
    SECTION("Test ticks.millis() to increment by one every call") {
        for(int i = 0; i<10; i++){
            printf("time: %d\n", ticks.millis());
        }
    }
}
