
#include "brewpi_avr.h"
#include "OneWire.h"

#include "ds2413.h"

#if 0
	/*
	 * Read all values at once, both current state and sensed values. The read performs data-integrity checks.
	 * Returns a negative result if the device cannot be read successfully within the given number of tries.
	 */
	byte DS2413::accessRead(uint8_t maxTries) /* const */
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
	bool DS2413::accessWrite(uint8_t b, uint8_t maxTries)
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
#endif

