/*
 * Copyright 2013 Matthew McGowan
 * Sensor.h
 *
 * Created: 20/02/2013 15:58:27
 *  Author: mat
 *
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include "Arduino.h"
#include "DigitalPin.h"

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

typedef Sensor<boolean> SwitchSensor;

class DigitalPinSensor : public SwitchSensor
{
	private:
	int pin;
	boolean invert;
	
	public:
	DigitalPinSensor(int _pin, boolean _invert, boolean _internalPullup)
		: pin(_pin), invert(_invert)
		{
			pinMode(pin, _internalPullup ? INPUT_PULLUP : INPUT);
		}

	virtual boolean sense()
	{
		return digitalRead(pin) ^ invert;
	}


};



/* A SwitchSensor whose state is provided by a hardware pin. 
  By using a template, the compiler can inline the call to digitalRead. 
*/
template<int pin, boolean invert, boolean internalPullup> 
class DigitalPinSensorInline : public SwitchSensor
{
	public:
	DigitalPinSensorInline() {
		fastPinMode(pin, internalPullup ? INPUT_PULLUP : INPUT);
	}
	
	virtual boolean sense() {
		return fastDigitalRead(pin) ^ invert;
	}
};




#endif /* SENSOR_H_ */