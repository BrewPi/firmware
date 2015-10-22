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
#include <string>

#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorPwm.h"
#include "json_writer.h"
#include "json_reader.h"

BOOST_AUTO_TEST_SUITE(EsjTest)

BOOST_AUTO_TEST_CASE(serialize_nested_actuators) {
    //ActuatorBool * actBool = new ActuatorBool();
    //ActuatorTimeLimited * actTl = new ActuatorTimeLimited(actBool, 10, 20);
    ActuatorDigital * boolAct1 = new ActuatorBool();
    ActuatorMutexDriver * mutexAct1 = new ActuatorMutexDriver(boolAct1);
    ActuatorPwm act1(mutexAct1, 20);

    std::string json;

    json = JSON::producer<ActuatorPwm>::convert(act1);
    BOOST_CHECK_EQUAL("TADA", json);

}

BOOST_AUTO_TEST_SUITE_END()

