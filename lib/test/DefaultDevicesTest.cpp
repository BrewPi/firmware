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


#include "defaultDevices.h"
#include "temperatureFormats.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(DefaultDevicesTest)

BOOST_AUTO_TEST_CASE(defaultTempSensor_works_with_all_TempSensorBasic_functions){
    TempSensorBasic * sensor = defaultTempSensorBasic();

    BOOST_CHECK_EQUAL(sensor->isConnected(), false);

    BOOST_CHECK_EQUAL(sensor->init(), false);

    BOOST_CHECK_EQUAL(sensor->read(), temp_t::invalid());
}

BOOST_AUTO_TEST_CASE(defaultSetPoint_works_for_all_SetPoint_functions){
    SetPoint * setpoint = defaultSetPoint();

    setpoint->write(20.0); // doesn't return anything, just should not crash

    BOOST_CHECK_EQUAL(setpoint->read(), temp_t::invalid()); // value should still be invalid
}

BOOST_AUTO_TEST_CASE(defaultLinearActuator_works_for_all_ActuatorRange_functions){
    ActuatorRange * act = defaultLinearActuator();

    BOOST_CHECK_EQUAL(act->getBareActuator(), act);
    BOOST_CHECK_EQUAL(act->type(), ACTUATOR_RANGE);

    BOOST_CHECK_EQUAL(act->getValue(), temp_t::invalid());
    act->setValue(50.0); // should do nothing
    BOOST_CHECK_EQUAL(act->getValue(), temp_t::invalid());

    BOOST_CHECK_EQUAL(act->min(), temp_t::invalid());
    BOOST_CHECK_EQUAL(act->max(), temp_t::invalid());
}

BOOST_AUTO_TEST_SUITE_END()
