/* 
 * File:   ArduinoActuator.h
 * Author: mat
 *
 * Created on 19 August 2013, 20:32
 */

#pragma once

#include "Actuator.h"

class DigitalPinActuator ACTUATOR_BASE_CLASS_DECL
{
	private:
	bool invert;
	uint8_t pin;
	public:
	DigitalPinActuator(uint8_t pin, bool invert) {
		this->invert = invert;
		this->pin = pin;
		setActive(false);
		pinMode(pin, OUTPUT);
	}
	
	inline ACTUATOR_METHOD void setActive(bool active) {
		digitalWrite(pin, active^invert ? HIGH : LOW);
	}
	
	bool isActive() { return ((digitalRead(pin)!=LOW) ^ invert); }
};
