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


#include "InterfaceTypeVisitor.h"
#include "ActuatorPwm.h"
#include "ActuatorMocks.h"
#include "ActuatorMutexGroup.h"
#include "TempSensorMock.h"
#include "Pid.h"
#include "SetPoint.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(InterfaceTypeTest)

BOOST_AUTO_TEST_CASE(specified_interface_type_can_be_queried_from_generic_interface){
    TempSensorInterface * sensor = new TempSensorMock(20.0);
    ActuatorDigitalInterface * dAct = new ActuatorBool();
    ActuatorRangeInterface * rAct = new ActuatorPwm(dAct,4);
    SetPointInterface * sp = new SetPointSimple(20.0);
    ControllerInterface * pid = new Pid(sensor, rAct, sp);

    BOOST_REQUIRE_EQUAL(getInterfaceType(*sensor), 50);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*dAct), 10);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*rAct), 11);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*sp), 100);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*pid), 200);

    Interface * _sensor = sensor;
    Interface * _dAct = dAct;
    Interface * _rAct = rAct;
    Interface * _sp = sp;
    Interface * _pid = pid;

    BOOST_REQUIRE_EQUAL(getInterfaceType(*_sensor), 50);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*_dAct), 10);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*_rAct), 11);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*_sp), 100);
    BOOST_REQUIRE_EQUAL(getInterfaceType(*_pid), 200);
}

BOOST_AUTO_TEST_SUITE_END()
