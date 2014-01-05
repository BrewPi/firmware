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

// Enables use of "first on bus" address via a constructor that takes just the OneWire bus
#ifndef DS2413_DYNAMIC_ADDRESS 
#define DS2413_DYNAMIC_ADDRESS 0
#endif

#define  DS2413_FAMILY_ID 0x3A

/*
 * Provides access to a OneWire-addressable dual-channel I/O device. 
 * The channel latch can be set to on (false) or off (true).
 * When a channel is off (PIOx=1), the channel state can be sensed. This is the power on-default. 
 *
 * channelRead/channelWrite reads and writes the channel latch state to turn the output transistor on or off
 * channelSense senses if the channel is pulled high.
 */
class DS2413
{
public:
	
	DS2413()
	{		
	}

	/*
	 * Initializes this ds2413.
	 * /param oneWire The oneWire bus the device is connected to
	 * /param address The oneWire address of the device to use.
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
	 * Determines if the device is connected. Note that the value returned here is potentially stale immediately on return,
	 * and should only be used for status reporting. In particular, a return value of true does not provide any guarantee
	 * that subsequent operations will succeed.
	 */
	bool isConnected()
	{
		return validAddress(oneWire, this->address) && accessRead()>=0;
	}
	
	// assumes pio is either 0 or 1, which translates to masks 0x1 and 0x2
	uint8_t pioMask(pio_t pio) { return pio++; }

	/*
	 * Reads the output state of a given channel, defaulting to a given value on error.
	 * Note that for a read to make sense the channel must be off (value written is 1).
	 */
	bool channelRead(pio_t pio, bool defaultValue)
	{
		byte result = channelReadAll();		
		if (result<0)
			return defaultValue;
		return (result & pioMask(pio));
	}
	
	/*
	 * Reads the output state of a given channel, defaulting to a given value on error.
	 * Note that for a read to make sense the channel must be off (value written is 1).
	 */
	bool channelSense(pio_t pio, bool defaultValue)
	{
		byte result = channelSenseAll();
		if (result<0)
			return defaultValue;
		return (result & pioMask(pio));
	}


	/*
	 * Performs a simultaneous read of both channels.
	 * /return <0 if there was an error otherwise bit 1 is channel A state, bit 2 is channel B state.
	 */
	uint8_t channelReadAll();
	uint8_t channelSenseAll();
	
	/*
	 * Writes to the latch for a given PIO.
	 * /param set	1 to switch the pin off, 0 to switch on. 
	 */
	bool channelWrite(pio_t pio, bool set)
	{
		bool ok = false;
		byte result = channelReadAll();
		if (result>=0) {
			uint8_t mask = pioMask(pio);
			if (set)
				result |= mask;
			else
				result &= ~mask;
			ok = channelWriteAll((uint8_t)result);
		}
		return ok;
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
	 * The lower 4-bits are the values as described under PIO ACCESSS READ [F5h] in the ds2413 datasheet:	 
	 * b0: PIOA state
	 * b1: PIOA output latch state
	 * b2: PIOB state
	 * b3: PIOB output latch state
	 */
	byte accessRead(uint8_t maxTries=3) /* const */
	{		
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
	 * /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1	 
	 * /param maxTries the maximum number of attempts before giving up.
	 * /return true on success
	 */
	bool accessWrite(uint8_t b, uint8_t maxTries=3)
	{
		#define ACCESS_WRITE 0x5A
		#define ACK_SUCCESS 0xAA
		#define ACK_ERROR 0xFF
		
		b |= 0xFC;   		/* Upper 6 bits should be set to 1's */
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
			
			//out.print("tries "); out.print(maxTries); out.print(" ack ");out.print(ack, HEX);out.print(" newValues ");out.print(newSettings, HEX);
			//out.println();
		} while (ack!=ACK_SUCCESS && maxTries-->0);
		
		oneWire->reset();
		return ack==ACK_SUCCESS;
 	}

	OneWire* oneWire;	
	DeviceAddress address;	
};

#endif