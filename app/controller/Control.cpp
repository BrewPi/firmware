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
#include "json_writer.h"

Control::Control()
{
    // set up static devices for backwards compatibility with tempControl
    beer1Sensor = new TempSensor(defaultTempSensorBasic(), "beer1");
    beer2Sensor = new TempSensor(defaultTempSensorBasic(), "beer2");
    fridgeSensor = new TempSensor(defaultTempSensorBasic(), "fridge");

    mutex = new ActuatorMutexGroup();

    heater1Mutex = new ActuatorMutexDriver(defaultActuator(), mutex);
    heater1 = new ActuatorPwm(heater1Mutex, 4); // period 4s

    heater2Mutex = new ActuatorMutexDriver(defaultActuator(), mutex);
    heater2 = new ActuatorPwm(heater2Mutex, 4); // period 4s

    coolerTimeLimited = new ActuatorTimeLimited(defaultActuator(), 120, 180); // 2 min minOn time, 3 min minOff
    coolerMutex = new ActuatorMutexDriver(coolerTimeLimited, mutex);
    cooler = new ActuatorPwm(coolerMutex, 1200); // period 20 min

    beer1Set = new SetPointSimple();
    beer2Set = new SetPointSimple();
    fridgeSet = new SetPointSimple();

    fridgeSetPointActuator = new ActuatorSetPoint(fridgeSet, fridgeSensor, beer1Set);
    fridgeSetPointActuator->setMin(-10.0);
    fridgeSetPointActuator->setMax(10.0);

    heater1Pid = new Pid(fridgeSensor, heater1, fridgeSet);
    heater1Pid->setName("heater1");

    coolerPid = new Pid(fridgeSensor, cooler, fridgeSet);
    coolerPid->setActuatorIsNegative(true);
    coolerPid->setName("cooler");

    heater2Pid = new Pid(beer2Sensor, heater2, beer2Set);
    heater2Pid->setName("heater2");

    beerToFridgePid = new Pid(beer1Sensor, fridgeSetPointActuator, beer1Set);
    beerToFridgePid->setName("beer2fridge");

    pids.push_back(heater1Pid);
    pids.push_back(heater2Pid);
    pids.push_back(coolerPid);
    pids.push_back(beerToFridgePid);

    sensors.push_back(fridgeSensor);
    sensors.push_back(beer1Sensor);
    sensors.push_back(beer2Sensor);

    actuators.push_back(cooler);
    actuators.push_back(heater1);
    actuators.push_back(heater2);

    beer1SetNamed = new SetPointNamed(beer1Set, "beer1set");
    beer2SetNamed = new SetPointNamed(beer2Set, "beer2set");
    fridgeSetNamed = new SetPointNamed(fridgeSet, "fridgeset");

    setpoints.push_back(beer1SetNamed);
    setpoints.push_back(beer2SetNamed);
    setpoints.push_back(fridgeSetNamed);

    mutex->setDeadTime(1800000); // 30 minutes
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

    delete beer1SetNamed;
    delete beer2SetNamed;
    delete fridgeSetNamed;

    delete mutex;

    delete heater1Pid;
    delete heater2Pid;
    delete coolerPid;
    delete beerToFridgePid;

    pids.clear();
    sensors.clear();
    actuators.clear();
}

// This update function should be called every second
void Control::update(){
    updateSensors();
    updatePids();
    updateActuators();
    mutex->update();
}

void Control::updatePids(){
    for ( auto &pid : pids ) {
        pid->update();
    }
}

// The actuator update should be called often to generate the PWM signal
void Control::updateSensors(){
    for ( auto &sensor : sensors ) {
        sensor->update();
    }
}

void Control::updateActuators(){
    for ( auto &actuator : actuators ) {
        actuator->update();
    }
}

void Control::serialize(JSON::Adapter& adapter){
    JSON::Class root(adapter, "Control");
    JSON_E(adapter, pids);
    JSON_E(adapter, sensors);
    JSON_E(adapter, actuators);
    JSON_T(adapter, setpoints);
}

Control control;
