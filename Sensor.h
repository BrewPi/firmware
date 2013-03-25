/*
 * Copyright 2013 Matthew McGowan
 * Sensor.h
 *
 * Created: 20/02/2013 15:58:27
 *  Author: mat
 *
 */ 

#pragma once

#include "Arduino.h"
#include "FastDigitalPin.h"

template<class T> class Sensor
{	
	public:	
	virtual T sense()=0;
};

template <class T>
class ValueSensor : public virtual Sensor<T>
{
public:	
	ValueSensor(T initial) : value(initial) {}

	virtual T sense() {
		return value;
	}
	
	void setValue(T _value) {
		value = _value;
	}
	
private:
	T value;	
};

typedef Sensor<bool> SwitchSensor;

class DigitalPinSensor : public SwitchSensor
{
	private:
	int pin;
	bool invert;
	
	public:
	DigitalPinSensor(int _pin, bool _invert, bool _internalPullup)
		: pin(_pin), invert(_invert)
		{
			pinMode(pin, _internalPullup ? INPUT_PULLUP : INPUT);
		}

	virtual bool sense()
	{
		return digitalRead(pin) ^ invert;
	}


};



/* A SwitchSensor whose state is provided by a hardware pin. 
  By using a template, the compiler can inline the call to digitalRead. 
*/
template<uint8_t pin, bool invert, bool internalPullup> 
class DigitalPinSensorInline : public SwitchSensor
{
	public:
	inline __attribute__((always_inline)) DigitalPinSensorInline() {
		fastPinMode(pin, internalPullup ? INPUT_PULLUP : INPUT);
	}
	
	virtual bool sense() {
		return fastDigitalRead(pin) ^ invert;
	}
};

