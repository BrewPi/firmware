/*
 * OneWireTempSensor.h
 *
 * Created: 15/03/2013 09:53:09
 *  Author: mat
 */ 


#ifndef ONEWIRETEMPSENSOR_H_
#define ONEWIRETEMPSENSOR_H_

#include "TempSensor.h"
#include "FastDigitalPin.h"

class DallasTemperature;
class OneWire;


class OneWireTempSensor : public BasicTempSensor {
public:	
	OneWireTempSensor(OneWire* bus, DeviceAddress address=NULL) : oneWire(bus), sensor(NULL) {
		lastRequestTime = 0;
		connected = 0;	
		
		if (address)
			memcpy(sensorAddress, address, sizeof(DeviceAddress));
		else
			sensorAddress[0] = 0;
	};
	
	~OneWireTempSensor(){
		delete sensor;
	};
	
	bool isConnected(void){
		return connected;
	}
	
	fixed7_9 init();
	fixed7_9 read();
	
	private:
	bool connected;
	unsigned long lastRequestTime; // in milliseconds
	
	OneWire * oneWire;
	DallasTemperature * sensor;
	DeviceAddress sensorAddress;	
};



#endif /* ONEWIRETEMPSENSOR_H_ */