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
 *
 * Actuator.h
 *
 * Created: 19/02/2013 18:23:38
 *  Author: mat
 */ 


#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#define ACTUATOR_VIRTUAL 0

#if ACTUATOR_VIRTUAL==1	
	#define ACTUATOR_METHOD virtual
	#define ACTUATOR_METHOD_IMPL =0
	#define ACTUATOR_BASE_CLASS_DECL : public Actuator
#else
	#define ACTUATOR_METHOD inline
	#define ACTUATOR_METHOD_IMPL {}
	#define ACTUATOR_BASE_CLASS_DECL
#endif


/************************************************************************/
/* An actuator simply turns something on or off.                        */
/************************************************************************/

class Actuator
{
	public:
	ACTUATOR_METHOD boolean isActive() ACTUATOR_METHOD_IMPL;
	ACTUATOR_METHOD void setActive(boolean active) ACTUATOR_METHOD_IMPL;
	ACTUATOR_METHOD void activate()  { setActive(true); }
	ACTUATOR_METHOD void deactivate() { setActive(false); }
};

/************************************************************************/
/* An Actuator that uses a digital pin to control the actuator state.                                                                     */
/************************************************************************/
class DigitalPinActuator ACTUATOR_BASE_CLASS_DECL
{
	private:
	int pin;
	boolean active;
	boolean inverse;
	
	public:
	DigitalPinActuator(int _pin, boolean _inverse=false, boolean _initiallyActive=false) : pin(_pin), inverse(_inverse)
	{
		pinMode(pin, OUTPUT);
		setActive(_initiallyActive);
	}
	
	ACTUATOR_METHOD void setActive(boolean active) {
		this->active = active;
		digitalWrite(pin, active ? HIGH : LOW);
	}

	ACTUATOR_METHOD boolean isActive() {
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