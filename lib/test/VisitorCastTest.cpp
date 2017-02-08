/*
 * Copyright 2017 BrewPi/Elco Jacobs.
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


#include "VisitorCast.h"
#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "ActuatorMocks.h"
#include "ActuatorMutexGroup.h"
#include "TempSensorMock.h"
#include "Pid.h"
#include "SetPoint.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(VisitorCastTest)
BOOST_AUTO_TEST_CASE(casting_interfaces_to_specialized_interfaces){
    // Instantiate some test objects
    TempSensorMock * sensor = new TempSensorMock(20.0);
    ActuatorBool * boolAct = new ActuatorBool();
    ActuatorPwm * pwmAct = new ActuatorPwm(*boolAct,4);

    // and some generic pointers to them
    Interface * _sensor = sensor;
    Interface * _boolAct = boolAct;
    Interface * _pwmAct = pwmAct;

    // create a visitor that returns ActuatorAnalog pointers when the cast is possible.
    VisitorCast<ActuatorAnalog> ranger;
    ActuatorAnalog * act;

    // try some impossible casts, these should return nullptr
    sensor->accept(ranger); // A temp sensor cannot be cast to a range actuator
    act = ranger.getCastResult();
    BOOST_REQUIRE_EQUAL(act, static_cast<decltype(act)>(nullptr));

    _sensor->accept(ranger); // Not from a generic interface pointer either
    act = ranger.getCastResult();
    BOOST_REQUIRE_EQUAL(act, static_cast<decltype(act)>(nullptr));

    _boolAct->accept(ranger); // A bool actuator cannot be cast to a range actuator
    act = ranger.getCastResult();
    BOOST_REQUIRE_EQUAL(act, static_cast<decltype(act)>(nullptr));

    // try possible casts
    pwmAct->accept(ranger); // A PWM actuator can be cast to a range actuator
    act = ranger.getCastResult();
    BOOST_REQUIRE_EQUAL(act, pwmAct);

    _pwmAct->accept(ranger); // Also from a generic interface pointer
    act = ranger.getCastResult();
    BOOST_REQUIRE_EQUAL(act, pwmAct);

    // test helper function for testing just one type
    act = asInterface<ActuatorAnalog>(_boolAct);
    BOOST_REQUIRE_EQUAL(act, static_cast<decltype(act)>(nullptr));

    act = asInterface<ActuatorAnalog>(_pwmAct);
    BOOST_REQUIRE_EQUAL(act, pwmAct);
}
BOOST_AUTO_TEST_SUITE_END()
