/* 
 * File:   OneWireSwitch.cpp
 * Author: Elco
 * 
 * Created on 15 december 2014, 21:38
 */

#include "OneWireSwitch.h"

OneWireSwitch::OneWireSwitch() {
}

OneWireSwitch::~OneWireSwitch() {
}

/*
 * Initializes this OneWire slave.
 * /param oneWire The oneWire bus the device is connected to
 * /param address The oneWire address of the device to use.
 */
void OneWireSwitch::init(OneWire* oneWire, DeviceAddress address) {
    this->oneWire = oneWire;
    memcpy(this->address, address, sizeof (DeviceAddress));
}

DeviceAddress& OneWireSwitch::getDeviceAddress() {
    return address;
}

bool OneWireSwitch::validAddress(OneWire* oneWire, DeviceAddress deviceAddress) {
    return deviceAddress[0] && (oneWire->crc8(deviceAddress, 7) == deviceAddress[7]);
}

/*
 * Determines if the device is connected. Note that the value returned here is potentially stale immediately on return,
 * and should only be used for status reporting. In particular, a return value of true does not provide any guarantee
 * that subsequent operations will succeed.
 */
bool OneWireSwitch::isConnected() {
    return validAddress(oneWire, this->address) && accessRead() >= 0;
}

/*
	* Read all values at once, both current state and sensed values. The read performs data-integrity checks.
	* Returns a negative result if the device cannot be read successfully within the given number of tries.
	* The lower 4-bits are the values as described under PIO ACCESSS READ [F5h] in the ds2413 datasheet:	 
	* b0: PIOA state
	* b1: PIOA output latch state
	* b2: PIOB state
	* b3: PIOB output latch state
	*/
uint8_t OneWireSwitch::accessRead(uint8_t maxTries) /* const */
{		
	#define ACCESS_READ 0xF5
		
	oneWire->reset();
	oneWire->select(address);
	oneWire->write(ACCESS_READ);
		
	//bool success = false;
	uint8_t data;
	/*do 
	{
		data = oneWire->read();
		success = (data>>4)==(!data&0xF);
		data &= 0xF;
	} while (!success && maxTries-->0);
		
	oneWire->reset();		
	return success ? data : data|0x80;
         */
        data = oneWire->read();
        return data;
}
	
/*
	* Writes the state of all PIOs in one operation.
	* /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1	 
	* /param maxTries the maximum number of attempts before giving up.
	* /return true on success
	*/
bool OneWireSwitch::accessWrite(uint8_t b, uint8_t maxTries)
{
	#define ACCESS_WRITE 0x5A
	#define ACK_SUCCESS 0xAA
	#define ACK_ERROR 0xFF
		
	//b |= 0xFC;   		/* Upper 6 bits should be set to 1's */
	uint8_t ack = 0;
	do
	{
		oneWire->reset();
		oneWire->select(address);
		oneWire->write(ACCESS_WRITE);
		oneWire->write(b);
			
		/* data is sent again, inverted to guard against transmission errors */
		oneWire->write(~b);
		/* Acknowledgement byte, 0xAA for success, 0xFF for failure. */
		ack = oneWire->read();
						
		if (ack==ACK_SUCCESS)
			oneWire->read();		// status byte sent after ack
			
	} while (ack!=ACK_SUCCESS && maxTries-->0);
		
	oneWire->reset();
	return ack==ACK_SUCCESS;
}