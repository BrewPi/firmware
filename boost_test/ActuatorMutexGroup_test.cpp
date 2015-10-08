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

#include "Actuator.h"
#include "ActuatorTimeLimited.h"
#include "Ticks.h"
#include "ActuatorMutexGroup.h"

BOOST_AUTO_TEST_SUITE(ActuatorMutex)

BOOST_AUTO_TEST_CASE(two_actuators_belonging_to_the_same_group_cannot_be_active_at_once) {
    ActuatorDigital * act1 = new ActuatorBool();
    ActuatorDigital * act2 = new ActuatorBool();
    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();

    if(mutex->requestActive(act1, 5)){
        act1->setActive(true);
    }
    BOOST_REQUIRE(mutex->requestActive(act1, 5)); // already active actuator request is allowed
    BOOST_REQUIRE(mutex->requestActive(act1, 4)); // also with lower priority
    BOOST_REQUIRE(!mutex->requestActive(act2, 1)); // second actuator request is not allowed
    act1->setActive(false);
    BOOST_REQUIRE(mutex->requestActive(act2, 1)); // until first actuator is disabled
}

BOOST_AUTO_TEST_SUITE_END()

