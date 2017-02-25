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
#include "SensorSetPointPair.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ActuatorSetPointTest)

BOOST_AUTO_TEST_CASE(set_value){
    SetPointSimple targetSetPoint(20.0);
    SetPointSimple referenceSetPoint(20.0);
    TempSensorMock targetSensor(20.0);
    TempSensorMock referenceSensor(19.0);

    SensorSetPointPair target(targetSensor, targetSetPoint);
    SensorSetPointPair reference(referenceSensor, referenceSetPoint);

    auto act = ActuatorSetPoint(target, reference);

    act.set(10.0);

    BOOST_CHECK_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(30.0));
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0)); // difference between setpoints is now 10
    BOOST_CHECK_EQUAL(act.value(), temp_t(0.0)); // actual value is still zero, because targetSensor has not changed

    targetSensor.setTemp(30.0);
    BOOST_REQUIRE_EQUAL(act.value(), temp_t(10.0)); // actual value is 10 when sensor has reached setpoint

    act.set(-10.0);
    BOOST_CHECK_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(10.0));
    BOOST_CHECK_EQUAL(act.setting(), temp_t(-10.0)); // difference between setpoints is now 10
    BOOST_CHECK_EQUAL(act.value(), temp_t(10.0)); // value is still -10, because targetSensor has not changed

    targetSensor.setTemp(10.0);
    BOOST_CHECK_EQUAL(act.value(), temp_t(-10.0)); // value is -10 when sensor has reached setpoint

    referenceSetPoint.write(10.0);
    referenceSensor.setTemp(15.0);
    targetSetPoint.write(20.0);
    targetSensor.setTemp(20.0);
    act.set(12.0);

    // when using the reference setpoint as baseline:
    BOOST_CHECK_EQUAL(act.value(), temp_t(10.0)); // value() returns target sensor - ref setpoint
    BOOST_CHECK_EQUAL(act.setting(), temp_t(12.0)); // setting() returns target setpoint - ref setpoint
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(22.0));

    act.setReferenceSettingOrValue(false); // use reference sensor as baseline instead of setpoint
    act.update();

    // when using the reference temperature as baseline:
    BOOST_CHECK_EQUAL(act.value(), temp_t(5.0)); // value() returns target sensor - ref sensor
    BOOST_CHECK_EQUAL(act.setting(), temp_t(12.0)); // setting() returns target setpoint - ref sensor
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(27.0));

    // when reference sensor is invalid, target setpoint will be set to invalid.
    referenceSensor.setConnected(false);
    act.update();
    BOOST_CHECK_EQUAL(act.value(), temp_t::invalid()); // value() returns target sensor - ref sensor
    BOOST_CHECK_EQUAL(act.setting(), temp_t(12.0)); // setting() still returns requested offset
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t::invalid());

    // when reference setpoint is invalid, target setpoint will be set to invalid
    act.setReferenceSettingOrValue(true); // use reference setpoint again
    referenceSensor.setConnected(true);
    referenceSetPoint.write(temp_t::invalid());
    act.update();
    BOOST_CHECK_EQUAL(act.value(), temp_t::invalid()); // value() returns target sensor - ref sensor
    BOOST_CHECK_EQUAL(act.setting(), temp_t(12.0)); // setting() still returns requested offset
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t::invalid());
}

BOOST_AUTO_TEST_CASE(min_max){
    SetPointSimple targetSetPoint(20.0);
    SetPointSimple referenceSetPoint(20.0);
    TempSensorMock targetSensor(20.0);
    TempSensorMock referenceSensor(20.0);

    SensorSetPointPair target(targetSensor, targetSetPoint);
    SensorSetPointPair reference(referenceSensor, referenceSetPoint);

    auto act = ActuatorSetPoint(target, reference, -10.0, 10.0);

    act.set(20.0);
    targetSensor.setTemp(30.0);

    BOOST_CHECK_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(30.0));
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0));

    act.set(-20.0);
    targetSensor.setTemp(10.0);
    BOOST_CHECK_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(10.0));
    BOOST_CHECK_EQUAL(act.setting(), temp_t(-10.0));
}

BOOST_AUTO_TEST_CASE(when_target_sensor_is_invalid_actuator_value_is_invalid){
    SetPointSimple targetSetPoint(20.0);
    SetPointSimple referenceSetPoint(20.0);
    TempSensorMock targetSensor(20.0);
    TempSensorMock referenceSensor(20.0);

    SensorSetPointPair target(targetSensor, targetSetPoint);
    SensorSetPointPair reference(referenceSensor, referenceSetPoint);

    auto act = ActuatorSetPoint(target, reference, -10.0, 10.0);
    act.set(20.0);
    targetSensor.setTemp(30.0);

    targetSensor.setConnected(false);
    act.update();

    BOOST_CHECK_EQUAL(referenceSetPoint.read(), temp_t(20.0));
    BOOST_CHECK_EQUAL(targetSetPoint.read(), temp_t(30.0));
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0)); // set value
    BOOST_CHECK_EQUAL(act.value(), temp_t::invalid()); // achieved value
}

BOOST_AUTO_TEST_SUITE_END()
