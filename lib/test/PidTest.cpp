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

// yes this is hacky, but it allows us to some private variables without adding a lot of getters
#define protected public
#include "Pid.h"
#undef protected

#include "SetPoint.h"
#include <cstdio>
#include <math.h>
#include "TempSensorMock.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"
#include "ActuatorPwm.h"
#include "runner.h"
#include <iostream>
#include <fstream>
#include "ActuatorSetPoint.h"
#include "SensorSetPointPair.h"

struct PidTest {
public:
    PidTest() :
        sensor(20.0),
        vAct(),
        act(vAct,4),
        sp(20.0),
        input(sensor, sp),
        pid(input, act)
    {
        BOOST_TEST_MESSAGE( "setup PID test fixture" );

    }
    ~PidTest(){
        BOOST_TEST_MESSAGE( "tear down PID test fixture" );
    }

    TempSensorMock sensor;
    ActuatorBool vAct;
    ActuatorPwm act;
    SetPointSimple sp;
    SensorSetPointPair input;
    Pid pid;
};

// next line sets up the fixture for each test case in this suite
BOOST_FIXTURE_TEST_SUITE( pid_test, PidTest )

// using this fixture test case macro resets the fixture
BOOST_FIXTURE_TEST_CASE(just_proportional, PidTest)
{
    pid.setConstants(10.0, 0, 0);
    sp.write(21.0);

    sensor.setTemp(20.0);

    pid.update();
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0));

    // now try changing the temperature input
    sensor.setTemp(18.0);
    pid.update();

    // inputs are filtered, so output should still be close to the old value
    BOOST_CHECK_CLOSE(double(act.setting()), 10.0, 1);

    for(int i = 0; i<100; i++){
        pid.update();
        act.update();
    }
    // after a enough updates, filters have settled and new PID value is Kp*error
    BOOST_CHECK_CLOSE(double(act.setting()), 30.0, 1);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_integral, PidTest)
{
    pid.setConstants(10.0, 600, 0);
    sp.write(21.0);

    sensor.setTemp(20.0);

    // update for 10 minutes
    for(int i = 0; i < 600; i++){
        pid.update();
        act.update();
        delay(1000);
    }

    // integrator result is Kp * error * 1 / Ti, So 10* 600 * 1 degree error / 600 = 10.0
    // proportional gain is 10, total is 20
    BOOST_CHECK_CLOSE(double(act.setting()), 20.0, 2);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_derivative, PidTest)
{
    pid.setConstants(10.0, 0, 60);
    sp.write(35.0);
    pid.setInputFilter(0);
    pid.setDerivativeFilter(4);

    // update for 10 minutes
    for(int i = 0; i <= 600; i++){
        sensor.setTemp(temp_t(20.0) + temp_t(i*0.015625));
        pid.update();
        act.update();
        delay(1000);
    }

    BOOST_CHECK_EQUAL(sensor.read(), temp_t(29.375)); // sensor value should have gone up 9.375 degrees

    // derivative part is -9.375 (-10*60*0.015625)
    // proportional part is 10.0*(35 - 29.375) = 56.25

    BOOST_CHECK_CLOSE(double(act.setting()), 10.0*(35 - 29.375) - 10*60*0.015625, 5);
}


// using this fixture test case macro resets the fixture
BOOST_FIXTURE_TEST_CASE(just_proportional_cooling, PidTest)
{
    pid.setConstants(10.0, 0, 0);
    pid.setActuatorIsNegative(true);
    sp.write(19.0);

    sensor.setTemp(20.0);

    pid.update();
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0));

    // now try changing the temperature input
    sensor.setTemp(22.0);
    pid.update();

    // inputs are filtered, so output should still be close to the old value
    BOOST_CHECK_CLOSE(double(act.setting()), 10.0, 1);

    for(int i = 0; i<100; i++){
        pid.update();
        act.update();
        delay(1000);
    }
    // after a enough updates, filters have settled and new PID value is Kp*error
    BOOST_CHECK_CLOSE(double(act.setting()), 30.0, 1);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_integral_cooling, PidTest)
{
    pid.setConstants(10.0, 600, 0);
    pid.setActuatorIsNegative(true);
    sp.write(19.0);

    sensor.setTemp(20.0);

    // update for 10 minutes
    for(int i = 0; i < 600; i++){
        pid.update();
        act.update();
        delay(1000);
    }

    // integrator result is error / Ti * time, So 600 * 1 degree error / 60 = 10.0
    BOOST_CHECK_CLOSE(double(act.setting()), 20.0, 2);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_derivative_cooling, PidTest)
{
    pid.setConstants(10.0, 0, 60);
    pid.setActuatorIsNegative(true);
    sp.write(5.0);

    // update for 10 minutes
    for(int i = 0; i <= 600; i++){
        sensor.setTemp(temp_t(20.0) - temp_t(i*0.015625));
        pid.update();
        act.update();
        delay(1000);
    }

    BOOST_CHECK_EQUAL(sensor.read(), temp_t(10.625)); // sensor value should have gone up 9.375 degrees

    BOOST_CHECK_CLOSE(double(act.setting()), 10.0*(10.625-5.0) - 10*0.015625*60, 5);
}

BOOST_FIXTURE_TEST_CASE(integrator_windup_heating_PI, PidTest)
{
    pid.setConstants(10.0, 60, 0);
    sp.write(22.0);
    sensor.setTemp(20.0);

    // update for 20 minutes, integrator will grow by 20 (kp*error) per minute
    for(int i = 0; i < 1200; i++){
        pid.update();
        act.update();
        delay(1000);
    }

    BOOST_CHECK_CLOSE(double(act.setting()), 100.0, 5); // actuator should be at maximum
    BOOST_CHECK_CLOSE(double(pid.i), 80.0, 5); // integral part should be limited to 80 (100 - proportional part)
}

BOOST_FIXTURE_TEST_CASE(integrator_windup_cooling_PI, PidTest)
{
    pid.setConstants(10.0, 60, 0.0);
    pid.setActuatorIsNegative(true);
    sp.write(20.0);
    sensor.setTemp(22.0);

    // update for 20 minutes, integrator will grow by -20 (kp*error) per minute
    for(int i = 0; i < 1200; i++){
        pid.update();
        act.update();
        delay(1000);
    }

    BOOST_CHECK_CLOSE(double(act.setting()), 100.0, 5); // actuator should be at maximum
    BOOST_CHECK_CLOSE(double(pid.i), -80.0, 5); // integral part should be limited to 40 (-100 - proportional part)
}

BOOST_AUTO_TEST_CASE(inputError_is_invalid_and_actuator_zero_when_input_is_invalid_longer_than_10_s){
    auto sp = SetPointSimple(25.0); // setpoint is higher than temperature, actuator will heat
    auto sensor = TempSensorMock(20.0);
    auto pin = ActuatorBool();
    auto act = ActuatorPwm(pin,4);
    auto input = SensorSetPointPair(sensor, sp);
    auto p = Pid(input, act);

    p.setConstants(10.0, 0.0, 0.0);
    p.update();
    BOOST_CHECK_EQUAL(act.setting(), temp_t(50.0)); // 10.0*(25.0-20.0)

    sensor.setConnected(false);
    p.update();

    // last values will be remembered during invalid input shorter than updates
    for(int i=0;i<20;i++){
        p.update(); // is normally called every second
        if(i < 9){
            // before being unavailable for 10 seconds
            BOOST_CHECK_EQUAL(p.inputError, temp_t(-5.0));
            BOOST_CHECK_EQUAL(act.setting(), temp_t(50.0)); // 10.0*(25.0-20.0)
        }
        else{
            // after being unavailable for 10 seconds
            BOOST_CHECK_EQUAL(p.inputError, temp_t::invalid()); // input error is marked as invalid
            BOOST_CHECK_EQUAL(act.setting(), temp_t(0.0)); // actuator is zero
        }
    }

    BOOST_CHECK_EQUAL(p.inputError, temp_t::invalid());
    BOOST_CHECK_EQUAL(act.setting(), temp_t(0.0));
}


BOOST_AUTO_TEST_CASE(pid_driving_setpoint_actuator){
    auto sp = SetPointSimple(25.0); // setpoint is higher than temperature, actuator will heat
    auto sensor = TempSensorMock(20.0);
    auto targetSensor = TempSensorMock(20.0);
    auto targetSetpoint = SetPointSimple(20.0);

    auto input = SensorSetPointPair(sensor, sp);
    auto target = SensorSetPointPair(targetSensor, targetSetpoint);

    auto act = ActuatorSetPoint(target, input);

    auto p = Pid(input, act);

    p.setConstants(2.0, 40, 0);
    p.update();

    // first check correct behavior under normal conditions
    // actuator value will be (sp-sensor)*kp = (25-20)*2 = 10;
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0));

    // setpoint will be reference sp + actuator value = 35
    BOOST_CHECK_EQUAL(targetSetpoint.read(), temp_t(35.0));

    // achieved actuator value will be targetSensor - reference setpoint (sp) = 20.0 - 25.0
    BOOST_CHECK_EQUAL(act.value(), temp_t(-5.0));

    for(int i=0; i<10; i++){
        p.update();
    }
    // integrator will stay at zero due to anti-windup (actuator is not reaching target)
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0)); // still just proportional

    // but if target sensor is reaching value, the integrator will increase
    targetSensor.setTemp(35.0);
    p.update(); // integral will increase with p (10)
    p.update(); // integral is updated after setting output (lags 1 update), so do 2 updates

    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.25)); // proportional (10) + integral (integral/Ti) (10/40=0.25)

    // now check how the pid responds to a disconnected target sensor
    targetSensor.setConnected(false);
    targetSensor.update();
    p.update();

    // setpoint will still be set, because this is what scales the actuators from (for example)
    // beer temp -> fridge temp setting -> actuators
    // the feedback of the actual fridge temp is lost, but the setpoint should still be set

    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.5)); // +0.25 because of another actuator increase

    // setpoint will be reference sp + actuator value = 35.5
    BOOST_CHECK_EQUAL(targetSetpoint.read(), temp_t(35.5));

    // achieved actuator value will be invalid
    BOOST_CHECK_EQUAL(act.value(), temp_t::invalid());
}

BOOST_AUTO_TEST_SUITE_END()
