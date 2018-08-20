/*
 * Copyright 2015 BrewPi Elco Jacobs / Matthew McGowan
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



#include "ActuatorTimeLimited.h"

#include "Ticks.h"

void ActuatorTimeLimited::setState(const State & newState, const int8_t &priority, const ticks_seconds_t & now)
{
    State oldState = state;

    if (oldState == State::Active && newState == State::Inactive){
        if (timeSinceToggle() <= minOnTime){
            newState = State::Active;    // do not turn off before minOnTime has passed
            // use <= because stored value is truncated in divide from milliseconds to seconds
        }
    }

    if (oldState == State::Inactive && newState == State::Active){
        if (timeSinceToggle() <= minOffTime){
            newState = State::Inactive;    // do not turn on before minOffTime has passed
        }
    }

    if (oldState != newState){
        target.setState(newState, priority);
        state = target.getState();

        if(oldState != state && state != State::Unknown){
            toggleTime = now();
        }
    }
}

update_t ActuatorTimeLimited::update(const update_t & t)
{
    target.update(t);
    state = target.getState(); // make sure state is always up to date with target
    if (state == State::Active && (timeSinceToggle() >= maxOnTime)){
        setState(State::Inactive);
    }
    return t + 1000;
}

ticks_seconds_t ActuatorTimeLimited::timeSinceToggle(const ticks_seconds_t & now) const
{
    return timeSinceSeconds(now, toggleTime);
}
