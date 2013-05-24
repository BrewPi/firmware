/*
 * OneWireTempSensor.h
 *
 * Created: 15/03/2013 09:53:09
 *  Author: mat
 */ 


#ifndef ONEWIRETEMPSENSOR_H_
#define ONEWIRETEMPSENSOR_H_

#include "Brewpi.h"
#include "TempSensor.h"
#include "FastDigitalPin.h"

class DallasTemperature;
class OneWire;

class OneWireTempSensor : public BasicTempSensor {
public:	
	/**
	 * Constructs a new onewire temp sensor.
	 * /param bus	The onewire bus this sensor is on.
	 * /param address	The onewire address for this sensor. If all bytes are 0 in the address, the first temp sensor
	 *    on the bus is used.
	 * /param calibration	A temperature value that is added to all readings. This can be used to calibrate the sensor.	 
	 */
	OneWireTempSensor(OneWire* bus, DeviceAddress address, fixed4_4 calibrationOffset)
	: oneWire(bus), sensor(NULL) {
		lastRequestTime = 0;
		connected = true;  // assume connected. Transition from connected to disconnected prints a message.
		memcpy(sensorAddress, address, sizeof(DeviceAddress));
		this->calibrationOffset = calibrationOffset;
	};
	
	~OneWireTempSensor();
	
	bool isConnected(void){
		return connected;
	}		
	
	fixed7_9 init();
	fixed7_9 read();
	
	private:

	void setConnected(bool connected);
	void waitForConversion();
	
	uint16_t lastRequestTime; // in seconds
	
	OneWire * oneWire;
	DallasTemperature * sensor;
	DeviceAddress sensorAddress;

	fixed4_4 calibrationOffset;		
	bool connected;
	
};



#endif /* ONEWIRETEMPSENSOR_H_ */