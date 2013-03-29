/*
 * ExternalTempSensor.h
 *
 * Created: 29/03/2013 04:46:34
 *  Author: mat
 */ 

#pragma once

#include "TempSensor.h"

class ExternalTempSensor : public BasicTempSensor
{
	public:
	ExternalTempSensor() : _temperature(0), _connected(false) { }

	void setConnected(bool connected)
	{
		_connected = connected;
	}

	bool isConnected() { return _connected; }

	fixed7_9 init() {
		return read();
	}
	
	fixed7_9 read() {
		if (!isConnected())
			return DEVICE_DISCONNECTED;				
		return _temperature;
	}
	
	void setValue(fixed7_9 newTemp) {
		_temperature = newTemp;		
	}
	
	private:
	fixed7_9 _temperature;
	bool _connected;
};
