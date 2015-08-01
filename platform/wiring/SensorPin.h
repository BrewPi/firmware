/* 
 * File:   SensorArduinoPin.h
 * Author: mat
 *
 * Created on 22 August 2013, 19:36
 */

#pragma once

#include "Brewpi.h"
#include "Board.h"

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

