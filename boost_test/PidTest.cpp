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


struct PidTest {
public:
    PidTest(){
        BOOST_TEST_MESSAGE( "setup PID test fixture" );

        sensor = new TempSensorMock(20.0);
        vAct = new ActuatorBool();
        act = new ActuatorPwm(vAct,4);
        sp = new SetPointSimple(20.0);

        pid = new Pid(sensor, act, sp);
    }
    ~PidTest(){
        BOOST_TEST_MESSAGE( "tear down PID test fixture" );
        delete sensor;
        delete vAct;
        delete act;
        delete pid;
    }

    TempSensorMock * sensor;
    ActuatorDigital * vAct;
    ActuatorPwm * act;
    Pid * pid;
    SetPointSimple * sp;
};

// next line sets up the fixture for each test case in this suite
BOOST_FIXTURE_TEST_SUITE( pid_test, PidTest )

// using this fixture test case macro resets the fixture
BOOST_FIXTURE_TEST_CASE(just_proportional, PidTest)
{
    pid->setConstants(10.0, 0, 0);
    sp->write(21.0);

    sensor->setTemp(20.0);

    pid->update();
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(10.0));

    // now try changing the temperature input
    sensor->setTemp(18.0);
    pid->update();

    // inputs are filtered, so output should still be close to the old value
    BOOST_CHECK_CLOSE(double(act->getValue()), 10.0, 1);

    for(int i = 0; i<100; i++){
        pid->update();
    }
    // after a enough updates, filters have settled and new PID value is Kp*error
    BOOST_CHECK_CLOSE(double(act->getValue()), 30.0, 1);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_integral, PidTest)
{
    pid->setConstants(10.0, 600, 0);
    sp->write(21.0);

    sensor->setTemp(20.0);

    // update for 10 minutes
    for(int i = 0; i < 600; i++){
        pid->update();
    }

    // integrator result is Kp * error * 1 / Ti, So 10* 600 * 1 degree error / 600 = 10.0
    // proportional gain is 10, total is 20
    BOOST_CHECK_CLOSE(double(act->getValue()), 20.0, 2);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_derivative, PidTest)
{
    pid->setConstants(10.0, 0, 60);
    sp->write(35.0);
    pid->setInputFilter(0);
    pid->setDerivativeFilter(4);

    // update for 10 minutes
    for(int i = 0; i <= 600; i++){
        sensor->setTemp(temp_t(20.0) + temp_t(i*0.015625));
        pid->update();
    }

    BOOST_CHECK_EQUAL(sensor->read(), temp_t(29.375)); // sensor value should have gone up 9.375 degrees

    // derivative part is -9.375 (-10*60*0.015625)
    // proportional part is 10.0*(35 - 29.375) = 56.25

    BOOST_CHECK_CLOSE(double(act->getValue()), 10.0*(35 - 29.375) - 10*60*0.015625, 5);
}


// using this fixture test case macro resets the fixture
BOOST_FIXTURE_TEST_CASE(just_proportional_cooling, PidTest)
{
    pid->setConstants(10.0, 0, 0);
    pid->setActuatorIsNegative(true);
    sp->write(19.0);

    sensor->setTemp(20.0);

    pid->update();
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(10.0));

    // now try changing the temperature input
    sensor->setTemp(22.0);
    pid->update();

    // inputs are filtered, so output should still be close to the old value
    BOOST_CHECK_CLOSE(double(act->getValue()), 10.0, 1);

    for(int i = 0; i<100; i++){
        pid->update();
    }
    // after a enough updates, filters have settled and new PID value is Kp*error
    BOOST_CHECK_CLOSE(double(act->getValue()), 30.0, 1);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_integral_cooling, PidTest)
{
    pid->setConstants(10.0, 600, 0);
    pid->setActuatorIsNegative(true);
    sp->write(19.0);

    sensor->setTemp(20.0);

    // update for 10 minutes
    for(int i = 0; i < 600; i++){
        pid->update();
    }

    // integrator result is error / Ti * time, So 600 * 1 degree error / 60 = 10.0
    BOOST_CHECK_CLOSE(double(act->getValue()), 20.0, 2);
}

BOOST_FIXTURE_TEST_CASE(proportional_plus_derivative_cooling, PidTest)
{
    pid->setConstants(10.0, 0, 60);
    pid->setActuatorIsNegative(true);
    sp->write(5.0);

    // update for 10 minutes
    for(int i = 0; i <= 600; i++){
        sensor->setTemp(temp_t(20.0) - temp_t(i*0.015625));
        pid->update();
    }

    BOOST_CHECK_EQUAL(sensor->read(), temp_t(10.625)); // sensor value should have gone up 9.375 degrees

    BOOST_CHECK_CLOSE(double(act->getValue()), 10.0*(10.625-5.0) - 10*0.015625*60, 5);
}

BOOST_FIXTURE_TEST_CASE(integrator_windup_heating_PI, PidTest)
{
    pid->setConstants(10.0, 60, 0);
    sp->write(22.0);
    sensor->setTemp(20.0);

    // update for 10 minutes, integrator will grow by 20 (kp*error) per minute (60)
    for(int i = 0; i < 600; i++){
        pid->update();
    }

    BOOST_CHECK_CLOSE(double(act->getValue()), 100.0, 5); // actuator should be at maximum
    BOOST_CHECK_CLOSE(double(pid->i), 80.0, 5); // integral part should be limited to 80 (100 - proportional part)
}

BOOST_FIXTURE_TEST_CASE(integrator_windup_cooling_PI, PidTest)
{
    pid->setConstants(10.0, 60, 0.0);
    pid->setActuatorIsNegative(true);
    sp->write(20.0);
    sensor->setTemp(22.0);

    // update for 10 minutes, integrator will grow by -20 (kp*error) per minute (60)
    for(int i = 0; i < 600; i++){
        pid->update();
    }

    BOOST_CHECK_CLOSE(double(act->getValue()), 100.0, 5); // actuator should be at maximum
    BOOST_CHECK_CLOSE(double(pid->i), -80.0, 5); // integral part should be limited to 40 (-100 - proportional part)
}


/*
BOOST_FIXTURE_TEST_CASE(auto_tuning_test, PidTest)
{
    pid->setConstants(50.0, 0.0, 0.0);
    pid->setSetPoint(20.0);
    pid->setAutoTune(true);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "setpoint, sensor, output lag, max derivative, actuator, p, i, d, Kp, Ki, Kd" << endl;

    // rise temp from 20 to 30, with a delayed response
    for(int t = -50; t < 600; t++){
        // step response from 10 to 20 degrees with delay and slow transition
        // rises from 20 at t=200 to 30 at t=300
        // maximum derivative is 0.1 at t=250 and sensorVal=25
        // rise time is 50
        // the setpoint changes at 50, so the detected delay should be 150


        if(t==0){
            pid->setSetPoint(30.0);
        }

        temp sensorVal;
        if(t <= 100){
            sensorVal = 20;
        }
        else if(t <= 300){
            double t_ = 0.01*(t-200); // scale so transition is at 200
            sensorVal = 25 + 10 * t_ / (1 + ( t_* t_ ));
        }
        else{
            sensorVal = 30;
        }

        sensor->setTemp(sensorVal);
        pid->update();
        csv << pid->getSetPoint() << ", " << sensorVal << ", " <<
                pid->getOutputLag() << ",  "<< pid->getMaxDerivative() << ", " <<
                act->readValue() << "," << pid->p << "," << pid->i << "," << pid->d << "," <<
                pid->Kp << "," << pid->Ki << "," << pid-> Kd << endl;
    }
    csv.close();

    BOOST_CHECK_CLOSE(double(pid->getOutputLag()), 150, 1);
    BOOST_CHECK_CLOSE(double(pid->getMaxDerivative()), 0.1 * 60, 1); // derivative is per minute

    // For Ziegler-Nichols tuning for a decay ratio of 0.25, the following conditions should be true:
    // R = maximum derivative = 10 degrees / 100s = 0.1 deg/s = 6 deg/min
    // L = lag time = 150s = 2.5 min
    // Kp = 1.2 / (RL)
    // Ki = Kp * 1/(2L)
    // Kd = Kp * 0.5L

    // Here we use less agressive tuning to reduce overshoot
    // Kp = 0.4 / (RL)
    // Ki = Kp * 1/(2L)
    // Kd = Kp * 0.33L

    // Keep in mind that actuators outputs are 0-100 and derivative and integral are per minute
    BOOST_CHECK_CLOSE(double(pid->Kp) , 100 * 0.4/(6.0 * 2.5), 5);
    BOOST_CHECK_CLOSE(double(pid->Ki), double(pid->Kp) / (2 * 2.5), 5);
    BOOST_CHECK_CLOSE(double(pid->Kd), double(pid->Kp) * 0.33 * 2.5, 5);
}
*/

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(pid_initialization) // a new suite without the fixture

BOOST_AUTO_TEST_CASE(pid_can_update_after_bare_init_without_crashing){
    Pid * p = new Pid();
    p->update();
}

BOOST_AUTO_TEST_CASE(pid_can_update_with_only_actuator_defined){
    TempSensorBasic * sensor = new TempSensorMock(20.0);
    Pid * p = new Pid();
    p->setInputSensor(sensor);
    p->update();
}

BOOST_AUTO_TEST_CASE(pid_can_update_with_only_sensor_defined){
    ActuatorDigital * pin = new ActuatorBool();
    ActuatorRange * act = new ActuatorPwm(pin,4);
    Pid * p = new Pid();
    p->setOutputActuator(act);
    p->update();
}

BOOST_AUTO_TEST_CASE(pid_can_update_with_only_setpoint_defined){
    SetPoint * sp = new SetPointSimple(20.0);
    Pid * p = new Pid();
    p->setSetPoint(sp);
    p->update();
}

BOOST_AUTO_TEST_SUITE_END()

