/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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

#include "Brewpi.h"
#include "FastDigitalPin.h"
#include "Pins.h"

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