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

    ActuatorOnOff * act = new ActuatorOnOff(v, minOn, minOff, maxOn);

    SECTION("Test that minimum off time and maximum on time are honored"){
        printf("\n\n**** Testing min OFF and max ON time for ActuatorOnOff ****\n\n");

        while(ticks.seconds() < 10000) {
            bool stateBeforeUpdate = act->isActive();
            ticks_millis_t time = delay(100);

            act->setActive(true);
            act->update(); // turns off the actuator when max on time has passed

            if(stateBeforeUpdate != act->isActive()){
                printf("Turned ");
                if(stateBeforeUpdate){
                    printf("ON");
                }
                else{
                    printf("OFF");
                }
                printf(" at time %d ms\n", time);
            }

            if(time < minOff*1000) {
                REQUIRE(act->isActive() == false); // actuator should still be off
            }
            if(time > (minOff+1)*1000 && time < (minOff + maxOn)*1000) {
                REQUIRE(act->isActive() == true); // actuator should be on now
            }
            if(time > (minOff + maxOn + 2)*1000){
                REQUIRE(act->isActive() == false); // actuator should be off again
                break;
            }
        }
    }

    SECTION("Test that minimum ON time is honored"){
        printf("\n\n**** Testing min ON time for ActuatorOnOff ****\n\n");
        ticks_seconds_t time;
        do {
            delay(100);
            act->setActive(true);
            time = ticks.seconds();
        }while(!act->isActive()); // wait for actuator to go active

        ticks_seconds_t onMoment = time;
        while (act->timeSinceToggle() < minOn) {
            act->setActive(false);
            delay(1000);
            time = ticks.seconds();
            REQUIRE(act->isActive());
        }
        printf("Was ON for %d seconds\n", time - onMoment);
    }
}

