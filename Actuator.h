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

/************************************************************************/
/* An actuator simply turns something on or off.                        */
/************************************************************************/

class Actuator
{
	public:
	virtual boolean isActive()=0;
	virtual void setActive(boolean active)=0;
	virtual void activate()  { setActive(true); }
	virtual void deactivate() { setActive(false); }
};

/************************************************************************/
/* An Actuator that uses a digital pin to control the actuator state.                                                                     */
/************************************************************************/
class DigitalPinActuator : public Actuator
{
	private:
	int pin;
	boolean active;
	boolean inverse;
	
	public:
	DigitalPinActuator(int _pin, boolean _inverse=false, boolean _initiallyActive=false);
	
	virtual void setActive(boolean active);	
	virtual boolean isActive();	
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