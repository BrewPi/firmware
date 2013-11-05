/*
 * Copyright 2013 Matthew McGowan 
 * Copyright 2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef ARDUINO

#include "Brewpi.h"
#include "OneWire.h"
#include "PiLink.h"

typedef uint8_t DeviceAddress[8];
typedef uint8_t pio_t;

#ifndef DS2413_DYNAMIC_ADDRESS 
#define DS2413_DYNAMIC_ADDRESS 0
#endif

#define  DS2413_FAMILY_ID 0x3A

class DS2413
{
public:
	
	DS2413() : oneWire(NULL)
	{		
	}

	/*
	 * Initializes this ds2413 actuator device. 
	 * /param oneWire The oneWire bus the device is connected to
	 * /param address The oneWire address of the device to use. If {@code NULL}
	 *		the first device found that is a ds2413 is used. 
	 */
	
	void init(OneWire* oneWire, DeviceAddress address)
	{
		this->oneWire = oneWire;
		memcpy(this->address, address, sizeof(DeviceAddress));
	}

#if DS2413_DYNAMIC_ADDRESS 
	void init(OneWire* oneWire)
	{
		this->oneWire = oneWire;
		getAddress(oneWire, this->address, 0);
	}
#endif	

	/*
	 * Determines if the device is connected. Note that the value returned here is potentially state immediately on return,
	 * and should only be used for status reporting. In particular, a return value of true does not provide any guarantee
	 * that subsequent operations will succeed.
	 */
	bool isConnected()
	{
		return validAddress(oneWire, this->address) && channelReadAll()!=0xFF;
	}
	
	uint8_t pioMask(pio_t pio) { return pio==0 ? 1 : 2; }

	bool channelRead(pio_t pio, bool defaultValue)
	{
		byte result = channelReadAll();
		if (result<0)
			return defaultValue;
		return (result & pioMask(pio))!=0;
	}
	
	/*
	 * Performs a simultaneous read of both channels.
	 * \ return 0xFF if there was an error otherwise bit 1 is channel A state, bit 2 is channel B state.
	 */
	uint8_t channelReadAll();
	
	void channelWrite(pio_t pio, bool set)
	{
		byte result = channelReadAll();
		if (result>=0) {
			uint8_t mask = pioMask(pio);
			if (set)
				result |= mask;
			else
				result &= ~mask;
			channelWriteAll((uint8_t)result);
		}
		else{
			logDebug("ChannelReadAll failed");
		}
	}
	
	void channelWriteAll(uint8_t values);
	
	DeviceAddress& getDeviceAddress()
	{
		return address;
	}		

	static bool validAddress(OneWire* oneWire, DeviceAddress deviceAddress)
	{
		return deviceAddress[0] && (oneWire->crc8(deviceAddress, 7) == deviceAddress[7]);
	}
	

	/*
	 * Fetches the address at the given enumeration index.
	 * Only devices matching the 2413 family ID are considered.
	 * /param deviceAddress the address found
	 * /param index	The 0-based device index to return
	 * /return true if the device was found and is the address valid.
	 */
#if DS2413_DYNAMIC_ADDRESS
	static bool getAddress(OneWire* oneWire, uint8_t* deviceAddress, uint8_t index)
	{
		oneWire->reset_search();

		for (uint8_t pos = 0; deviceAddress[0] = 0, oneWire->search(deviceAddress); )
		{
			if (deviceAddress[0]==DS2413_FAMILY_ID)
			{				
				if (pos++ == index)
					return true;
			}			
		}
		return false;
	}
#endif	
private:

	/*
	 * Read all values at once, both current state and sensed values. The read performs data-integrity checks.
	 * Returns a negative result if the device cannot be read successfully within the given number of tries.
	 */
	byte accessRead(uint8_t maxTries=100) /* const */
	{		
		// todo - due to bus errors, should the entire operation be retried? At present only the results are re-read, but it may be necessary to
		// perform the select and write again too.
		#define ACCESS_READ 0xF5
		
		oneWire->reset();
		oneWire->select(address);
		oneWire->write(ACCESS_READ);
		
		bool success = false;
		uint8_t data;
		do 
		{
			data = oneWire->read();
			success = (data>>4)==(!data&0xF);
			data &= 0xF;
		} while (!success && maxTries-->0);
		
		oneWire->reset();		
		return success ? data : data|0x80;
	}
	
	/*
	 * Writes the state of all PIOs in one operation.                                                                    
	 */
	bool accessWrite(uint8_t b, uint8_t maxTries=100)
	{
		#define ACCESS_WRITE 0x5A
		#define ACK_SUCCESS 0xAA
		#define ACK_ERROR 0xFF
		
		/* Upper 6 bits should be set to 1's */
		b |= 0xFC;
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
			//uint8_t newSettings;
			if (ack==ACK_SUCCESS)
				/*newSettings = */oneWire->read();		// status byte sent after ack
			
			oneWire->reset();
			//out.print("tries "); out.print(maxTries); out.print(" ack ");out.print(ack, HEX);out.print(" newValues ");out.print(newSettings, HEX);
			//out.println();
		} while (ack!=ACK_SUCCESS && maxTries-->0);
		
		return ack==ACK_SUCCESS;
 	}

	OneWire* oneWire;	
	DeviceAddress address;
	uint8_t state;
};

#endif