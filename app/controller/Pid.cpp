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

Pid::Pid() {
    integralUpdateCounter = 0;
}

Pid::Pid(const Pid& orig) {
}

Pid::~Pid() {
}

void Pid::setSetPoint(fixed7_9 setPoint) {
    if ((setPoint - this->setPoint) > doubleToTempDiff(0.25)) {
        integral = 0; // reset integrator for big jumps in setpoint
    }
}

void Pid::setMinMax(fixed7_9 min, fixed7_9 max) {
    this->min = min;
    this->max = max;
}

fixed7_9 Pid::update() {
    error = setPoint - input->readSlowFiltered();
    slope = input->readSlope();

    if (integralUpdateCounter++ == 60) {
        integralUpdateCounter = 0;
        temp_diff integratorUpdate = error;

        bool updateSign = (integratorUpdate > 0); // 1 = positive, 0 = negative
        bool integratorSign = (integral > 0);

        if (updateSign == integratorSign) {
            // update will increase integrator action
            if (output >= max || output <= min) {
                //output is already saturated, don't increase integral
                integratorUpdate = 0;
            } else if (slope < 0 && updateSign) {
                // input is decreasing and error is positive
                integratorUpdate = 0; // already moving in the right direction
            } else if (slope > 0 && !updateSign) {
                // input is increasing and error is negative
                integratorUpdate = 0; // already moving in the right direction
            }
        } else {
            // integrator action is decreased. Decrease faster than increase.
            // quadratically when above 0.5 degree difference
            integratorUpdate = integratorUpdate * 2;
            if(error > intToTempDiff(1)){
                integratorUpdate = multiplyFactorTemperatureDiff(integratorUpdate,integratorUpdate);
            }
        }
        integral += integratorUpdate;
    }
    // calculate PID parts.
    p = multiplyFactorTemperatureDiff(Kp, error);
    i = multiplyFactorTemperatureDiffLong(Ki, integral);
    d = multiplyFactorTemperatureDiff(Kd, slope);
    output = constrainTemp(p+i+d, min, max);
    return output;
}