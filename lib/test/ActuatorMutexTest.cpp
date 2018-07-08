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
#include "ActuatorMutexGroup.h"
#include "ActuatorMutexDriver.h"

BOOST_AUTO_TEST_SUITE(ActuatorMutexTest)

BOOST_AUTO_TEST_CASE(two_actuators_belonging_to_the_same_group_cannot_be_active_at_once) {
    auto act1 = ActuatorBool();
    auto act2 = ActuatorBool();
    auto mutex = ActuatorMutexGroup();

    auto actm1 = ActuatorMutexDriver(act1, mutex);
    auto actm2 = ActuatorMutexDriver(act2, mutex);

    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(actm1.getState() == ActuatorDigital::State::Active); // actuator is first, so is allowed to go active
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Active); // target actuator is active

    actm2.setState(ActuatorDigital::State::Active, 10);
    BOOST_CHECK(act2.getState() == ActuatorDigital::State::Inactive); // actuator 2 is not allowed to go active while act1 is still active

    actm1.setState(ActuatorDigital::State::Inactive, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Inactive);
    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Inactive); // second request to go active from act1 is blocked, because act2 has an open request with higher priority

    actm2.setState(ActuatorDigital::State::Active, 10);
    BOOST_CHECK(act2.getState() == ActuatorDigital::State::Active); // actuator 2 is now allowed to go active
}


BOOST_AUTO_TEST_CASE(dead_time_between_actuators_is_honored) {
    auto act1 = ActuatorBool();
    auto act2 = ActuatorBool();
    auto mutex = ActuatorMutexGroup();

    auto actm1 = ActuatorMutexDriver(act1, mutex);
    auto actm2 = ActuatorMutexDriver(act2, mutex);

    mutex.setDeadTime(10000); // 10 seconds dead time

    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(actm1.getState() == ActuatorDigital::State::Active); // actuator is first, so is allowed to go active
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Active); // target actuator is active

    delay(1000);

    actm2.setState(ActuatorDigital::State::Active, 10);
    BOOST_CHECK(act2.getState() == ActuatorDigital::State::Inactive); // actuator 2 is not allowed to go active while act1 is still active

    actm1.setState(ActuatorDigital::State::Inactive, 5);
    BOOST_CHECK(actm1.getState() == ActuatorDigital::State::Inactive);
    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(actm1.getState() == ActuatorDigital::State::Inactive); // second request to go active from act1 is blocked, because act2 has an open request with higher priority

    actm2.setState(ActuatorDigital::State::Active, 10);
    BOOST_CHECK(actm2.getState() == ActuatorDigital::State::Inactive); // actuator 2 is still not allowed to go active, because of dead time

    int i;
    for(i = 1; i<20; i++){
        delay(1000);
        actm2.setState(ActuatorDigital::State::Active, 10);
        if(actm2.getState() == ActuatorDigital::State::Active){
            break;
        }

    }
    BOOST_CHECK(actm2.getState() == ActuatorDigital::State::Active);
    BOOST_CHECK_EQUAL(i,10); // act2 was allowed to go active after 10 seconds
}


BOOST_AUTO_TEST_CASE(dead_time_does_not_block_same_actuator_from_going_active_again) {
    auto act1 = ActuatorBool();
    auto act2 = ActuatorBool();
    auto mutex = ActuatorMutexGroup();

    auto actm1 = ActuatorMutexDriver(act1, mutex);
    auto actm2 = ActuatorMutexDriver(act2, mutex);

    mutex.setDeadTime(10000); // 10 seconds dead time

    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Active); // target actuator is active

    delay(1000);

    actm1.setState(ActuatorDigital::State::Inactive, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Inactive); // target actuator is inactive

    delay(1000);
    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Active); // target actuator is active again
}


BOOST_AUTO_TEST_CASE(mutex_works_with_time_limited_actuator) {
    auto act1 = ActuatorBool();
    auto act1tl = ActuatorTimeLimited(act1, 10, 20);
    auto act2 = ActuatorBool();

    delay(20000); // let initial minimum off time pass

    auto mutex = ActuatorMutexGroup();

    auto actm1 = ActuatorMutexDriver(act1tl, mutex);
    auto actm2 = ActuatorMutexDriver(act2, mutex);

    actm1.setState(ActuatorDigital::State::Active, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Active); // target actuator is active

    for(int i=0; i<200; i++){
        mutex.update(); // does not pass time
    }

    actm1.setState(ActuatorDigital::State::Inactive, 5);
    BOOST_CHECK(act1.getState() == ActuatorDigital::State::Active); // target actuator is still active due to time limit

    for(int i=0; i<200; i++){
        mutex.update();
    }
    actm2.setState(ActuatorDigital::State::Active, 10);
    BOOST_CHECK(act2.getState() == ActuatorDigital::State::Inactive); // actuator 2 is not allowed to go active while act1 is still active
}

BOOST_AUTO_TEST_CASE(when_there_have_been_no_requests_for_a_while_dead_time_is_still_honored_on_new_request) {
    // this test was introduced to make sure lastActiveTime is not only updated on requests

    auto act1 = ActuatorBool();
    auto act1tl = ActuatorTimeLimited(act1, 10, 20);
    auto act2 = ActuatorBool();

    delay(20000); // let initial minimum off time pass


    auto mutex = ActuatorMutexGroup();

    auto actm1 = ActuatorMutexDriver(act1tl, mutex);
    auto actm2 = ActuatorMutexDriver(act2, mutex);
    mutex.setDeadTime(10000); // 10 seconds dead time

    actm1.setState(ActuatorDigital::State::Active, 5);

    for(int i=0; i<200; i++){
        mutex.update();
        delay(1000);
    }
    actm1.setState(ActuatorDigital::State::Inactive, 5);

    actm2.setState(ActuatorDigital::State::Active, 10);
    BOOST_CHECK(act2.getState() == ActuatorDigital::State::Inactive); // not allowed, because of dead time
}

BOOST_AUTO_TEST_CASE(test_destructors1) {
    auto act1 = new ActuatorBool();
    auto act2 = new ActuatorBool();
    auto mutex = new ActuatorMutexGroup();

    auto actm1 = new ActuatorMutexDriver(*act1, *mutex);
    auto actm2 = new ActuatorMutexDriver(*act2, *mutex);

    delete mutex;
    delete actm1;
    delete actm2;
    delete act1;
    delete act2;
}


BOOST_AUTO_TEST_SUITE_END()

