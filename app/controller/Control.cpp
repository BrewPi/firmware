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



#include "Control.h"

#include <stddef.h>

#include "../devices/Actuator/Actuator.h"
#include "../devices/Sensor.h"
#include "../devices/TempSensor/TempSensorDisconnected.h"


Control::Control(){}

extern ValueSensor<bool>      defaultSensor;
extern ValueActuator          defaultActuator;
extern DisconnectedTempSensor defaultTempSensor;

void Control::initBackwardsCompatible(void)
{

    // this is for cases where the device manager hasn't configured beer/fridge sensor.
    if (beerSensor == NULL)
    {
        beerSensor = new TempSensor(TEMP_SENSOR_TYPE_BEER, &defaultTempSensor);
        beerSensor -> init();
    }

    if (fridgeSensor == NULL)
    {
        fridgeSensor = new TempSensor(TEMP_SENSOR_TYPE_FRIDGE, &defaultTempSensor);
        fridgeSensor -> init();
    }

    if (chamberHeater == NULL){
        chamberHeater = new ActuatorPwm(&defaultActuator, 4);
    }
    if (chamberCooler == NULL){
            chamberCooler = new ActuatorPwm(&defaultActuator, 600);
        }

    if (beerHeater == NULL)
    {
        beerHeater = new ActuatorPwm(&defaultActuator, 4);
    }

    Pid * beerHeaterPid   = new Pid(beerSensor, beerHeater);
    Pid * fridgeHeaterPid = new Pid(fridgeSensor, chamberHeater);
    Pid * fridgeCoolerPid = new Pid(fridgeSensor, chamberCooler);
    //Pid * beerToFridgePid = new Pid;

    pids.push_back(beerHeaterPid);
    pids.push_back(fridgeHeaterPid);
    pids.push_back(fridgeCoolerPid);
    // pids.push_back(beerToFridgePid);

    for ( auto &i : pids ) {
       // i->setConstants(doubleToTempDiff(5.0), doubleToTempDiff(0.2), doubleToTempDiff(-1.5));
        i->setInputFilter(4u);
        i->setDerivativeFilter(4u);
        i->setDoubleDerivativeFilter(4u);
        i->setMinMax(0, 255);
    }
}

