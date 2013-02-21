/*
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
/* Sensor.h
 *
 * Created: 20/02/2013 15:58:27
 *  Author: mat
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include "Arduino.h"

template<class T> class Sensor
{	
	public:	
	virtual T& sense(T& t)=0;
};

/************************************************************************/
/* Can pass the result by value                                         */
/************************************************************************/
template<class T>
class ByValueSensor : public Sensor<T>
{
	public:
	virtual T sense();
	
	virtual T& sense(T& target) {
		target = sense();
		return target;
	}
};

class SwitchSensor : public ByValueSensor<boolean>
{
};

class DigitalPinSensor : public SwitchSensor
{
	private:
	int pin;
	boolean invert;
	
	public:
	DigitalPinSensor(int _pin, boolean _internalPullup, boolean _invert);
	
	virtual boolean sense();	
};

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