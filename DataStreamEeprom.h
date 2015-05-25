#pragma once
/*
 * DataStreamEeprom.h
 *
 * Created: 09/02/2014 07:47:12
 *  Author: mat
 */ 

#include "DataStream.h"

/**
 * Provides storage for a stream offset and length.
 */
template <class Offset, class Length> class StreamRegion
{
	protected:
		Offset _offset;
		Length _length;

	public:		
		Offset offset() { return _offset; }
		Length length() { return _length; }
			
		void reset(Offset o, Length l) {
			_offset = o;
			_length = l;
		}
};

struct EepromStreamRegion : public StreamRegion<eptr_t, uint16_t> 
{
};

/**
 * A datastream implementation that writes to a region of eeprom.
 * Eeprom is written via the EepromAccess helper.
 * The stream is only written up to the length specified - once that length has been filled
 * writes are silently failed.
 * @see EepromAccess
 */
class EepromDataOut : public DataOut, public EepromStreamRegion
{
public:	
	bool write(uint8_t value) {
		if (_length) {
			eepromAccess.writeByte(_offset++, value);
			_length--;
			return true;
		}
		return false;
	}
	void close() {
		_length = 0;
	}
	
};


/**
 * A data input stream that reads from a region of eeprom.
 * @see EepromAccess
 */
struct EepromDataIn : public DataIn, public EepromStreamRegion
{
	bool hasNext() { return _length; }
	uint8_t peek() { return eepromAccess.readByte(_offset); }
		
	uint8_t next() {
		uint8_t result = 0;
		if (_length) {
			_length--;
			result = eepromAccess.readByte(_offset++);
		}
		return result;
	}
};
	
