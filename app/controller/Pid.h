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

#include "newTemperatureFormats.h"
#include "FilterCascaded.h"
#include "TempSensorBasic.h"
#include "Actuator.h"

class Pid
{
    public:
        Pid(BasicTempSensor * input,
            LinearActuator * output);

        Pid(const Pid & orig);

        virtual ~Pid();

        void init();

        void update();

        void setSetPoint(temp val);

        void setConstants(temp kp,
                          temp ki,
                          temp kd);

        void setFiltering(uint8_t b);

        uint8_t getFiltering();

        void setInputFilter(uint8_t b);

        void setDerivativeFilter(uint8_t b);

        void setMinMax(temp min,
                       temp max);

        bool setInputSensor(BasicTempSensor * s);

        bool setOutputActuator(LinearActuator * a);

        uint16_t getOutputLag(){ return outputLag; };

        temp_precise getMaxDerivative(){ return maxDerivative; };

    protected:
        LinearActuator *  outputActuator;
        BasicTempSensor * inputSensor;
        temp              Kp;    // proportional gain
        temp              Ki;    // integral gain
        temp              Kd;    // derivative gain
        temp              Ka;    // integrator anti windup gain
        temp              min;
        temp              max;
        temp              setPoint;
        temp              p;
        temp              i;
        temp              d;
        temp              error;
        temp_precise      derivative;
        temp_long         integral;
        FilterCascaded    inputFilter;
        FilterCascaded    derivativeFilter;
        uint8_t           failedReadCount;

        bool              autotune; // auto tuning enabled
        bool              tuning; // tuning in this step response
        uint16_t          outputLag;
        temp_precise      maxDerivative;
};
