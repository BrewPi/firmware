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

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "DigitalPin.h"

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

class ValueActuator ACTUATOR_BASE_CLASS_DECL
{
public:
	ValueActuator() : state(false) {}
	ValueActuator(bool initial) : state(initial) {}
	//ACTUATOR_METHOD bool isActive() { return state; }
	ACTUATOR_METHOD void setActive(bool active) { state = active; }

private:
	bool state;	
};


template<int pin, bool invert> class DigitalPinActuatorInline ACTUATOR_BASE_CLASS_DECL
{
	public:
	DigitalPinActuatorInline()
	{
#ifndef __OPTIMIZE__
		pinMode(pin, OUTPUT);
#else		
		fastPinMode(pin, OUTPUT);
#endif		
	}
	
	ACTUATOR_METHOD void setActive(bool active) {		
#ifndef __OPTIMIZE__
		digitalWrite(pin, active^invert ? HIGH : LOW);
#else		
		fastDigitalWrite(pin, active^invert ? HIGH : LOW);
#endif		
	}

};

/************************************************************************/
/* An Actuator that uses a digital pin to control the actuator state.                                                                     */
/************************************************************************/
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

class OneWire;

/************************************************************************/
/* An actuator that uses the 2408 to control state.                                                                     */
/************************************************************************/
class OneWireActuator : public Actuator
{
	private:
	//OneWire& oneWire;
	
};	
	


#endif /* ACTUATOR_H_ */