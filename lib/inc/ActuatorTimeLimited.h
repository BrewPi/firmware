/*
 * Copyright 2013 Matthew McGowan
 * Copyright 2013 BrewPi/Elco Jacobs.
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

#include <stdint.h>

#include "ActuatorInterfaces.h"
#include "Ticks.h"
#include "ControllerMixins.h"
#include "RefTo.h"

class ActuatorTimeLimited final : public ActuatorDigital, public ActuatorTimeLimitedMixin
{
public:
    ActuatorTimeLimited(ActuatorDigital & _target,
            ticks_seconds_t   _minOnTime = 120,
            ticks_seconds_t   _minOffTime = 180,
            ticks_seconds_t   _maxOnTime = UINT16_MAX) :
        target(_target),
        minOnTime(_minOnTime),
        maxOnTime(_maxOnTime),
        minOffTime(_minOffTime),
        toggleTime(0)
    {
        state = State::Inactive;
    }

    ~ActuatorTimeLimited() = default;

    /**
     * Accept function for visitor pattern
     * @param dispatcher Visitor to process this class
     */
    virtual void accept(VisitorBase & v) override final {
    	v.visit(*this);
    }

    void setState(State state, int8_t priority = 127) override final;

    State getState() const override final
    {
        return state;
    }

    virtual void update() override final;

    virtual void fastUpdate() override final {} // time limit is in seconds, no fast update needed


    void setTimes(ticks_seconds_t   _minOnTime,
                  ticks_seconds_t   _minOffTime,
                  ticks_seconds_t   _maxOnTime = UINT16_MAX){
        minOnTime = _minOnTime;
        minOffTime = _minOffTime;
        maxOnTime = _maxOnTime;
    }
    ticks_seconds_t timeSinceToggle(void) const;

private:
    ActuatorDigital & target;
    ticks_seconds_t        minOnTime;
    ticks_seconds_t        maxOnTime;
    ticks_seconds_t        minOffTime;
    ticks_seconds_t        toggleTime;
    // shadow copy to prevent sending unnecessary updates to target
    State                  state;

    friend class ActuatorTimeLimitedMixin;
};
