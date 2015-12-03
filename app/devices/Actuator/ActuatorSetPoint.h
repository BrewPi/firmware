/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#include "ActuatorInterfaces.h"
#include "SetPoint.h"
#include "TempSensorBasic.h"
#include "defaultDevices.h"
 
/*
 * A linear actuator that sets a setpoint to reference setpoint + actuator value
 */
class ActuatorSetPoint : private ActuatorBottom, public ActuatorRange
{
public:
    ActuatorSetPoint(SetPoint * targSetPoint = defaultSetPoint(), // set point to manipulate
                     TempSensorBasic * targSensor = defaultTempSensorBasic(), // sensor to go with target setpoint
                     SetPoint * refSetPoint = defaultSetPoint(), //set point to offset from
                     temp_t mini = temp_t::min(), // minimum actuator value (targ - ref)
                     temp_t maxi = temp_t::max()) :  // maximum actuator value
        targetSetPoint(targSetPoint),
        targetSensor(targSensor),
        referenceSetPoint(refSetPoint),
        minimum(mini),
        maximum(maxi){
    }

    void setValue(temp_t const& val) {
        temp_t offset = val;
        if(offset < minimum){
            offset = minimum;
        }
        else if(offset > maximum){
            offset = maximum;
        }
        targetSetPoint->write(referenceSetPoint->read() + offset);
    }

    // getValue returns difference between sensor and reference, because that is the actual actuator value.
    // By returning the actually achieved value, instead of the difference between the setpoints,
    // a PID can read back the actual actuator value and perform integrator anti-windup
    temp_t getValue() const{
        return targetSensor->read() - referenceSetPoint->read();
    }

    temp_t min() const{
        return minimum;
    }

    temp_t max() const{
        return maximum;
    }

    void setMin(temp_t min){
        minimum = min;
    }

    void setMax(temp_t max){
        maximum = max;
    }

    virtual void update(){}; //no actions required

    void serialize(JSON::Adapter& adapter){
        JSON::Class root(adapter, "ActuatorSetPoint");
        JSON_E(adapter, targetSetPoint);
        JSON_E(adapter, targetSensor);
        JSON_E(adapter, referenceSetPoint);
        JSON_E(adapter, minimum);
        JSON_T(adapter, maximum);
    }


private:
    SetPoint * targetSetPoint;
    TempSensorBasic * targetSensor;
    SetPoint * referenceSetPoint;
    temp_t minimum;
    temp_t maximum;
};
