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



#include "Control.h"

#include <vector>
#include "Pid.h"
#include "defaultDevices.h"
#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "ActuatorTimeLimited.h"
#include "TempSensor.h"
#include "ActuatorSetPoint.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"

Control::Control()
{
    // set up static devices for backwards compatibility with tempControl
    beer1Sensor = &defaultTempSensorBasic;
    beer2Sensor = &defaultTempSensorBasic;
    fridgeSensor = &defaultTempSensorBasic;

    beer1SensorNamed = new TempSensor(beer1Sensor, "beer1");
    beer2SensorNamed = new TempSensor(beer2Sensor, "beer2");
    fridgeSensorNamed = new TempSensor(fridgeSensor, "fridge");

    mutex = new ActuatorMutexGroup();

    heater1Pin = &defaultActuator;
    heater1Mutex = new ActuatorMutexDriver(heater1Pin, mutex);
    heater1 = new ActuatorPwm(heater1Mutex, 4); // period 4s

    heater2Pin = &defaultActuator;
    heater2Mutex = new ActuatorMutexDriver(heater1Pin, mutex);
    heater2 = new ActuatorPwm(heater2Mutex, 4); // period 4s

    coolerPin = &defaultActuator;
    coolerTimeLimited = new ActuatorTimeLimited(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
    coolerMutex = new ActuatorMutexDriver(coolerTimeLimited, mutex);
    cooler = new ActuatorPwm(coolerMutex, 900); // period 15 min

    fridgeSetPointActuator = new ActuatorSetPoint(fridgeSet, beer1Set);

    beer1Set = new SetPointSimple();
    beer2Set = new SetPointSimple();
    fridgeSet = new SetPointSimple();

    heater1Pid = new Pid(fridgeSensor, heater1, fridgeSet);

    coolerPid = new Pid(fridgeSensor, cooler, fridgeSet);
    coolerPid->setActuatorIsNegative(true);

    heater2Pid = new Pid(beer2Sensor, heater2, beer2Set);

    beerToFridgePid = new Pid(beer1Sensor, fridgeSetPointActuator, beer1Set);

    pids.push_back(heater1Pid);
    pids.push_back(heater2Pid);
    pids.push_back(coolerPid);
    pids.push_back(beerToFridgePid);

    sensors.push_back(fridgeSensorNamed);
    sensors.push_back(beer1SensorNamed);
    sensors.push_back(beer2SensorNamed);

    actuators.push_back(cooler);
    actuators.push_back(heater1);
    actuators.push_back(heater2);
}

Control::~Control(){
    delete heater1Mutex;
    delete heater1;

    delete heater2Mutex;
    delete heater2;

    delete coolerTimeLimited;
    delete coolerMutex;
    delete cooler;

    delete fridgeSetPointActuator;

    delete beer1Set;
    delete beer2Set;
    delete fridgeSet;

    delete beer1SensorNamed;
    delete beer2SensorNamed;
    delete fridgeSensorNamed;

    delete mutex;

    delete heater1Pid;
    delete heater2Pid;
    delete coolerPid;
    delete beerToFridgePid;

    pids.clear();
    sensors.clear();
    actuators.clear();
}

void Control::update(){
    for ( auto &sensor : sensors ) {
        sensor->update();
    }

    for ( auto &pid : pids ) {
        pid->update();
    }

    for ( auto &actuator : actuators ) {
        actuator->update();
    }
}

Control control;
