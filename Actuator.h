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

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "brewpi_avr.h"
#include "FastDigitalPin.h"

#define ACTUATOR_VIRTUAL 1

#if ACTUATOR_VIRTUAL
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
	ACTUATOR_METHOD void setActive(bool active) ACTUATOR_METHOD_IMPL;
#if ACTUATOR_VIRTUAL
	virtual ~Actuator() {}
#endif		
		
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
	ACTUATOR_METHOD bool isActive() { return state; }

private:
	bool state;	
};

template<uint8_t pin, bool invert> 
class DigitalConstantPinActuator ACTUATOR_BASE_CLASS_DECL
{
	public:
	DigitalConstantPinActuator()
	{
		fastPinMode(pin, OUTPUT);
		fastDigitalWrite(pin, invert ? HIGH : LOW);
	}
	
	inline ACTUATOR_METHOD void setActive(bool active) {		
		fastDigitalWrite(pin, active^invert ? HIGH : LOW);
	}

};

class DigitalPinActuator ACTUATOR_BASE_CLASS_DECL
{
private:
	bool invert;
	uint8_t pin;
public:		
	DigitalPinActuator(uint8_t pin, bool invert) {
		this->invert = invert;
		this->pin = pin;
		pinMode(pin, OUTPUT);
	}
	
	inline ACTUATOR_METHOD void setActive(bool active) {
		digitalWrite(pin, active^invert ? HIGH : LOW);
	}
};	
	
#endif