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



#include "TemperatureFormats.h"
#include "FilterCascaded.h"
#include "TempSensor.h"
#include "ActuatorPwm.h"

#pragma once

class Pid
{
    public:
        Pid(TempSensor * input, Actuator * output);

        Pid(const Pid & orig);

        virtual ~Pid();

        void init();

        fixed7_9 update(fixed7_9 input);

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
        fixed7_9         Kp; // proportional gain
        fixed7_9         Ki; // integral gain
        fixed7_9         Kd; // derivative gain
        fixed7_9         Ka; // integrator anti windup gain
        fixed7_9         min;
        fixed7_9         max;
        fixed7_9         setPoint;
        fixed7_9         output;
        fixed7_9         p;
        fixed7_9         i;
        fixed7_9         d;
        fixed7_9         error;
        fixed7_9         derivative;
        fixed7_9         doubleDerivative;
        long_temperature integral;
        uint8_t          integralUpdateCounter;
        FilterCascaded   inputFilter;
        FilterCascaded   derivativeFilter;
        FilterCascaded   doubleDerivativeFilter;
};
