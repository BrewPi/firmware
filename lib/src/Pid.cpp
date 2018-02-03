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
         settings({
             0.0,
             0,
             0,
             true
         }),
         state({
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0,
            0.0
         }),
         failedReadCount(255), // start at 255, so inputFilter is refreshed at first valid read
         previousInputSetting(temp_t::invalid())
{
    setInputFiltering(0);
    // some filtering necessary due to quantization causing steps in the temperature
    setDerivativeFiltering(2);
}

void Pid::setConstants(temp_long_t kp,
                       uint16_t ti,
                       uint16_t td)
{
    settings.kp = kp;
    settings.ti = ti;
    settings.td = td;
}

void Pid::update()
{
    state.inputValue = input.value();
    state.inputSetting = input.setting();
    bool validSensor = !state.inputValue.isDisabledOrInvalid();
    bool validInputSetting = !state.inputSetting.isDisabledOrInvalid();

    if (!validSensor){
        // Could not read from input sensor
        if (failedReadCount < 255){    // limit
            failedReadCount++;
        }
    }
    else{
        if (failedReadCount > 60){ // filters are stale, re-initialize them
            inputFilter.init(state.inputValue);
            derivativeFilter.init(temp_precise_t(0.0));
        }
        failedReadCount = 0;
    }

    bool tooManyFailedReads = false;

    if(validSensor){ // only update internal filters and inputError if input sensor is valid
        inputFilter.add(state.inputValue);

        if(validInputSetting){
            if(previousInputSetting.isDisabledOrInvalid()){
                previousInputSetting = state.inputSetting;
            }
            temp_precise_t previousError = inputFilter.readPrevOutput() - previousInputSetting;
            temp_precise_t currentError = inputFilter.readOutput() - state.inputSetting;
            temp_precise_t delta = currentError - previousError;
            previousInputSetting = state.inputSetting;

            state.error = currentError; // store input error, as temp_t, instead of temp_precise_t

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
            state.derivative = derivativeFilter.readOutput() >> uint8_t(10);
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

    if(!settings.enabled || tooManyFailedReads || !validInputSetting){
        state.error = temp_t::invalid();
        state.p = 0.0;
        state.i = 0.0;
        state.d = 0.0;
    }
    else{
        // calculate PID parts.
        state.p = settings.kp * -state.error;
        state.i = (settings.ti != 0) ? (state.integral/settings.ti) : temp_long_t(0.0);
        state.d = -settings.kp * (state.derivative * settings.td);
    }

    if(!settings.enabled){
        return;
    }

    temp_long_t pidResult = temp_long_t(state.p) + temp_long_t(state.i) + temp_long_t(state.d);

    state.outputSetting = pidResult;

    // try to set the output to the desired setting
    output.set(state.outputSetting);

    // get the clipped setting from the actuator
    temp_long_t achievedSetting = output.setting();

    // get the actually achieved value from the actuator (actuator may not reach setting)
    state.outputValue = output.value();

    if(settings.ti == 0){ // 0 has been chosen to indicate that the integrator is disabled. This also prevents divide by zero.
        state.integral = 0.0;
    }
    else{
        // update integral with anti-windup back calculation
        // pidResult - output is zero when actuator is not saturated

        temp_long_t antiWindup(temp_long_t::base_type(0));

        state.integral = state.integral + state.p;

        if(pidResult != temp_long_t(achievedSetting)){
            // clipped to actuator min or max set in target actuator
            // calculate anti-windup from setting instead of actual value, so it doesn't dip under the maximum
            antiWindup = pidResult - achievedSetting;
            antiWindup *= 3; // Anti windup gain is 3
            // make sure anti-windup is at least p when clipping to prevent further windup
            antiWindup = (state.p > temp_long_t(0.0) && antiWindup < state.p) ? state.p : antiWindup;
            antiWindup = (state.p < temp_long_t(0.0) && antiWindup > state.p) ? state.p : antiWindup;
        }
        else {
            temp_t achievedOutput = output.value();
            if(!achievedOutput.isDisabledOrInvalid()){
                // only apply anti-windup when it is possible to read back the actual value
                // Actuator could be not reaching set value due to physics or limits in its target actuator
                // Get the actual achieved value in actuator. This could differ due to slowness time/mutex limits

                // Anti windup gain is 3
                antiWindup = (pidResult - temp_long_t(state.outputValue));
                antiWindup *= 3.0;

                // Disable anti-windup if integral part dominates. But only if it counteracts p.
                if(antiWindup.sign() == state.p.sign()){
                    if(settings.kp.sign() == -1 && state.i < state.p+state.p+state.p){
                        antiWindup = 0.0;
                    }
                    else if( state.i > state.p+state.p+state.p ){
                        antiWindup = 0.0;
                    }
                }
            }
        }
        temp_long_t reducedIntegral = state.integral - antiWindup;
        if(state.integral.sign() * reducedIntegral.sign() == 1){
            if(state.integral.sign() * antiWindup.sign() == 1){
                // only apply anti-windup if it will bring the PID result closer to zero
                state.integral = reducedIntegral;
            }
        }
        else{
            state.integral = 0.0; // set to zero if crossing zero due to anti-windup
        }
    }
}
