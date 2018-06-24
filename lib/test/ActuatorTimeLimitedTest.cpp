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

#include "ActuatorInterfaces.h"
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

    ActuatorTimeLimited * act = new ActuatorTimeLimited(*v, minOn, minOff, maxOn);

    *output << "\n\n**** Testing min OFF and max ON time for ActuatorTimeLimited ****\n\n";

    while(ticks.seconds() < 10000) {
        ActuatorDigital::State stateBeforeUpdate = act->getState();
        ticks_millis_t time = delay(100);

        act->setState(ActuatorDigital::State::Active);
        act->update(); // turns off the actuator when max on time has passed

        ActuatorDigital::State stateAfterUpdate = act->getState();
        if(stateBeforeUpdate != stateAfterUpdate){
            *output << "Turned ";
            if(stateAfterUpdate == ActuatorDigital::State::Active){
                *output << "ON";
            }
            else if(stateAfterUpdate == ActuatorDigital::State::Inactive){
                *output << "OFF";
            }
            else {
            	BOOST_FAIL("Actuator is in unknown state");
            }
            *output << " at time " << time << "ms\n";
        }

        if(time < minOff*1000) {
            BOOST_REQUIRE_MESSAGE(act->getState() == ActuatorDigital::State::Inactive, "actuator should still be off");
        }
        if(time > (minOff+1)*1000 && time < (minOff + maxOn)*1000) {
            BOOST_REQUIRE_MESSAGE(act->getState() == ActuatorDigital::State::Active, "actuator should be on now");
        }
        if(time > (minOff + maxOn + 2)*1000){
            BOOST_REQUIRE_MESSAGE(act->getState() == ActuatorDigital::State::Inactive, "actuator should be off again");
            break;
        }
    }

    delete v;
    delete act;
}

BOOST_AUTO_TEST_CASE(minimum_on_time_is_honored) {
    srand(time(NULL));
    ticks.reset();
    ActuatorDigital * v = new ActuatorBool();
    const uint16_t minOn = 100;
    const uint16_t maxOn = 200;
    const uint16_t minOff = 300;

    ActuatorTimeLimited * act = new ActuatorTimeLimited(*v, minOn, minOff, maxOn);

    *output << ("\n\n**** Testing min ON time for ActuatorTimeLimited ****\n\n");
    ticks_seconds_t time;
    do {
        delay(100);
        act->setState(ActuatorDigital::State::Active);
        time = ticks.seconds();
    } while (act->getState() == ActuatorDigital::State::Inactive); // wait for actuator to go active

    ticks_seconds_t onMoment = time;
    while (act->timeSinceToggle() < minOn) {
        act->setState(ActuatorDigital::State::Inactive);
        delay(1000);
        time = ticks.seconds();
        BOOST_REQUIRE(act->getState() == ActuatorDigital::State::Active);
    }
    *output << "Was ON for " << time - onMoment << "seconds\n";

    delete v;
	delete act;
}

BOOST_AUTO_TEST_CASE(correct_state_is_returned_with_actuatorNop) {
    ActuatorDigital * v = new ActuatorNop();
    const uint16_t minOn = 100;
    const uint16_t maxOn = 200;
    const uint16_t minOff = 300;

    ActuatorTimeLimited * act = new ActuatorTimeLimited(*v, minOn, minOff, maxOn);

    act->setState(ActuatorDigital::State::Inactive); // make sure cached state is correct
    BOOST_CHECK(act->getState() == ActuatorDigital::State::Inactive);

    delay(minOff*2000); // ensure minOffTime is not blocking the change

    act->setState(ActuatorDigital::State::Active);
    // ActuatorNop cannot go active, so cached state state should stay inactive too.
    BOOST_CHECK(act->getState() == ActuatorDigital::State::Inactive);

    delete v;
	delete act;
}

BOOST_AUTO_TEST_SUITE_END()
