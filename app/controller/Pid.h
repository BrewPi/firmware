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
#include "Nameable.h"
#include "json_writer.h"

class Pid : public Nameable
{
    public:
        Pid(TempSensorBasic * input, ActuatorRange * output, SetPoint * setPoint);

        Pid() : Pid(defaultTempSensorBasic(), defaultLinearActuator(), defaultSetPoint()){}

        Pid(const Pid & orig);

        virtual ~Pid();

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
            inputError = 0;
            p = 0;
            i = 0;
            d = 0;
            if(turnOffOutputActuator){
                outputActuator -> setValue(0.0);
            }
        }


        void serialize(JSON::Adapter& adapter){
            JSON::Class root(adapter, "Pid");
            std::string name(getName()); // get name as std string for json_writer
            JSON_E(adapter, name);
            JSON_E(adapter, enabled);
            JSON_E(adapter, setPoint);
            JSON_E(adapter, inputSensor);

            JSON_E(adapter, inputError);
            JSON_E(adapter, Kp);
            JSON_E(adapter, Ti);
            JSON_E(adapter, Td);
            JSON_E(adapter, p);
            JSON_E(adapter, i);
            JSON_E(adapter, d);

            JSON_E(adapter, actuatorIsNegative);
            JSON_T(adapter, outputActuator);
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

    friend class TempControl;
};
