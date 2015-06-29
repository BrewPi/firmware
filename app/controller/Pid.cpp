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

Pid::Pid(BasicTempSensor * input, Actuator * output)
{
    setConstants(temp(10.0), temp(0.2), temp(-1.5));
    setMinMax(temp::min(), temp::max());

    error                 = 0;
    derivative            = 0;
    integral              = 0;
    failedReadCount       = 0;
}

Pid::~Pid(){}

void Pid::setConstants(temp kp,
                       temp ki,
                       temp kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
    Ka = temp(5.0)*Kp;
}

void Pid::update()
{
    temp inputVal;

    if (!inputSensor || (inputVal = inputSensor->read())==TEMP_SENSOR_DISCONNECTED) {
        // Could not read from input sensor
        if(failedReadCount < 255){  // limit
            failedReadCount++;
        }
        // Try to reconnect
        if(inputSensor->init()){
            if(failedReadCount>60){
                // re-initialize filters if sensor has been lost longer than 60 seconds
                inputVal = inputSensor->read();
                inputFilter.init(inputVal);
                derivativeFilter.init(temp_precise(0.0));
                failedReadCount = 0;
            }
        } else{
            if(failedReadCount>60){
                outputActuator->setActive(false);
            }
            return;
        }
    }

    inputFilter.add(inputSensor->read());

    error = setPoint - inputFilter.readOutput();

    temp_precise delta = inputFilter.readOutput() - inputFilter.readOldestOutput();
    derivativeFilter.add(delta);

    derivative = derivativeFilter.readOutput();


    // calculate PID parts.
    p      = Kp * error;
    i      = Ki * integral;
    d      = Kp * derivative;

    temp_long pidResult = temp_long(p) + temp_long(i) + temp_long(d);

    temp output = pidResult; // will be constrained to -128/128 here

    if(output < min){
        output = min;
    }
    if (output > max){
        output = max;
    }

    // update integral with anti-windup back calculation
    integral = integral + error + Ka*(output-pidResult); // pidResult - output is zero when actuator is not saturated

}

void Pid::setSetPoint(temp val)
{
    if ((val - setPoint) > temp(0.25)){
        integral = 0;    // reset integrator for big jumps in setpoint
    }
    setPoint = val;
}

void Pid::setMinMax(temp min,
                    temp max)
{
    this -> min = min;
    this -> max = max;
}

void Pid::setInputFilter(uint8_t b){
    inputFilter.setCoefficients(b);
}

void Pid::setDerivativeFilter(uint8_t b){
    derivativeFilter.setCoefficients(b);
}

