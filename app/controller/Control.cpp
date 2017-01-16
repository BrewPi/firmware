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
#include "Platform.h"

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
    beer1Sensor = new TempSensor(defaultTempSensor());
    beer1Sensor->setName("beer1");
    beer2Sensor = new TempSensor(defaultTempSensor());
    beer2Sensor->setName("beer2");
    fridgeSensor = new TempSensor(defaultTempSensor());
    fridgeSensor->setName("fridge");

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

    heaterInputSensor = new TempSensorFallback(fridgeSensor, beer1Sensor);
    heater1Pid = new Pid(PtrLookup(heaterInputSensor), PtrLookup(heater1), PtrLookup(fridgeSet));
    heater1Pid->setName("heater1");

    coolerInputSensor = new TempSensorFallback(fridgeSensor, beer1Sensor);
    coolerPid = new Pid(PtrLookup(coolerInputSensor), PtrLookup(cooler), PtrLookup(fridgeSet));
    coolerPid->setActuatorIsNegative(true);
    coolerPid->setName("cooler");

    heater2Pid = new Pid(PtrLookup(beer2Sensor), PtrLookup(heater2), PtrLookup(beer2Set));
    heater2Pid->setName("heater2");

    beerToFridgePid = new Pid(PtrLookup(beer1Sensor), PtrLookup(fridgeSetPointActuator), PtrLookup(beer1Set));
    beerToFridgePid->setName("beer2fridge");

    beer1Set->setName("beer1set");
    beer2Set->setName("beer2set");
    fridgeSet->setName("fridgeset");

    objects.push_back(heater1Pid);
    objects.push_back(heater2Pid);
    objects.push_back(coolerPid);
    objects.push_back(beerToFridgePid);

    objects.push_back(fridgeSensor);
    objects.push_back(beer1Sensor);
    objects.push_back(beer2Sensor);
    objects.push_back(coolerInputSensor);
    objects.push_back(heaterInputSensor);

    objects.push_back(cooler);
    objects.push_back(heater1);
    objects.push_back(heater2);

    objects.push_back(beer1Set);
    objects.push_back(beer2Set);
    objects.push_back(fridgeSet);

    objects.push_back(mutex);

    mutex->setDeadTime(1800000); // 30 minutes
}

Control::~Control(){
#if defined(ARDUINO) || defined(SPARK)
    // global control object is static and never destroyed.
    // omit proper destructor to save space.
#else
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

    delete mutex;

    delete heater1Pid;
    delete heater2Pid;
    delete coolerPid;
    delete beerToFridgePid;

    objects.clear();
#endif
}

// This update function should be called every second
void Control::update(){
    for ( auto &obj : objects ) {
        obj->update();
    }
}

void Control::fastUpdate(){
    for ( auto &obj : objects ) {
    	obj->fastUpdate();
    }
}

void Control::serialize(JSON::Adapter& adapter){
    JSON::Class root(adapter, "Control");
    JSON_T(adapter, objects);
}

Control control;
