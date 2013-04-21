/*
 * OneWireActuator.h
 *
 * Created: 21/04/2013 02:43:11
 *  Author: mat
 * 
 * Implements an actuator using the one-wire 2413 device. (2450 may be added later.)
 */ 


#pragma once

#include "Actuator.h"
#include "ds2413.h"

class OneWireActuator : public Actuator
{
public:	
	void init(OneWire* bus, DeviceAddress& address, pio_t pio) {
		device.init(bus, address);		
	}
	
			
private:
	DS2413 device;	
	uint8_t pio;
};

