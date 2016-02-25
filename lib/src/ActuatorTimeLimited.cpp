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

void ActuatorTimeLimited::setActive(bool newState)
{
    bool oldState = state;

    if (oldState && !newState){
        if (timeSinceToggle() <= minOnTime){
            newState = true;    // do not turn off before minOnTime has passed
            // use <= because stored value is truncated in divide from milliseconds to seconds
        }
    }

    if (!oldState && newState){
        if (timeSinceToggle() <= minOffTime){
            newState = false;    // do not turn on before minOffTime has passed
        }
    }

    if (oldState != newState){
        target -> setActive(newState);
        state = target -> isActive();

        if(oldState != state){
            toggleTime = ticks.seconds();
        }
    }
}

void ActuatorTimeLimited::update()
{
    target->update();
    if (state && (timeSinceToggle() >= maxOnTime)){
        setActive(false);
    }
}

ticks_seconds_t ActuatorTimeLimited::timeSinceToggle() const
{
    return ticks.timeSinceSeconds(toggleTime);
}
