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
#include "ControllerMixins.h"
#include "ProcessValue.h"

/*
 * A linear actuator that sets a setpoint to reference setpoint + actuator value
 */
class ActuatorOffset final : public ActuatorAnalog, public ActuatorOffsetMixin
{
public:
    ActuatorOffset(ProcessValue & _target, // process value to manipulate
                     ProcessValue & _reference, // process value to offset from
                     temp_t _min = temp_t::min(), // minimum actuator value (targ - ref)
                     temp_t _max = temp_t::max()) :  // maximum actuator value
        target(_target),
        reference(_reference),
        offset(0.0),
        minimum(_min),
        maximum(_max),
        useReferenceSetting(true)
    {
    }
    ~ActuatorOffset() = default;

    /**
     * Accept function for visitor pattern
     * @param dispatcher Visitor to process this class
     */
    void accept(VisitorBase & v) final {
    	v.visit(*this);
    }

    temp_t readReference() const {
        return (useReferenceSetting) ? reference.setting() : reference.value();
    }

    void set(temp_t const& val) override final {
        if(val < minimum){
            offset = minimum;
        }
        else if(val> maximum){
            offset = maximum;
        }
        else{
            offset = val;
        }
        update();
    }

    temp_t setting() const override final {
        return offset;
    }

    // value() returns the actually achieved offset
    // By returning the actually achieved value, instead of the difference between the setpoints,
    // a PID can read back the actual actuator value and perform integrator anti-windup
    temp_t value() const override final{
        temp_t targetValue = target.value();
        temp_t referenceValue = readReference();

        if(targetValue.isDisabledOrInvalid() || referenceValue.isDisabledOrInvalid()){
            return temp_t::invalid();
        }
        return targetValue - referenceValue;
    }

    temp_t min() const {
        return minimum;
    }

    temp_t max() const {
        return maximum;
    }

    void setMin(temp_t min) {
        minimum = min;
    }

    void setMax(temp_t max) {
        maximum = max;
    }

    void setReferenceSettingOrValue(bool useSetting) {
        useReferenceSetting = useSetting;
    }

    void update() override final {
        temp_t referenceValue = readReference();
        if(referenceValue.isDisabledOrInvalid()){
            target.set(temp_t::invalid());
            return;
        }
        temp_t targetValue = referenceValue + offset;
        target.set(targetValue);
    };
    void fastUpdate() override final {}; //no actions required

private:
    ProcessValue & target; // process value to manipulate
    ProcessValue & reference; // process value to offset from
    temp_t offset;
    temp_t minimum;
    temp_t maximum;
    bool useReferenceSetting; // use setting of reference and not actual value if true (default)

    friend class ActuatorOffsetMixin;
};
