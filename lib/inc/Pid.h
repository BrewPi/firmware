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
#include <cstring>

class Pid final : public ControllerInterface, public PidMixin
{

    public:
        Pid(ProcessValue & _input,
            ProcessValue & _output);
        ~Pid() = default;

        struct Settings {
            Settings() : kp(0.0), ti(0), td(0), enabled(false) {}
            ~Settings() = default;
            temp_long_t       kp;    // proportional gain
            uint16_t          ti;    // integral time constant
            uint16_t          td;    // derivative time constant
            bool              enabled;
        };
        struct State {
            State() : inputValue(0.0), inputSetting(0.0), outputValue(0.0), outputSetting(0.0),
                    p(0.0), i(0.0), d(0.0), integral(0.0), derivative(0.0), error(0.0){}
            ~State() = default;
            temp_t            inputValue;
            temp_t            inputSetting;
            temp_t            outputValue;
            temp_t            outputSetting;
            temp_long_t       p;
            temp_long_t       i;
            temp_long_t       d;
            temp_long_t       integral;
            temp_precise_t    derivative;
            temp_t            error; // last element for 32-bit alignment
        };


        /**
         * Accept function for visitor pattern
         * @param dispatcher Visitor to process this class
         */
        virtual void accept(VisitorBase & v) override final {
        	v.visit(*this);
        }

        void init();

        virtual void update() override final;

        void setConstants(temp_long_t kp,
                          uint16_t ti,
                          uint16_t td);


        uint8_t getInputFiltering()
        {
            return inputFilter.getFiltering();
        }

        uint8_t getDerivativeFiltering()
        {
            return derivativeFilter.getFiltering();
        }

        void setInputFiltering(uint8_t b)
        {
            inputFilter.setFiltering(b);
        }

        void setDerivativeFiltering(uint8_t b)
        {
            derivativeFilter.setFiltering(b);
        }

        void enable(){
            settings.enabled = true;
        }

        void disable(bool turnOffOutput){
            settings.enabled = false;
            state.error = 0.0;
            state.p = 0.0;
            state.i = 0.0;
            state.d = 0.0;
            if(turnOffOutput){
                output.set(0.0);
            }
        }

        void setSettings(Settings const & from){
            settings = from;
        }


        Settings const& getSettings(){
            return settings;
        }

        State const& getState(){
            return state;
        }

    protected:
        ProcessValue & input;
        ProcessValue & output;
        Settings settings;
        State state;

        FilterCascaded    inputFilter;
        FilterCascaded    derivativeFilter;

    private:
        // remember previous setpoint, to be able to take the derivative of the error, instead of the input
        uint8_t           failedReadCount;
        temp_t            previousInputSetting;

    friend class TempControl;
    friend class PidMixin;
};
