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



#include "newTemperatureFormats.h"
#include "FilterCascaded.h"
#include "TempSensor.h"
#include "ActuatorPwm.h"

#pragma once

class Pid
{
    public:
        Pid(TempSensor * input,
            Actuator *   output);

        Pid(const Pid & orig);

        virtual ~Pid();

        void init();

        void update();

        void setSetPoint(fixed7_9 val);

        void setConstants(fixed7_9 kp,
                          fixed7_9 ki,
                          fixed7_9 kd);

        void setInputFilter(uint8_t b);

        void setDerivativeFilter(uint8_t b);

        void setDoubleDerivativeFilter(uint8_t b);

        void setMinMax(fixed7_9 min,
                       fixed7_9 max);

    private:
        Actuator *        outputActuator;
        BasicTempSensor * inputSensor;
        temp_diff         Kp;    // proportional gain
        temp_diff         Ki;    // integral gain
        temp_diff         Kd;    // derivative gain
        temp_diff         Ka;    // integrator anti windup gain
        temp_diff         min;
        temp_diff         max;
        temp              setPoint;
        temp_diff         p;
        temp_diff         i;
        temp_diff         d;
        temp_diff         error;
        temp_precise      derivative;
        temp_precise      doubleDerivative;
        temp_long         integral;
        uint8_t           integralUpdateCounter;
        FilterCascaded    inputFilter;
        FilterCascaded    derivativeFilter;
        FilterCascaded    doubleDerivativeFilter;
        uint8_t           failedReadCount;
};
