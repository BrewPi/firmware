/* 
 * File:   SensorArduinoPin.h
 * Author: mat
 *
 * Created on 22 August 2013, 19:36
 */

#pragma once

#include "Brewpi.h"
#include "FastDigitalPin.h"
#include "Board.h"

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


