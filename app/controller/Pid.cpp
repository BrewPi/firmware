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

Pid::Pid(BasicTempSensor * input,
         LinearActuator *        output)
{
    setConstants(temp(10.0), temp(0.2), temp(-1.5));
    setMinMax(temp::min(), temp::max());

    p = 0;
    i = 0;
    d = 0;
    inputError           = 0;
    derivative      = 0;
    integral        = 0;
    failedReadCount = 0;

    setInputSensor(input);
    setOutputActuator(output);

    autotune = false;
    tuning = false;
    outputLag = 0;
    maxDerivative = 0.0;
    setPoint = temp::invalid_val;
}

Pid::~Pid(){}

void Pid::setConstants(temp_long kp,
                       temp_long ki,
                       temp_long kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

void Pid::update()
{
    temp inputVal;
    bool disable = false;

    if( setPoint.isDisabledOrInvalid()){
        disable = true;
    }

    if (!inputSensor || (inputVal = inputSensor -> read()).isDisabledOrInvalid()){
        // Could not read from input sensor
        if (failedReadCount < 255){    // limit
            failedReadCount++;
        }

        // Try to reconnect
        if (inputSensor -> init()){
            if (failedReadCount > 60){
                // re-initialize filters if sensor has been lost longer than 60 seconds
                inputVal = inputSensor -> read();

                inputFilter.init(inputVal);
                derivativeFilter.init(temp_precise(0.0));

                failedReadCount = 0;
            }
        } else{
            if (failedReadCount > 60){
                disable = true;
            }
        }
    }

    if ( disable ){
        inputError = 0;
        p = 0;
        i = 0;
        d = 0;
        outputActuator -> setActive(false);
        return;
    }

    inputFilter.add(inputVal);

    inputError = setPoint - inputFilter.readOutput();

    temp_precise delta = inputFilter.readOutput() - inputFilter.readPrevOutput();
    derivativeFilter.add(delta * temp_precise(60.0)); // use slope per minute

    derivative = derivativeFilter.readOutput();

    // calculate PID parts.
    p = Kp * inputError;
    i = integral; // integral is fed with Ki*error
    d = Kd * derivative;

    temp_long pidResult = temp_long(p) + temp_long(i) + temp_long(d);
    temp      output    = pidResult;    // will be constrained to -128/128 here

    temp previousOutput = outputActuator->readValue();

    outputActuator -> setValue(output);

    // get actual value from actuator
    output = outputActuator->readValue();

    // update integral with anti-windup back calculation
    // pidResult - output is zero when actuator is not saturated
    temp_long antiWindup = (pidResult - output);

    integral = integral + Ki * (inputError - antiWindup) / temp_long(60);

    if(autotune){
        tune(output, previousOutput);
    }
}

void Pid::tune(temp output, temp previousOutput){
    static uint16_t lagTimer = 0;
    static temp tuningStartTemp = inputFilter.readOutput();

    temp min = outputActuator->min();
    temp max = outputActuator->max();
    temp tuningThreshold = (max >> uint8_t(1)) + (min >> uint8_t(1)); // (min + max) / 2

    if(output == outputActuator->max() && previousOutput < tuningThreshold){
        tuning = true; // only start tuning at a big step to the maximum output
    }
    // cancel tuning when the output is under the tuning threshold before maximum derivative is detected
    tuning = tuning & (output > tuningThreshold);

    // Detect when at max derivative, the time until this happens is the lag time
    // Together with the maximum derivative, this is used to determine the PID parameters

    if(tuning){
        if(derivativeFilter.detectPosPeak(&maxDerivative)){
            uint16_t filterDelay = derivativeFilter.getDelay();
            uint16_t timeToMaxDerivative = (lagTimer <filterDelay) ? 0  : lagTimer - filterDelay;

            // set PID constants to Ziegler-Nichols tuned settings for a decay ratio of 0.25

            temp_long lag = temp_long(timeToMaxDerivative) / temp_long(60.0); // derivative and integral are per minute, scale back here
            temp_long riseTime = (inputFilter.readOutput() - tuningStartTemp)  / derivative;
            if(riseTime < temp_long(0)){
                riseTime = 0.0;
            }
            lag = lag - riseTime; // rise time is not part of the lag
            if(lag < temp_long(0)){
                lag = 0.0;
            }

            outputLag =  uint16_t(lag * temp_long(60)); // store outputlag in seconds

            temp_long RL = derivative * lag;
            if (RL < temp_long(0.5)){ // prevent divide by zero
                Kp = 240.0;
            }
            else{
                Kp = temp_long(100.0*1.2) / RL;
            }

            Kd = Kp*lag*temp_long(-0.5);

            if(lag > temp_long(2)){
                Ki = Kp/(lag+lag);
            }
            else{
                Ki = Kp*temp_long(0.25);
            }

            // The delay time of the filter is the time it takes to rise to 0.5 in a step response
            // This is almost the same as the rise time here. Set the filters so that they can track the fastest rise
            // so rise time / 2, have to scale back to seconds again here

            inputFilter.setFilteringForDelay(uint16_t(temp_long(30) * riseTime));
            derivativeFilter.setFilteringForDelay(uint16_t(temp_long(30) * riseTime));

            tuning = false; // tuning ready
        }
        else{
            if(lagTimer < UINT16_MAX){
                lagTimer++;
            }
        }
    }
    else{
        lagTimer= 0;
        tuningStartTemp = inputFilter.readOutput();
    }
}

void Pid::setSetPoint(temp val)
{
    /*if ((val - setPoint) > temp(0.25)){
        integral = 0;    // reset integrator for big jumps in setpoint
    }*/

    setPoint = val;
}

void Pid::setMinMax(temp min,
                    temp max)
{
    this -> min = min;
    this -> max = max;
}

void Pid::setFiltering(uint8_t b){
    inputFilter.setFiltering(b);
    derivativeFilter.setFiltering(b);
}

uint8_t Pid::getFiltering(){
    return inputFilter.getFiltering();
}

void Pid::setInputFilter(uint8_t b)
{
    inputFilter.setFiltering(b);
}

void Pid::setDerivativeFilter(uint8_t b)
{
    derivativeFilter.setFiltering(b);
}

bool Pid::setInputSensor(BasicTempSensor * s)
{
    temp t = s -> read();

    if (t.isDisabledOrInvalid()){
        return false;    // could not read from sensor
    }

    inputSensor = s;

    inputFilter.init(t);
    derivativeFilter.init(0.0);

    return true;
}

bool Pid::setOutputActuator(LinearActuator * a)
{
    outputActuator = a;

    return true;
}

