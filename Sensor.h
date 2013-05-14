/*
 * Copyright 2013 Matthew McGowan
 * Sensor.h
 *
 * Created: 20/02/2013 15:58:27
 *  Author: mat
 *
 */ 

#pragma once

#include "brewpi_avr.h"
#include "FastDigitalPin.h"
#include "pins.h"

template<class T> class Sensor
{	
	public:	
	virtual T sense()=0;
	
	virtual ~Sensor() {}
};

template <class T>
class ValueSensor : public Sensor<T>
{
public:	
	ValueSensor(T initial) : value(initial) {}

	virtual T sense() {
		return (T)0;
	}
	
	void setValue(T _value) {
		value = _value;
	}
	
private:
	T value;	
};

typedef Sensor<bool> SwitchSensor;


/* A SwitchSensor whose state is provided by a hardware pin. 
  By using a template, the compiler can inline and optimize the call to digitalRead to a single instruction.
*/
template<uint8_t pin, bool invert, bool internalPullup> 
class DigitalConstantPinSensor : public SwitchSensor
{
	public:
	DigitalConstantPinSensor() {
		fastPinMode(pin, internalPullup ? INPUT_PULLUP : INPUT);
	}
	
	virtual bool sense() {
		return fastDigitalRead(pin) ^ invert;
	}
};

class DigitalPinSensor : public SwitchSensor
{
private:
	bool invert;
	uint8_t pin;
	
	
	
public:

	DigitalPinSensor(uint8_t pin, bool invert)
	{
		pinMode(pin, USE_INTERNAL_PULL_UP_RESISTORS ? INPUT_PULLUP : INPUT);
		this->invert = invert;
		this->pin = pin;		
	}
	
	virtual bool sense() {
		return digitalRead(pin) ^ invert;
	}	
};