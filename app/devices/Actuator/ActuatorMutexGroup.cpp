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

#include "ActuatorMutexGroup.h"
#include "Ticks.h"
#include "Actuator.h"
#include <vector>

void ActuatorMutexGroup::registerActuator(ActuatorDigital * act, int8_t prio){
    ActuatorPriority ap = {act, prio};
    actuatorPriorities.push_back(ap);
}

uint8_t ActuatorMutexGroup::find(ActuatorDigital * act){
    for (unsigned i=0; i<actuatorPriorities.size(); ++i){
        if(actuatorPriorities[i].actuator == act){
            return i;
        }
    }
    return -1;
}

void ActuatorMutexGroup::unRegisterActuator(uint8_t index){
    actuatorPriorities.erase(actuatorPriorities.begin() + index);
}

bool ActuatorMutexGroup::requestActive(ActuatorDigital * requester, int8_t newPriority){
    // loop over all actuators to see if my request has highest priority
    // and if no other actuators are active

    bool requestHonored = true;
    ActuatorPriority * me = 0;

    for (uint8_t i=0; i<actuatorPriorities.size(); ++i){
        ActuatorPriority other = actuatorPriorities[i];

        if(other.actuator == requester){
            other.priority = newPriority; // update my own priority
            me = &actuatorPriorities[i];
        }
        else{
            if(other.priority > newPriority){
                requestHonored = false;
            }
            if(other.actuator->isActive()){
                requestHonored = false;
            }
        }
        if(me && !requestHonored){
            break;
        }
    }
    if(!me){ // I was not in the list
        registerActuator(requester, newPriority);
        me = &actuatorPriorities.back();
    }
    if(getWaitTime() > 0){
        requestHonored = false; // dead time has not passed
    }
    if(requestHonored){
        lastActiveTime = ticks.millis();
        me->priority = 0; // when the request is honored, set the priority to zero. Requester can re-request with higher prio.
    }
    return requestHonored;
}

void ActuatorMutexGroup::setDeadTime(ticks_millis_t time){
    deadTime = time;
    if(lastActiveTime == 0){
        // lastActive has not been set yet, make sure deadTime does not get in the way of first request
        lastActiveTime = ticks.millis() - deadTime;
    }
}

ticks_millis_t ActuatorMutexGroup::getWaitTime(){
    ticks_millis_t elapsed = ticks.timeSince(lastActiveTime);
    if(elapsed >= deadTime){
        return 0;
    }
    else{
        return (deadTime - elapsed);
    }
}

// update decreases all priorities by 1 and removes old requests.
void ActuatorMutexGroup::update(){
    for (uint8_t i=0; i<actuatorPriorities.size(); ++i){
        if(actuatorPriorities[i].priority == -128){
            unRegisterActuator(i); // stale request, remove actuator from list
        }
        else{
            actuatorPriorities[i].priority--;
        }
    }
}
