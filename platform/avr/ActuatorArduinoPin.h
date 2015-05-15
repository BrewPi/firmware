/* 
 * File:   ArduinoActuator.h
 * Author: mat
 *
 * Created on 19 August 2013, 20:32
 */

#pragma once

#include <stdint.h>
#include "Platform.h"
#include "Actuator.h"
#include "FastDigitalPin.h"

template<uint8_t pin, bool invert>
class DigitalConstantPinActuator ACTUATOR_BASE_CLASS_DECL
{
	private:
	bool active;
	
	public:
	DigitalConstantPinActuator() : active(false)
	{
		setActive(false);
		fastPinMode(pin, OUTPUT);
	}
	
	inline ACTUATOR_METHOD void setActive(bool active) {
		this->active = active;
		fastDigitalWrite(pin, active^invert ? HIGH : LOW);
	}
	
	bool isActive() { return active; }

};

