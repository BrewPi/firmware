/*
* Copyright 2016 BrewPi/Elco Jacobs.
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

#include "ActuatorInterfaces.h"
#include "ActuatorMutexGroup.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorPwm.h"
#include "ActuatorMocks.h"
#include "defaultDevices.h"


BOOST_AUTO_TEST_SUITE(ActuatorInstallHelperTest)

BOOST_AUTO_TEST_CASE(install_and_uninstall_final_actuator){
    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    mutex->setDeadTime(1000);
    ActuatorDigital * coolerPin = new ActuatorBool();
    ActuatorTimeLimited * coolerTimeLimited = new ActuatorTimeLimited(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
    ActuatorMutexDriver * coolerMutex = new ActuatorMutexDriver(coolerTimeLimited, mutex);
    ActuatorPwm * cooler = new ActuatorPwm(coolerMutex, 10); // period 10 min

    ActuatorDigital * heaterPin = new ActuatorBool();
    ActuatorMutexDriver * heaterMutex = new ActuatorMutexDriver(heaterPin, mutex);
    ActuatorPwm * heater = new ActuatorPwm(heaterMutex, 4); // period 4s

    BOOST_CHECK_EQUAL(coolerTimeLimited->getTarget(), coolerPin);

    // make sure actuator is added to the mutex group

    cooler->setValue(short(50));
    heater->setValue(short(50));
    ticks_millis_t start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds
        cooler->update();
        heater->update();
        mutex->update();
        delay(100);
    }

    BOOST_CHECK(cooler->removeNonForwarder()); // returns true on successful uninstall
    BOOST_CHECK(heater->removeNonForwarder()); // returns true on successful uninstall
    BOOST_CHECK_EQUAL(cooler->getTarget(), coolerMutex); // unchanged
    BOOST_CHECK_EQUAL(coolerMutex->getTarget(), coolerTimeLimited); // unchanged
    BOOST_CHECK_EQUAL(coolerTimeLimited->getTarget(), defaultActuator()); // replaced by default actuator
    BOOST_CHECK_EQUAL(heater->getTarget(), heaterMutex); // unchanged
    BOOST_CHECK_EQUAL(heaterMutex->getTarget(), defaultActuator()); // replaced by default actuator

    // mutex group should not crash due to old references to uninstalled actuators
    start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds
        cooler->update();
        heater->update();
        mutex->update();
        delay(100);
    }

    BOOST_CHECK(!cooler->removeNonForwarder()); // returns false, when target is already default actuator
    BOOST_CHECK(!heater->removeNonForwarder()); // returns false, when target is already default actuator

    coolerPin = new ActuatorBool(); // uninstall deleted the previous instance, need to recreate!
    heaterPin = new ActuatorBool(); // uninstall deleted the previous instance, need to recreate!

    BOOST_CHECK(cooler->replaceNonForwarder(coolerPin)); // returns true on successful install
    BOOST_CHECK(heater->replaceNonForwarder(heaterPin)); // returns true on successful install
    BOOST_CHECK_EQUAL(cooler->getTarget(), coolerMutex); // unchanged
    BOOST_CHECK_EQUAL(coolerMutex->getTarget(), coolerTimeLimited); // unchanged
    BOOST_CHECK_EQUAL(coolerTimeLimited->getTarget(), coolerPin); // replaced by default coolerPin again
    BOOST_CHECK_EQUAL(heater->getTarget(), heaterMutex); // unchanged
    BOOST_CHECK_EQUAL(heaterMutex->getTarget(), heaterPin); // replaced by default coolerPin again

    // mutex group should not crash due to old references to uninstalled actuators
    start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds
        cooler->update();
        heater->update();
        mutex->update();
        delay(100);
    }

    BOOST_CHECK(!cooler->replaceNonForwarder(coolerPin)); // returns false when actuator was already installed
    BOOST_CHECK(!heater->replaceNonForwarder(heaterPin)); // returns false when actuator was already installed
}

BOOST_AUTO_TEST_SUITE_END()

