#include "catch.hpp"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time, to seed rand */

#include "ActuatorOnOff.h"
#include "Ticks.h"
#include <cstring>
// delay ms miliseconds and return current time afterwards

#include "main.h"

TEST_CASE("Test ActuatorOnOff class with ValueActuator as driver", "[actuatoronoff]") {
    srand(time(NULL));
    ticks.reset();
    Actuator * v = new ValueActuator();
    const uint16_t minOn = 100;
    const uint16_t maxOn = 200;
    const uint16_t minOff = 300;

    ActuatorOnOff * act = new ActuatorOnOff(v, minOn, maxOn, minOff);

    SECTION("Test that minimum off time and maximum on time are honored"){
        printf("\n\n**** Testing min OFF and max ON time for ActuatorOnOff ****\n\n");

        while(ticks.seconds() < 10000) {
            act->setActive(true);
            act->update(); // turns off the actuator when max on time has passed
            delay(1000);
            ticks_seconds_t time = ticks.seconds();
            printf("time: %10d \t", time);
            printf("seconds since toggle: %10d \n", act->timeSinceToggle());
            if(time <= minOff) {
                REQUIRE(act->isActive() == false); // actuator should still be off
            }
            else if(time <= (minOff + maxOn)) {
                REQUIRE(act->isActive() == true); // actuator should be on now
            }
            else {
                REQUIRE(act->isActive() == false); // actuator should be off again
                break;
            }
        }
    }

    SECTION("Test that minimum ON time is honored"){
        printf("\n\n**** Testing min ON time for ActuatorOnOff ****\n\n");
        do {
            delay(1000);
            act->setActive(true);
            ticks_seconds_t time = ticks.seconds();

            printf("time: %10d \t", time);
            printf("seconds since toggle: %10d \n", act->timeSinceToggle());
        }while(!act->isActive()); // wait for actuator to go active

        while (act->timeSinceToggle() < minOn) {
            act->setActive(false);
            ticks_seconds_t time = ticks.seconds();
            delay(1000);
            REQUIRE(act->isActive());
            printf("time: %10d \t", time);
            printf("seconds since toggle: %10d \n", act->timeSinceToggle());
        }
    }
}
