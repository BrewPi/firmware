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
#include "ActuatorPwm.h"
#include "TempSensor.h"

class Control
{
    public:
        Control();

        ~Control(){}

        void init(void);
        void initBackwardsCompatible(void);

        void reset(void);

        void update(void);

        std::vector<TempSensor*> sensors;
        std::vector<Pid*>        pids;
        std::vector<Actuator*>   actuators;

        // pointers for compatibility with device manager
        TempSensor *      beerSensor;
        TempSensor *      fridgeSensor;
        BasicTempSensor * ambientSensor;
        Actuator *        chamberCooler;
        Actuator *     chamberHeater;
        Actuator *     beerHeater;

};


extern Control control;
