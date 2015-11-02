/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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



#pragma once

#include <vector>
#include "Pid.h"
#include "ActuatorInterfaces.h"
#include "TempSensor.h"
#include "ActuatorMutexGroup.h"
#include "json_writer.h"
#include "ActuatorSetPoint.h"

class Control
{
public:
    Control();

    ~Control();

    void update();

    void serialize(JSON::Adapter& adapter);

    std::vector<SetPointNamed*> setpoints;
    std::vector<TempSensor*> sensors;
    std::vector<Pid*>        pids;
    std::vector<Actuator*>   actuators;

    // static setup below, we should support generating this dynamically later
protected:
    TempSensorBasic * fridgeSensor;
    TempSensorBasic * beer1Sensor;
    TempSensorBasic * beer2Sensor;

    TempSensor * fridgeSensorNamed;
    TempSensor * beer1SensorNamed;
    TempSensor * beer2SensorNamed;

    ActuatorDigital * coolerPin;
    ActuatorDigital * coolerTimeLimited;
    ActuatorDigital * coolerMutex;
    ActuatorRange * cooler;

    ActuatorDigital * heater1Pin;
    ActuatorDigital * heater1Mutex;
    ActuatorRange * heater1;

    ActuatorDigital * heater2Pin;
    ActuatorDigital * heater2Mutex;
    ActuatorRange *   heater2;

    ActuatorSetPoint * fridgeSetPointActuator;

    ActuatorMutexGroup * mutex;

    Pid * heater1Pid;
    Pid * heater2Pid;
    Pid * coolerPid;
    Pid * beerToFridgePid;

    SetPoint * beer1Set;
    SetPoint * beer2Set;
    SetPoint * fridgeSet;

    SetPointNamed * beer1SetNamed;
    SetPointNamed * beer2SetNamed;
    SetPointNamed * fridgeSetNamed;

    friend class TempControl;
    friend class DeviceManager;
};

extern Control control;
