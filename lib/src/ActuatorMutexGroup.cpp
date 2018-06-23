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
#include "ActuatorMutexDriver.h"
#include "ActuatorInterfaces.h"
#include "Ticks.h"

ActuatorMutexGroup::~ActuatorMutexGroup(){
	for(auto & ap : actuatorPriorities ){
		ap.actuator->signalDeletedMutexGroup(this);
	}
}

void ActuatorMutexGroup::registerActuator(ActuatorMutexDriver * act){
    actuatorPriorities.push_back(ActuatorPriority(act, -1));
}

void ActuatorMutexGroup::unRegisterActuator(ActuatorMutexDriver * act){
	auto it = actuatorPriorities.begin();
	auto end = actuatorPriorities.end();
	for(;it < end; it++){
		if(it->actuator == act){
			break;
		}
	}
	if(it != end){
		actuatorPriorities.erase(it);
	}
}

bool ActuatorMutexGroup::request(ActuatorMutexDriver * requester, ActuatorDigital::State newState, int8_t newPriority){
    // loop over all actuators to see if my request has highest priority
    // and if no other actuators are active

    bool otherActuatorActive = false;
    int8_t highestPriority = -1; // highest priority in the list (excluding requester)
    bool requestHonored = true;

    if(newState != ActuatorDigital::State::Active){
        newPriority = -1; // set requester priority to -1, because its not waiting to go active anymore
    }

    for(auto & ap : actuatorPriorities ){
    	ActuatorDigital::State actuatorState = ap.actuator->getState();
        if(ap.actuator == requester){
        	ap.priority = newPriority; // update my own priority
        }
        if(ap.priority > highestPriority){
        	highestPriority = ap.priority;
        }
        if(actuatorState == ActuatorDigital::State::Active){
            lastActiveTime = ticks.millis();
            lastActiveActuator = ap.actuator;
            if(ap.actuator != requester){
            	otherActuatorActive = true;
            }
        }
    }
	requestHonored &= !otherActuatorActive;
	requestHonored &= newPriority >= highestPriority;
	requestHonored &= !(getWaitTime() > 0) || lastActiveActuator == requester;

	// always allow false, otherwise allow when no one else is active and me has highest priority
    requestHonored = requestHonored || newState == ActuatorDigital::State::Inactive;

    return requestHonored;
}

void ActuatorMutexGroup::cancelRequest(ActuatorMutexDriver * requester){
    request(requester, ActuatorDigital::State::Inactive, -1);
}

void ActuatorMutexGroup::setDeadTime(ticks_millis_t time){
    deadTime = time;
    if(lastActiveTime == 0){
        // lastActive has not been set yet, make sure deadTime does not get in the way of first request
        lastActiveTime = ticks.millis() - deadTime;
    }
}

ticks_millis_t ActuatorMutexGroup::getWaitTime(){
    ticks_millis_t elapsed = ticks.millis() - lastActiveTime;
    if(elapsed >= deadTime){
        return 0;
    }
    else{
        return (deadTime - elapsed);
    }
}

// update decreases all priorities by 1, so that old requests lose their priority automatically
void ActuatorMutexGroup::update(){
	for(auto & ap : actuatorPriorities ){
        if(ap.priority > -1){
            ap.priority--;
        }
        if(ap.actuator->getState() == ActuatorDigital::State::Active){
            lastActiveTime = ticks.millis();
            lastActiveActuator = ap.actuator;
        }
    }
}
