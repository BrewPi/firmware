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



#pragma once

#include "temperatureFormats.h"
#include "FilterCascaded.h"
#include "TempSensorBasic.h"
#include "ActuatorInterfaces.h"
#include "SetPoint.h"
#include "defaultDevices.h"
#include "ControllerMixins.h"
#include "ControlLib.h"

CONTROL_LIB_BEGIN


class Pid : public PidMixin
{

    public:
        Pid(TempSensorBasic * input, ActuatorRange * output, SetPoint * setPoint);

        Pid() : Pid(defaultTempSensorBasic(), defaultLinearActuator(), defaultSetPoint()){}

        Pid(const Pid & orig);

        ~Pid() = default;

        void init();

        void update();

        void setConstants(temp_long_t kp,
                          uint16_t ti,
                          uint16_t td);

        void setFiltering(uint8_t b);

        uint8_t getFiltering();

        void setInputFilter(uint8_t b);

        void setDerivativeFilter(uint8_t b);

        bool setInputSensor(TempSensorBasic * s);

        TempSensorBasic * getInputSensor(){
            return inputSensor;
        }

        bool setOutputActuator(ActuatorRange * a);

        ActuatorRange * getOutputActuator(){
            return outputActuator;
        }

        void setSetPoint(SetPoint * s){
            setPoint = s;
        }

        SetPoint * getSetPoint(){
            return setPoint;
        }

        void setActuatorIsNegative(bool setting){
            actuatorIsNegative = setting;
        }

        void enable(){
            enabled = true;
        }

        void disable(bool turnOffOutputActuator){
            enabled = false;
            inputError = decltype(inputError)::base_type(0);
            p = decltype(p)::base_type(0);
            i = decltype(i)::base_type(0);
            d = decltype(d)::base_type(0);
            if(turnOffOutputActuator){
                outputActuator -> setValue(0.0);
            }
        }

        /*
        uint16_t getOutputLag(){ return outputLag; };

        temp_precise getMaxDerivative(){ return maxDerivative; };

        bool isTuning(){ return tuning; };

        void tune(temp output, temp previousOutput);

        void setAutoTune(bool doTune) { autotune = doTune; };
        */

    protected:
        ActuatorRange *   outputActuator;
        TempSensorBasic * inputSensor;
        SetPoint *        setPoint;
        temp_long_t       Kp;    // proportional gain
        uint16_t          Ti;    // integral time constant
        uint16_t          Td;    // derivative time constant
        temp_t            inputError;
        temp_long_t       p;
        temp_long_t       i;
        temp_long_t       d;
        temp_precise_t    derivative;
        temp_long_t       integral;
        FilterCascaded    inputFilter;
        FilterCascaded    derivativeFilter;
        uint8_t           failedReadCount;
        bool              actuatorIsNegative; // if true, the actuator lowers the input, e.g. a cooler
        bool              enabled;
        /*
        bool              autotune; // auto tuning enabled
        bool              tuning; // tuning in this step response
        uint16_t          outputLag;
        temp_precise      maxDerivative;
        */

    friend class PidMixin;
};

CONTROL_LIB_END
