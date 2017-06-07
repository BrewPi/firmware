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
#include "TempSensor.h"
#include "ActuatorInterfaces.h"
#include "SetPoint.h"
#include "defaultDevices.h"
#include "ControllerMixins.h"
#include "ControllerInterface.h"
#include "ProcessValue.h"

class Pid final : public ControllerInterface, public PidMixin
{

    public:
        Pid(ProcessValue & _input,
            ProcessValue & _output);
        ~Pid() = default;

        /**
         * Accept function for visitor pattern
         * @param dispatcher Visitor to process this class
         */
        void accept(VisitorBase & v) final {
        	v.visit(*this);
        }

        void init();

        void update();

        void setConstants(temp_long_t kp,
                          uint16_t ti,
                          uint16_t td);

        void setFiltering(uint8_t b);

        uint8_t getFiltering();

        void setInputFilter(uint8_t b);

        void setDerivativeFilter(uint8_t b);

        void setActuatorIsNegative(bool setting){
            actuatorIsNegative = setting;
        }

        void enable(){
            enabled = true;
        }

        void disable(bool turnOffOutput){
            enabled = false;
            inputError = decltype(inputError)::base_type(0);
            p = decltype(p)::base_type(0);
            i = decltype(i)::base_type(0);
            d = decltype(d)::base_type(0);
            if(turnOffOutput){
                output.set(0.0);
            }
        }

    protected:
        ProcessValue & input;
        ProcessValue & output;
        temp_long_t       Kp;    // proportional gain
        uint16_t          Ti;    // integral time constant
        uint16_t          Td;    // derivative time constant
        temp_long_t       p;
        temp_long_t       i;
        temp_long_t       d;
        temp_t            inputError;
        temp_precise_t    derivative;
        temp_long_t       integral;
        FilterCascaded    inputFilter;
        FilterCascaded    derivativeFilter;
        uint8_t           failedReadCount;
        bool              actuatorIsNegative; // if true, the actuator lowers the input, e.g. a cooler
        bool              enabled;

    private:
        // remember previous setpoint, to be able to take the derivative of the error, instead of the input
        temp_t            previousSetPoint;

    friend class TempControl;
    friend class PidMixin;
};
