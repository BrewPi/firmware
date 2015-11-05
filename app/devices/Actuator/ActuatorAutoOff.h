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

#include "ActuatorInterfaces.h"
#include <stdint.h>

class AutoOffActuator : public ActuatorDigital {
	
public:
	AutoOffActuator(uint16_t timeout, ActuatorDigital* target) {
		this->timeout = timeout;
		this->target = target;
		active = target->isActive();
	}
	
	void setActive(bool active);
	
	bool isActive() {
		return active; //target->isActive(); - this takes 20 bytes more
	}
	
    void update();

    void write(uint8_t val){};
        
private:
	uint16_t lastActiveTime;
	uint16_t timeout;
	ActuatorDigital* target;
	bool active;
};
