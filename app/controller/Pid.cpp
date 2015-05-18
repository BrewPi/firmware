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



#include "Pid.h"
#include "TempSensor.h"
#include "TemperatureFormats.h"

Pid::Pid()
{
    setConstants(doubleToTempDiff(5.0), doubleToTempDiff(0.2), doubleToTempDiff(-1.5));
    setMinMax(MIN_TEMP, MAX_TEMP);

    integralUpdateCounter = 0;
    error                 = 0;
    slope                 = 0;
    integral              = 0;
}

Pid::~Pid(){}

void Pid::setConstants(fixed7_9 kp,
                       fixed7_9 ki,
                       fixed7_9 kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

fixed7_9 Pid::update(fixed7_9 input)
{
    inputFilter.add(input);

    error = setPoint - inputFilter.readOutput();

    slopefilter.addDoublePrecision(inputFilter.readOutputDoublePrecision()
                                   - inputFilter.readOldestOutputDoublePrecision());

    slope = slopefilter.readOutput();

    if (integralUpdateCounter++ == 60){
        integralUpdateCounter = 0;

        temp_diff integratorUpdate = error;
        bool      updateSign       = (integratorUpdate > 0);    // 1 = positive, 0 = negative
        bool      integratorSign   = (integral > 0);

        if (updateSign == integratorSign){
            // update will increase integrator action
            if ((output >= max) || (output <= min)){
                // output is already saturated, don't increase integral
                integratorUpdate = 0;
            } else if ((slope < 0) && updateSign){
                // input is decreasing and error is positive
                integratorUpdate = 0;    // already moving in the right direction
            } else if ((slope > 0) &&!updateSign){
                // input is increasing and error is negative
                integratorUpdate = 0;    // already moving in the right direction
            }
        } else{
            // integrator action is decreased. Decrease faster than increase.
            // quadratically when above 0.5 degree difference
            integratorUpdate = integratorUpdate * 2;

            if (error > intToTempDiff(1)){
                integratorUpdate = multiplyFactorTemperatureDiff(integratorUpdate, integratorUpdate);
            }
        }

        integral += integratorUpdate;
    }

    // calculate PID parts.
    p      = multiplyFactorTemperatureDiff(Kp, error);
    i      = multiplyFactorTemperatureDiffLong(Ki, integral);
    d      = multiplyFactorTemperatureDiff(Kd, slope);
    output = constrainTemp(p + i + d, min, max);

    return output;
}

void Pid::setSetPoint(fixed7_9 val)
{
    if ((val - this -> setPoint) > doubleToTempDiff(0.25)){
        integral = 0;    // reset integrator for big jumps in setpoint
    }
}

void Pid::setMinMax(fixed7_9 min,
                    fixed7_9 max)
{
    this -> min = min;
    this -> max = max;
}
