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
#include "TempSensorDelegate.h"
#include "ActuatorSetPoint.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include "json_writer.h"
#include "VisitorCast.h"

Control::Control() :
    fridgeSensor(),
    beer1Sensor(),
    beer2Sensor(),
    roomSensor(),
    fridgeSensorWithFallback(fridgeSensor, beer1Sensor), // fall back to beer sensor if fridge sensor is unavailable
    beer1Set(),
    beer2Set(),
    fridgeSet(),
    mutex(),
    coolerToggle(),
    coolerTimeLimited(coolerToggle, 120, 180), // 2 min minOn time, 3 min minOff
    coolerMutex(coolerTimeLimited, &mutex),
    coolerPwm(coolerMutex, 1200), // period 20 min
    coolerPid(fridgeSensorWithFallback, coolerPwm, fridgeSet),
    heater1Toggle(),
    heater1Mutex(heater1Toggle, &mutex),
    heater1Pwm(heater1Mutex, 4), // period 4s
    heater1Pid(fridgeSensorWithFallback, heater1Pwm, fridgeSet),
    heater2Toggle(),
    heater2Mutex(heater2Toggle, &mutex),
    heater2Pwm(heater2Mutex, 4), // period 4s
    heater2Pid(beer2Sensor, heater2Pwm, beer2Set),
    fridgeSetPointActuator(fridgeSet, fridgeSensor, beer1Set),
    beerToFridgePid(beer1Sensor, fridgeSetPointActuator, beer1Set)
{
    // set up static devices for backwards compatibility with tempControl
    coolerPid.setActuatorIsNegative(true);
    mutex.setDeadTime(1800000); // 30 minutes
    fridgeSetPointActuator.setMin(-10.0);
    fridgeSetPointActuator.setMax(10.0);

    fridgeSensor.setName("fridge");
    beer1Sensor.setName("beer1");
    beer2Sensor.setName("beer2");
    beer2Sensor.setName("room");
    coolerToggle.setName("cooler");
    heater1Toggle.setName("heater1");
    heater2Toggle.setName("heater2");
    heater1Pid.setName("heater1pid");
    heater2Pid.setName("heater2pid");
    coolerPid.setName("coolerpid");
    beerToFridgePid.setName("beer2fridgepid");
    beer1Set.setName("beer1set");
    beer2Set.setName("beer2set");
    fridgeSet.setName("fridgeset");

    objects.push_back(&fridgeSensor);
    objects.push_back(&beer1Sensor);
    objects.push_back(&beer2Sensor);
    objects.push_back(&fridgeSensorWithFallback);
    objects.push_back(&roomSensor);

    objects.push_back(&beer1Set);
    objects.push_back(&beer2Set);
    objects.push_back(&fridgeSet);

    objects.push_back(&mutex);

    objects.push_back(&heater1Pid);
    objects.push_back(&heater2Pid);
    objects.push_back(&coolerPid);
    objects.push_back(&beerToFridgePid);

    objects.push_back(&coolerPwm);
    objects.push_back(&heater1Pwm);
    objects.push_back(&heater2Pwm);
}

Control::~Control(){
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
    std::vector<Interface *> pids;
    VisitorCast<Pid> vcp;
    // filter out only PIDs
    for ( auto &obj : objects ) {
        obj->accept(vcp);
        if(vcp.getCastResult() != nullptr){
            pids.push_back(obj);
        }
    }
    JSON_T(adapter, pids);
}

Control control;
