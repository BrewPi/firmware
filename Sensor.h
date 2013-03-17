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
	DigitalPinSensor(int _pin, boolean _internalPullup, boolean _invert);
	
	virtual boolean sense();	
};



/* A SwitchSensor whose state is provided by a hardware pin. */
template<int pin> class DigitalHardPinSensor : public SwitchSensor
{
	private:
	boolean invert;
	
	public:
	DigitalHardPinSensor(boolean _internalPullup, boolean _invert);
	
	virtual boolean sense() {
		return digitalRead(pin) ^ invert;
	}
};




#endif /* SENSOR_H_ */