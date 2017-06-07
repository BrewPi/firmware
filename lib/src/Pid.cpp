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

Pid::Pid(ProcessValue& _input, ProcessValue& _output) :
         input(_input),
         output(_output),
         Kp(0.0),
         Ti(0),
         Td(0),
         p(decltype(p)::base_type(0)),
         i(decltype(i)::base_type(0)),
         d(decltype(p)::base_type(0)),
         inputError(decltype(inputError)::base_type(0)),
         derivative(decltype(derivative)::base_type(0)),
         integral(decltype(integral)::base_type(0)),
         failedReadCount(255), // start at 255, so inputFilter is refreshed at first valid read
         actuatorIsNegative(false),
         enabled(true),
         previousSetPoint(temp_t::invalid())
{
    setInputFilter(0);
    // some filtering necessary due to quantization causing steps in the temperature
    setDerivativeFilter(2);
}

void Pid::setConstants(temp_long_t kp,
                       uint16_t ti,
                       uint16_t td)
{
    Kp = kp;
    Ti = ti;
    Td = td;
}

void Pid::update()
{
    temp_t currentSetPoint = input.setting();
    temp_t inputVal = input.value();
    bool validSensor = !inputVal.isDisabledOrInvalid();
    bool validSetPoint = !currentSetPoint.isDisabledOrInvalid();

    if (!validSensor){
        // Could not read from input sensor
        if (failedReadCount < 255){    // limit
            failedReadCount++;
        }
    }
    else{
        if (failedReadCount > 60){ // filters are stale, re-initialize them
            inputFilter.init(inputVal);
            derivativeFilter.init(temp_precise_t(0.0));
        }
        failedReadCount = 0;
    }

    bool tooManyFailedReads = false;

    if(validSensor){ // only update internal filters and inputError if input sensor is valid
        inputFilter.add(inputVal);

        if(validSetPoint){
            if(previousSetPoint.isDisabledOrInvalid()){
                previousSetPoint = currentSetPoint;
            }
            temp_precise_t previousError = inputFilter.readPrevOutput() - previousSetPoint;
            temp_precise_t currentError = inputFilter.readOutput() - currentSetPoint;
            temp_precise_t delta = currentError - previousError;
            previousSetPoint = currentSetPoint;

            inputError = currentError; // store input error, as temp_t, instead of temp_precise_t

            // Add to derivative filter shifted, because of limited precision for such low values
            // Limit to 0.125 degree per second, to prevent overflow in shift and to eliminate setpoint changes
            // 128/1024 = 0.125 C/s.
            temp_precise_t deltaClipped = delta;
            temp_precise_t max = temp_precise_t::max() >> uint8_t(10);
            temp_precise_t min = temp_precise_t::min() >> uint8_t(10);
            if(deltaClipped > max){
                deltaClipped = max;
            }
            else if(deltaClipped < min){
                deltaClipped = min;
            }
            derivativeFilter.add(deltaClipped << uint8_t(10));
            derivative = derivativeFilter.readOutput() >> uint8_t(10);
        }
        else{
            derivativeFilter.add(temp_precise_t(0.0));
        }
    }
    else{
        if(failedReadCount > 10){
            tooManyFailedReads = true; // after 10 failed reads, disable pid
        }

    }

    if(!enabled || tooManyFailedReads || !validSetPoint){
        inputError = temp_t::invalid();
        p = decltype(p)(0.0);
        i = decltype(i)(0.0);
        d = decltype(p)(0.0);
    }
    else{
        // calculate PID parts.
        p = Kp * -inputError;
        i = (Ti != 0) ? (integral/Ti) : temp_long_t(0.0);
        d = -Kp * (derivative * Td);
    }

    if(!enabled){
        return;
    }

    temp_long_t pidResult = temp_long_t(p) + temp_long_t(i) + temp_long_t(d);

    // Get output to send to actuator. When actuator is a 'cooler', invert the result
    temp_t      outputSetting    = (actuatorIsNegative) ? -pidResult : pidResult;

    output.set(outputSetting);

    // get the value that is clipped to the actuator's range
    outputSetting = output.setting();
    // When actuator is a 'cooler', invert the output again
    outputSetting = (actuatorIsNegative) ? -outputSetting : outputSetting;

    if(Ti == 0){ // 0 has been chosen to indicate that the integrator is disabled. This also prevents divide by zero.
        integral = decltype(integral)::base_type(0);
    }
    else{
        // update integral with anti-windup back calculation
        // pidResult - output is zero when actuator is not saturated
        // when the actuator is close the to pidResult (setpoint), disable anti-windup
        // this prevens small fluctuations from keeping the integrator at zero

        integral = integral + p;

        temp_long_t antiWindup(temp_long_t::base_type(0));
        if(pidResult != temp_long_t(outputSetting)){ // clipped to actuator min or max set in target actuator
            antiWindup = pidResult - outputSetting;
            antiWindup *= 5; // Anti windup gain is 5 when clipping to min/max
        }
        else{ // Actuator could be not reaching set value due to physics or limits in its target actuator
              // Get the actual achieved value in actuator. This could differ due to slowness time/mutex limits
            temp_t achievedOutput = output.value();
            if(!achievedOutput.isDisabledOrInvalid()){ // only apply anti-windup when it is possible to read back the actual value
                // When actuator is a 'cooler', invert the output again
                temp_long_t achievedOutputWithCorrectSign = (actuatorIsNegative) ? -achievedOutput : achievedOutput;

                // if the proportional part is bigger than what has been achieved by the actuator, apply anti-windup
                if(actuatorIsNegative){
                    if(p < achievedOutputWithCorrectSign){
                        antiWindup = (p - achievedOutputWithCorrectSign);
                    }
                }
                else{
                    if(p > achievedOutputWithCorrectSign){
                        antiWindup = (p - achievedOutputWithCorrectSign);
                    }
                }
                antiWindup *= 3; // Anti windup gain is 3 for this kind of windup
            }
        }

        // only apply anti-windup if it will decrease the integral and prevent crossing through zero
        if(integral.sign() * antiWindup.sign() == 1){
            if((integral - antiWindup).sign() != integral.sign()){
                integral = decltype(integral)::base_type(0);
            }
            else{
                integral -= antiWindup;
            }
        }
    }
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

