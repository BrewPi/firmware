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

#include "ActuatorAutoOff.h"
#include "Ticks.h"
#include "ControlLib.h"

CONTROL_LIB_BEGIN

void AutoOffActuator::setActive(bool active)
{
    this->active = active;
    target->setActive(active);
    if (active)
            lastActiveTime = ticks.seconds();
}

void AutoOffActuator::update() {
    if (ticks.timeSinceSeconds(lastActiveTime)>=timeout)
            setActive(false);
}


CONTROL_LIB_END
