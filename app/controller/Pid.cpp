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
    setConstants(temp_t(10.0), temp_t(0.2), temp_t(-1.5));
    setMinMax(temp_t::min(), temp_t::max());

    p = 0;
    i = 0;
    d = 0;
    inputError           = 0;
    derivative      = 0;
    integral        = 0;
    failedReadCount = 0;

    setInputSensor(input);
    setOutputActuator(output);

//    autotune = false;
//    tuning = false;
//    outputLag = 0;
//    maxDerivative = 0.0;
    setPoint = temp_t::invalid_val;
}

Pid::~Pid(){}

void Pid::setConstants(temp_long_t kp,
                       temp_long_t ki,
                       temp_long_t kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

void Pid::update()
{
    temp_t inputVal;
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
                derivativeFilter.init(temp_precise_t(0.0));

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

    temp_precise_t delta = inputFilter.readOutput() - inputFilter.readPrevOutput();
    derivativeFilter.add(delta * temp_precise_t(60.0)); // use slope per minute

    derivative = derivativeFilter.readOutput();

    // calculate PID parts.
    p = Kp * inputError;
    i = integral; // integral is fed with Ki*error
    d = temp_long_t(-1) * Kd * derivative;

    temp_long_t pidResult = temp_long_t(p) + temp_long_t(i) + temp_long_t(d);
    temp_t      output    = pidResult;    // will be constrained to -128/128 here

//    temp previousOutput = outputActuator->readValue();

    outputActuator -> setValue(output);

    // get actual value from actuator
    output = outputActuator->readValue();

    // update integral with anti-windup back calculation
    // pidResult - output is zero when actuator is not saturated
    temp_long_t antiWindup = (pidResult - output);

    integral = integral + Ki * (inputError - antiWindup) / temp_long_t(60);
/*
    if(autotune){
        tune(output, previousOutput);
    }
*/

}

void Pid::setSetPoint(temp_t val)
{
    setPoint = val;
}


void Pid::setMinMax(temp_t min,
                    temp_t max)
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
    temp_t t = s -> read();

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

/*

// Tune the PID with the Ziegler-Nichols Open-Loop Tuning Method or Process Reaction Method
// This determines the dead time and the reaction rate (max derivative) and calculates the PID parameters from that.
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
    if(output < tuningThreshold){
        if(lagTimer > 2*(derivativeFilter.getDelay() + inputFilter.getDelay())){
            tuning = false; // only stop tuning if filters have had time to settle
        }
    }

    // TODO: when this happens, check the filter delay and see if the maximum still has to come

    // Detect when at max derivative, the time until this happens is the lag time
    // Together with the maximum derivative, this is used to determine the PID parameters

    if(tuning){ // only for heating now
        // if the derivative of the input starts falling, we have hit an inflection point
        // Also check that the derivative is positive
        if(derivativeFilter.isFalling() && (derivativeFilter.readOutput() > temp_precise(0))){
            maxDerivative = derivativeFilter.readOutput(); // we're at the peak or past it
            uint16_t filterDelay = derivativeFilter.getDelay();
            uint16_t timeToMaxDerivative = (lagTimer <filterDelay) ? 0  : lagTimer - filterDelay;

            // set PID constants to have no overshoot

            temp_long deadTime = temp_long(timeToMaxDerivative) / temp_long(60.0); // derivative and integral are per minute, scale back here
            temp_long riseTime = (inputFilter.readOutput() - tuningStartTemp)  / derivative;
            if(riseTime < temp_long(0)){
                riseTime = 0.0;
            }
            deadTime = (deadTime > riseTime) ? deadTime - riseTime : temp_long(0); // rise time is not part of the dead time, eliminate it

            outputLag =  uint16_t(deadTime * temp_long(60)); // store outputlag in seconds

            temp_long RL = derivative * deadTime;

            if (RL < temp_long(0.25)){ // prevent divide by zero
                Kp = 160.0;
            }
            else{
                Kp = temp_long(100.0*0.4) / RL; // not aggressive. Quarter decay is 1.2 instead of 0.4. We don't want overshoot
            }

            if(deadTime > temp_long(1)){
                Ki = Kp/(deadTime+deadTime);
            }
            else{
                Ki = Kp*temp_long(0.5);
            }
            Kd = Kp*deadTime*temp_long(0.33);


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

*/
