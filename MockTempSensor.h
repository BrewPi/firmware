/*
 * MockTempSensor.h
 *
 * Created: 15/03/2013 10:16:41
 *  Author: mat
 */ 


#ifndef MOCKTEMPSENSOR_H_
#define MOCKTEMPSENSOR_H_

#include "TempSensor.h"

class MockTempSensor : public BasicTempSensor
{
public:	
	MockTempSensor(fixed7_9 initial, fixed7_9 delta) : _temperature(initial), _delta(delta), _connected(true) { }
	
	void setConnected(bool connected)
	{
		_connected = connected;
	}
	
	bool isConnected() { return _connected; }

	fixed7_9 init() {
		return read();
	}
	
	fixed7_9 read()
	{
		if (!isConnected())
			return DEVICE_DISCONNECTED;
		
		switch (tempControl.getMode()) {
			case COOLING:
				_temperature -= _delta;
				break;
			case HEATING:
				_temperature += _delta;
				break;
		}
		
		return _temperature;
	}
	
	private:
	fixed7_9 _temperature;	
	fixed7_9 _delta;	
	bool _connected;
};



#endif /* MOCKTEMPSENSOR_H_ */