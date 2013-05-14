/*
 * TempSensorBasic.h
 *
 * Created: 11/05/2013 04:58:13
 *  Author: mat
 */ 

#pragma once


class BasicTempSensor
{
public:
	virtual ~BasicTempSensor() { }
	
	virtual bool isConnected(void) = 0;
	
	/*
	 * Attempt to (re-)initialize the sensor and fetch a sensor reading
	 * Returns a temperature reading, or DEVICE_DISCONNECTED
	 */
	virtual fixed7_9 init() =0;

	/*
	 * Fetch a new reading from the sensor
	 */
	virtual fixed7_9 read() = 0;
	
};
