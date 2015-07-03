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

#include "Pid.h"
#include <cstdio>
#include <math.h>
#include "TempSensorMock.h"
#include "Actuator.h"
#include "ActuatorPwm.h"


struct Fixture {
public:
    Fixture(){
        BOOST_TEST_MESSAGE( "setup PID test fixture" );

        sensor = new MockTempSensor(20.0);
        vAct = new BoolActuator();
        act = new ActuatorPwm(vAct,4);

        pid = new Pid(sensor, act);

        // set filtering to minimum.
        pid->setInputFilter(0);
        pid->setDerivativeFilter(0);
    }
    ~Fixture(){
        BOOST_TEST_MESSAGE( "teardown PID test fixture" );
        delete sensor;
        delete vAct;
        delete act;
        delete pid;
    }

    MockTempSensor * sensor;
    Actuator * vAct;
    ActuatorPwm * act;
    Pid * pid;
};

// next line sets up the fixture for each test case in this suite
BOOST_FIXTURE_TEST_SUITE( pid_test, Fixture )

BOOST_AUTO_TEST_CASE (mock_sensor){
    BasicTempSensor * s = new MockTempSensor(20.0);
    temp t = s->read();

    BOOST_CHECK_EQUAL(t, temp(20.0));
}

// using this fixture test case macro resets the fixture
BOOST_FIXTURE_TEST_CASE(just_proportional, Fixture)
{
    pid->setConstants(10.0, 0.0, 0.0);
    pid->setSetPoint(21.0);

    sensor->setTemp(20.0);

    pid->update();
    BOOST_CHECK_EQUAL(act->readValue(), temp(10.0));

    // now try changing the temperature input
    sensor->setTemp(18.0);
    pid->update();

    // inputs are filtered, so output should still be close to the old value
    BOOST_CHECK_CLOSE(double(act->readValue()), 10.0, 1);

    for(int i = 0; i<100; i++){
        pid->update();
    }
    // after a enough updates, filters have settled and new PID value is Kp*error
    BOOST_CHECK_CLOSE(double(act->readValue()), 30.0, 1);
}

BOOST_FIXTURE_TEST_CASE(just_integral, Fixture)
{
    pid->setConstants(0.0, 5.0, 0.0);
    pid->setSetPoint(21.0);

    sensor->setTemp(20.0);

    // update for 10 minutes
    for(int i = 0; i < 600; i++){
        pid->update();
    }

    // integrator result is error * Ki, per minute. So 10 minutes * 1 degree error * 5 = 50.0
    BOOST_CHECK_CLOSE(double(act->readValue()), 50.0, 1);
}

BOOST_FIXTURE_TEST_CASE(just_derivative, Fixture)
{
    pid->setConstants(0.0, 0.0, -5.0);
    pid->setSetPoint(20.0);

    // update for 10 minutes
    for(int i = 0; i <= 600; i++){
        sensor->setTemp(temp(50.0) - temp(i*0.05));
        pid->update();
    }

    BOOST_CHECK_EQUAL(sensor->read(), temp(20.0)); // sensor value should have gone from 50 to 20 in 10 minutes


    // derivative is interpreted as degree per minute, in this case -3 deg / min. PID should be -3*-5 = 15.
    BOOST_CHECK_CLOSE(double(act->readValue()), 15.0, 1);
}

BOOST_AUTO_TEST_SUITE_END()

