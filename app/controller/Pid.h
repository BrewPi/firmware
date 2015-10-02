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
#include "Actuator.h"
#include "SetPoint.h"

class Pid
{
    public:
        Pid(BasicTempSensor * input,
            LinearActuator * output,
            SetPoint * setPoint);

        Pid(const Pid & orig);

        virtual ~Pid();

        void init();

        void update();

        void setConstants(temp_long_t kp,
                          temp_long_t ki,
                          temp_long_t kd);

        void setFiltering(uint8_t b);

        uint8_t getFiltering();

        void setInputFilter(uint8_t b);

        void setDerivativeFilter(uint8_t b);

        void setMinMax(temp_t min,
                       temp_t max);

        bool setInputSensor(BasicTempSensor * s);

        BasicTempSensor * getInputSensor(){
            return inputSensor;
        }

        bool setOutputActuator(LinearActuator * a);

        Actuator * getOutputActuator(){
            return outputActuator;
        }

        void setSetPoint(SetPoint * s){
            setPoint = s;
        }

        SetPoint * getSetPoint(){
            return setPoint;
        }

        /*
        uint16_t getOutputLag(){ return outputLag; };

        temp_precise getMaxDerivative(){ return maxDerivative; };

        bool isTuning(){ return tuning; };

        void tune(temp output, temp previousOutput);

        void setAutoTune(bool doTune) { autotune = doTune; };
        */

//    protected:
    public:
        LinearActuator *  outputActuator;
        BasicTempSensor * inputSensor;
        SetPoint *        setPoint;
        temp_long_t       Kp;    // proportional gain
        temp_long_t       Ki;    // integral gain
        temp_long_t       Kd;    // derivative gain
        temp_t            min;
        temp_t            max;
        temp_t            inputError;
        temp_long_t       p;
        temp_long_t       i;
        temp_long_t       d;
        temp_precise_t    derivative;
        temp_long_t       integral;
        FilterCascaded    inputFilter;
        FilterCascaded    derivativeFilter;
        uint8_t           failedReadCount;

        /*
        bool              autotune; // auto tuning enabled
        bool              tuning; // tuning in this step response
        uint16_t          outputLag;
        temp_precise      maxDerivative;
        */
};
