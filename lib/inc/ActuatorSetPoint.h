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
#include "TempSensorInterface.h"
#include "defaultDevices.h"
#include "ControllerMixins.h"
#include "RefTo.h"

/*
 * A linear actuator that sets a setpoint to reference setpoint + actuator value
 */
class ActuatorSetPoint final : public ActuatorRangeInterface, public ActuatorSetPointMixin
{
public:
    ActuatorSetPoint(std::function<Interface* ()> _targSetPoint, // set point to manipulate
                     std::function<Interface* ()> _targSensor, // sensor to go with target setpoint
                     std::function<Interface* ()> _refSetPoint, //set point to offset from
                     temp_t mini = temp_t::min(), // minimum actuator value (targ - ref)
                     temp_t maxi = temp_t::max()) :  // maximum actuator value
        minimum(mini),
        maximum(maxi)
    {
        targetSetPoint.setLookup(_targSetPoint);
        targetSensor.setLookup(_targSensor);
        referenceSetPoint.setLookup(_refSetPoint);
    }
    ~ActuatorSetPoint() = default;

    /**
     * Accept function for visitor pattern
     * @param dispatcher Visitor to process this class
     */
    void accept(VisitorBase & v) final {
    	v.visit(*this);
    }

    void setValue(temp_t const& val) override final {
        temp_t offset = val;
        if(offset < minimum){
            offset = minimum;
        }
        else if(offset > maximum){
            offset = maximum;
        }
        targetSetPoint().write(referenceSetPoint().read() + offset);
    }

    temp_t getValue() const override final {
        return targetSetPoint().read() - referenceSetPoint().read();
    }

    // getValue returns difference between sensor and reference, because that is the actual actuator value.
    // By returning the actually achieved value, instead of the difference between the setpoints,
    // a PID can read back the actual actuator value and perform integrator anti-windup
    temp_t readValue() const override final{
        temp_t targetTemp = targetSensor().read();
        if(targetTemp.isDisabledOrInvalid()){
            return temp_t::invalid();
        }
        return targetSensor().read() - referenceSetPoint().read();
    }

    temp_t min() const override final {
        return minimum;
    }

    temp_t max() const override final {
        return maximum;
    }

    void setMin(temp_t min) {
        minimum = min;
    }

    void setMax(temp_t max) {
        maximum = max;
    }

    void update() override final {}; //no actions required
    void fastUpdate() override final {}; //no actions required

private:
    RefTo<SetPointInterface> targetSetPoint;
    RefTo<TempSensorInterface> targetSensor;
    RefTo<SetPointInterface> referenceSetPoint;
    temp_t minimum;
    temp_t maximum;

    friend class ActuatorSetPointMixin;
};
