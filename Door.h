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
/*
 * Door.h
 *
 * Created: 20/02/2013 16:37:19
 *  Author: mat
 */ 


#ifndef DOOR_H_
#define DOOR_H_

#include "brewpi_avr.h"
#include "Sensor.h"

/*
 * Interface for requesting the state of the door.
 */
class Door
{
	public:	
	virtual bool isOpen();
};

/*
 * Door implementation that uses a sensor to provide the door state.
 */
class SensorDoor : public Door
{
	private:
	SwitchSensor& sensor;
	
	public:
	SensorDoor(SwitchSensor& _sensor) 
		: sensor(_sensor)
	{		
	}
	
	virtual bool isOpen() 
	{
		return !sensor.sense();
	}
};

class MockDoor : public Door
{
	public:
	virtual bool isOpen() { return false; }
};	

#endif /* DOOR_H_ */