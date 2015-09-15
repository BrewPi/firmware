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
#include "runner.h"
#include <iostream>
#include <fstream>


struct PidTest {
public:
    PidTest(){
        BOOST_TEST_MESSAGE( "setup PID test fixture" );

        sensor = new MockTempSensor(20.0);
        vAct = new BoolActuator();
        act = new ActuatorPwm(vAct,4);

        pid = new Pid(sensor, act);

        // set filtering to minimum.
        pid->setInputFilter(0);
        pid->setDerivativeFilter(0);
    }
    ~PidTest(){
        BOOST_TEST_MESSAGE( "tear down PID test fixture" );
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

struct FridgeSim : public PidTest {
    double airTemp = 20.0;
    double beerTemp = 20.0;
    double envTemp = 18.0;

    double airCapacity = 5 * 1.0035 * 1.225 * 0.200; // 5 * heat capacity of dry air * density of air * 200L volume (in kJ per kelvin).
    double beerCapacity = 4.2 * 1.0 * 20; // heat capacity water * density of water * 20L volume (in kJ per kelvin).

    double heaterPower = 0.3; // 300W, in kW.

    double envAirTransfer= 0.01;
    double airBeerTransfer= 0.02;

    double airEnergy = airTemp * airCapacity;
    double beerEnergy = beerTemp * beerCapacity;

    void updateSimBeerControl(temp actuatorValue){ // input 1-100
        double airTempNew = airTemp;
        double beerTempNew = beerTemp;
        airTempNew += heaterPower * double(actuatorValue) / (100.0 * airCapacity);

        airTempNew += (envTemp - airTemp) * envAirTransfer;

        airTempNew += (beerTemp - airTemp) * airBeerTransfer / airCapacity;
        beerTempNew += (airTemp - beerTemp) * airBeerTransfer / beerCapacity;

        airTemp = airTempNew;
        beerTemp = beerTempNew;

        sensor->setTemp(beerTemp);
    }

    void updateSimFridgeControl(temp actuatorValue){ // input 1-100
       double airTempNew = airTemp;
       double beerTempNew = beerTemp;
       airTempNew += heaterPower * double(actuatorValue) / (100.0 * airCapacity);

       airTempNew += (envTemp - airTemp) * envAirTransfer;

       airTempNew += (beerTemp - airTemp) * airBeerTransfer / airCapacity;
       beerTempNew += (airTemp - beerTemp) * airBeerTransfer / beerCapacity;

       airTemp = airTempNew;
       beerTemp = beerTempNew;

       sensor->setTemp(airTempNew);
    }
};

// next line sets up the fixture for each test case in this suite
BOOST_FIXTURE_TEST_SUITE( pid_test, PidTest )

BOOST_AUTO_TEST_CASE (mock_sensor){
    BasicTempSensor * s = new MockTempSensor(20.0);
    temp t = s->read();

    BOOST_CHECK_EQUAL(t, temp(20.0));
}

// using this fixture test case macro resets the fixture
BOOST_FIXTURE_TEST_CASE(just_proportional, PidTest)
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

BOOST_FIXTURE_TEST_CASE(just_integral, PidTest)
{
    pid->setConstants(0.0, 5.0, 0.0);
    pid->setSetPoint(21.0);

    sensor->setTemp(20.0);

    // update for 10 minutes
    for(int i = 0; i < 600; i++){
        pid->update();
    }

    // integrator result is error * Ki, per minute. So 10 minutes * 1 degree error * 5 = 50.0
    BOOST_CHECK_CLOSE(double(act->readValue()), 50.0, 2);
}

BOOST_FIXTURE_TEST_CASE(just_derivative, PidTest)
{
    pid->setConstants(0.0, 0.0, 5.0);
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

// Test heating fridge air based on beer temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(double_step_response_beer_control, FridgeSim)
{
    pid->setConstants(100.0, 0.0, 0.0);
    pid->setAutoTune(true);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "setPoint, error, beer, air, actuator, p, i, d, Kp, Ki, Kd" << endl;
    double setPoint = 20;
    for(int t = 0; t < 6000; t++){
        if(t==500){
            setPoint = 21;
        }
        if(t==2500){
            setPoint = 24;
        }
        pid->setSetPoint(setPoint);
        pid->update();

        temp outputVal = act->readValue();
        updateSimBeerControl(outputVal);
        csv << setPoint << "," << (beerTemp - setPoint) << "," << beerTemp << "," << airTemp << "," <<
            outputVal << "," << pid->p << "," << pid->i << "," << pid->d << ", " <<
            pid->Kp << "," << pid->Ki << "," << pid-> Kd << endl;
    }
    csv.close();
}

// Test heating fridge air based on fridge temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(double_step_response_fridge_control, FridgeSim)
{
    pid->setConstants(100.0, 0.0, 0.0);
    pid->setAutoTune(true);
    pid->setFiltering(0);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "setPoint, error, beer, air, actuator, p, i, d, Kp, Ki, Kd" << endl;
    double setPoint = 20;
    for(int t = 0; t < 6000; t++){
        if(t==500){
            setPoint = 24;
        }
        if(t==2500){
            setPoint = 28;
        }
        pid->setSetPoint(setPoint);
        pid->update();

        temp outputVal = act->readValue();
        updateSimFridgeControl(outputVal);
        csv << setPoint << "," << (airTemp - setPoint) << "," << beerTemp << "," << airTemp << "," <<
            outputVal << "," << pid->p << "," << pid->i << "," << pid->d << ", " <<
            pid->Kp << "," << pid->Ki << "," << pid-> Kd << endl;
    }
    csv.close();
}

BOOST_AUTO_TEST_SUITE_END()

