/*
 * DisconnectedTempSensor.h
 *
 * Created: 10/05/2013 18:11:44
 *  Author: mat
 */ 

#pragma once

#include "TempSensor.h"

class DisconnectedTempSensor : public BasicTempSensor {
	
public:
	bool isConnected() { return false; }

	fixed7_9 init() {
		return read();
	}
	
	fixed7_9 read() {
		return DEVICE_DISCONNECTED;
	}	
	
};