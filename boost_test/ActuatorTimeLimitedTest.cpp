/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/test/unit_test.hpp>

#include "runner.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time, to seed rand */
#include <cstring>

#include "ActuatorTimeLimited.h"
#include "ActuatorMocks.h"
#include "Ticks.h"


BOOST_AUTO_TEST_SUITE(ActuatorTimeLimited_with_bool_driver)

BOOST_AUTO_TEST_CASE(minimum_off_time_and_maximum_on_time_are_honored) {
    srand(time(NULL));
    ticks.reset();
    ActuatorDigital * v = new ActuatorBool();
    const uint16_t minOn = 100;
    const uint16_t maxOn = 200;
    const uint16_t minOff = 300;

    ActuatorTimeLimited * act = new ActuatorTimeLimited(v, minOn, minOff, maxOn);


    output << "\n\n**** Testing min OFF and max ON time for ActuatorTimeLimited ****\n\n";

    while(ticks.seconds() < 10000) {
        bool stateBeforeUpdate = act->isActive();
        ticks_millis_t time = delay(100);

        act->setActive(true);
        act->update(); // turns off the actuator when max on time has passed

        if(stateBeforeUpdate != act->isActive()){
            output << "Turned ";
            if(stateBeforeUpdate){
                output << "ON";
            }
            else{
                output << "OFF";
            }
            output << " at time " << time << "ms\n";
        }

        if(time < minOff*1000) {
            BOOST_REQUIRE_MESSAGE(act->isActive() == false, "actuator should still be off");
        }
        if(time > (minOff+1)*1000 && time < (minOff + maxOn)*1000) {
            BOOST_REQUIRE_MESSAGE(act->isActive() == true, "actuator should be on now");
        }
        if(time > (minOff + maxOn + 2)*1000){
            BOOST_REQUIRE_MESSAGE(act->isActive() == false, "actuator should be off again");
            break;
        }
    }
}

BOOST_AUTO_TEST_CASE(minimum_on_time_is_honored) {
    srand(time(NULL));
    ticks.reset();
    ActuatorDigital * v = new ActuatorBool();
    const uint16_t minOn = 100;
    const uint16_t maxOn = 200;
    const uint16_t minOff = 300;

    ActuatorTimeLimited * act = new ActuatorTimeLimited(v, minOn, minOff, maxOn);

    output << ("\n\n**** Testing min ON time for ActuatorTimeLimited ****\n\n");
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
        BOOST_REQUIRE(act->isActive());
    }
    output << "Was ON for " << time - onMoment << "seconds\n";

}

BOOST_AUTO_TEST_SUITE_END()
