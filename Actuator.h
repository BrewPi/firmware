/*
 * Copyright 2013 Matthew McGowan
 *
 * This file is part of BrewPi.
 * 
 * Actuator.h
 *
 * Created: 19/02/2013 18:23:38
 *  Author: mat
 */ 

#pragma once

#include "Arduino.h"
#include "FastDigitalPin.h"

#define ACTUATOR_VIRTUAL 1

#if ACTUATOR_VIRTUAL==1	
	#define ACTUATOR_METHOD virtual
	#define ACTUATOR_METHOD_IMPL =0
	#define ACTUATOR_BASE_CLASS_DECL : public Actuator
#else
	#define ACTUATOR_METHOD inline
	#define ACTUATOR_METHOD_IMPL {}
	#define ACTUATOR_BASE_CLASS_DECL
#endif


/*
 * An actuator simply turns something on or off.                        
 */

class Actuator
{
	public:
	//ACTUATOR_METHOD bool isActive() ACTUATOR_METHOD_IMPL;
	ACTUATOR_METHOD void setActive(bool active) ACTUATOR_METHOD_IMPL;
	ACTUATOR_METHOD void activate()  { setActive(true); }
	ACTUATOR_METHOD void deactivate() { setActive(false); }
};

/*
 * An actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator ACTUATOR_BASE_CLASS_DECL
{
public:
	ValueActuator() : state(false) {}
	ValueActuator(bool initial) : state(initial) {}

	ACTUATOR_METHOD void setActive(bool active) { state = active; }

private:
	bool state;	
};


template<uint8_t pin, bool invert> 
class DigitalPinActuatorInline ACTUATOR_BASE_CLASS_DECL
{
	public:
	DigitalPinActuatorInline()
	{
		fastPinMode(pin, OUTPUT);
	}
	
	ACTUATOR_METHOD void setActive(bool active) {		
		fastDigitalWrite(pin, active^invert ? HIGH : LOW);
	}

};

class DigitalPinActuator ACTUATOR_BASE_CLASS_DECL
{
	private:
	int pin;
	bool active;
	bool inverse;
	
	public:
	DigitalPinActuator(int _pin, bool _inverse=false, bool _initiallyActive=false) : pin(_pin), inverse(_inverse)
	{
		pinMode(pin, OUTPUT);
		setActive(_initiallyActive);
	}
	
	ACTUATOR_METHOD void setActive(bool active) {
		this->active = active;
		digitalWrite(pin, active ? HIGH : LOW);
	}

	ACTUATOR_METHOD bool isActive() {
		return active;								
	}

};
