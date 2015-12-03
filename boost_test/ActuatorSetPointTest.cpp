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


#include "ActuatorSetPoint.h"
#include "defaultDevices.h"
#include "TempSensorMock.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ActuatorSetPointTest)

BOOST_AUTO_TEST_CASE(set_value){
    SetPointSimple targetSetPoint(20.0);
    SetPointSimple referenceSetPoint(20.0);
    TempSensorMock targetSensor(20.0);

    ActuatorSetPoint act(&targetSetPoint, &targetSensor, &referenceSetPoint);

    act.setValue(10.0);

    BOOST_REQUIRE_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(targetSetPoint.read(), temp_t(30.0));
    BOOST_REQUIRE_EQUAL(act.getValue(), temp_t(0.0)); // value is still zero, because targetSensor has not changed

    targetSensor.setTemp(30.0);
    BOOST_REQUIRE_EQUAL(act.getValue(), temp_t(10.0)); // value is 10 when sensor has reached setpoint

    act.setValue(-10.0);
    BOOST_REQUIRE_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(targetSetPoint.read(), temp_t(10.0));
    BOOST_REQUIRE_EQUAL(act.getValue(), temp_t(10.0)); // value is still -10, because targetSensor has not changed

    targetSensor.setTemp(10.0);
    BOOST_REQUIRE_EQUAL(act.getValue(), temp_t(-10.0)); // value is -10 when sensor has reached setpoint
}

BOOST_AUTO_TEST_CASE(min_max){
    SetPointSimple targetSetPoint(20.0);
    SetPointSimple referenceSetPoint(20.0);
    TempSensorMock targetSensor(20.0);

    ActuatorSetPoint act(&targetSetPoint, &targetSensor, &referenceSetPoint, -10.0, 10.0);

    act.setValue(20.0);
    targetSensor.setTemp(30.0);

    BOOST_REQUIRE_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(targetSetPoint.read(), temp_t(30.0));
    BOOST_REQUIRE_EQUAL(act.getValue(), temp_t(10.0));

    act.setValue(-20.0);
    targetSensor.setTemp(10.0);
    BOOST_REQUIRE_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(targetSetPoint.read(), temp_t(10.0));
    BOOST_REQUIRE_EQUAL(act.getValue(), temp_t(-10.0));
}

BOOST_AUTO_TEST_SUITE_END()
