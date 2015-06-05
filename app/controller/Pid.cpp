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

Pid::Pid(TempSensor * input, Actuator * output)
{
    setConstants(doubleToTempDiff(10.0), doubleToTempDiff(0.2), doubleToTempDiff(-1.5));
    setMinMax(MIN_TEMP, MAX_TEMP);

    integralUpdateCounter = 0;
    error                 = 0;
    derivative            = 0;
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
    Ka = 5*Kp;
}

fixed7_9 Pid::update(fixed7_9 input)
{
    inputFilter.add(input);

    error = setPoint - inputFilter.readOutput();

    derivativeFilter.addDoublePrecision(inputFilter.readOutputDoublePrecision()
                                   - inputFilter.readOldestOutputDoublePrecision());

    doubleDerivativeFilter.addDoublePrecision(derivativeFilter.readOutputDoublePrecision()
                                   - derivativeFilter.readOldestOutputDoublePrecision());

    derivative = derivativeFilter.readOutput();
    doubleDerivative = doubleDerivativeFilter.readOutput();



    // calculate PID parts.
    p      = multiplyFactorTemperatureDiff(Kp, error);
    i      = multiplyFactorTemperatureDiffLong(Ki, integral);
    d      = multiplyFactorTemperatureDiff(Kd, derivative);

    long_temperature pidResult = p + i + d;

    output = constrainTemp(pidResult, min, max);

    // update integral with anti-windup back calculation
    integral += (error + multiplyFactorTemperatureDiff(Ka, output-pidResult)); // pidResult - output is zero when actuator is not saturated

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
