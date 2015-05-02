/*
 * Copyright 2015 BrewPi / Elco Jacobs
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

#include "TemperatureFormats.h"
#include "FilterCascaded.h"

#pragma once

class Pid {
public:
    Pid();
    Pid(const Pid& orig);
    virtual ~Pid();
    fixed7_9 update();
    void setSetPoint(fixed7_9* setPoint);
    void setMinMax(fixed7_9 min, fixed7_9 max);
    
private:
    fixed7_9 Kp;
    fixed7_9 Ki;
    fixed7_9 Kd;
    fixed7_9 min;
    fixed7_9 max;
    temp_diff iMaxError;

    temperature* setPoint;
    TempSensor* input;
    fixed7_9 output;

    fixed7_9 p;
    fixed7_9 i;
    fixed7_9 d;

    temp_diff error;
    temp_diff slope;
    long_temperature integral;
        
    uint8_t integralUpdateCounter;
};
