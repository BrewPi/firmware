#pragma once

#include "DataStream.h"
#include "Values.h"
#include "EepromAccess.h"
#include "DataStreamEeprom.h"
#include "SystemProfile.h"
#include "StreamUtil.h"

/**
 * Base class for a read-write value in eeprom. This class is responsible for moving the data
 * between eeprom and the stream. 
 */
class EepromBaseValue : public Value  {

protected:

	void _readTo(DataOut& out, eptr_t offset, uint8_t size)
	{
		EepromDataIn in;
		in.reset(offset, size);
		in.push(out, size);
	}
		
	void _writeFrom(DataIn& in, eptr_t offset, uint8_t size)
	{
		EepromDataOut out;
		out.reset(offset, size);
		in.push(out, size);
	}		

	/**
	 * Writes masked data to eeprom starting at the given address.
	 */
	void _writeMaskedFrom(DataIn& dataIn, DataIn& maskIn, int8_t length, 
                                                        eptr_t address) {
		while (--length>=0) {
			uint8_t current = eepromAccess.readByte(address);
			uint8_t update = WritableValue::nextMaskedByte(current, dataIn, maskIn);
			eepromAccess.writeByte(address++, update);
		}
	}

	void _writeMaskedOut(DataIn& dataIn, DataIn& maskIn, DataIn& in, DataOut& out, int8_t length) {
		while (--length>=0) {			
			out.write(WritableValue::nextMaskedByte(in.next(), dataIn, maskIn));
		}
	}


	object_t objectType() { return otValue | otWritableFlag; }
			
};

/**
 * An object that uses it's construction definition in eeprom storage also as it's value.
 * So, the initial construction data block sets the desired size and initial data.
 * Subsequent read/write operations read and write to that data. 
 */
class EepromValue : public EepromBaseValue
{	
protected:    
        eptr_t address;
		
public:

	void rehydrated(eptr_t address)
	{
		this->address = address;
	}

	void readTo(DataOut& out) {
		_readTo(out, eeprom_offset(), EepromValue::streamSize());
	}
	
	void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) {
		_writeMaskedFrom(dataIn, maskIn, EepromValue::streamSize(), address);
	}

	eptr_t eeprom_offset() { return address; }
	uint8_t streamSize() { return eepromAccess.readByte(address-1); }
		
	static Object* create(ObjectDefinition& defn) 
	{
		return new_object(EepromValue());
	}
};


/**
 * Streams an eeprom value of a given fixed size.
 */
class EepromBlock : public EepromBaseValue
{
	protected:
		eptr_t _offset;		
		uint8_t _size;
					
	public:
		EepromBlock(eptr_t offset, uint8_t size) : _offset(offset), _size(size) {}

		void readTo(DataOut& out) {
			_readTo(out, _offset, _size);
		}
	
        void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) {
            _writeMaskedFrom(dataIn, maskIn, _size, _offset);
        }

		eptr_t eeprom_offset() { return _offset; }
		uint8_t streamSize() { return _size; }
};


/**
 * A value that saves the state to eeprom when the difference between the persisted value and the current
 * value is greater than a given threshold.
 */
class PersistChangeValue : EepromValue
{
	int16_t currentValue;
	
public:
	
	int16_t difference() {
		return readPointer(eeprom_offset()+2);
	}	
	
	int16_t savedValue() {
		return readPointer(eeprom_offset());
	}
	
	void rehydrated(eptr_t address) {
		EepromValue::rehydrated(address);
		currentValue = savedValue();
	}
	
	void readTo(DataOut& out) {
		out.write(uint8_t(currentValue>>8));
		out.write(uint8_t(currentValue&0xFF));		
	}
				
	void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) {
		// to save space, we don't bother allowing masking against the source
		currentValue = int16_t(readMaskedByte(dataIn, maskIn))<<8 | readMaskedByte(dataIn, maskIn);				
		if (abs(currentValue-savedValue())>difference())
			writePointer(eeprom_offset(), currentValue);
	}
			
	uint8_t streamSize() {
		return 2;
	}

	/**
	 * 2 bytes: current value
	 * 2 bytes: difference threshold to save
	 */
	static Object* create(ObjectDefinition& def) {				
		return new_object(PersistChangeValue());
	}
};

#if 0
/**
 * Provides state read/write (in addition to stream read/write) for an eeprom value. 
 */
template <class T, int _size=sizeof(T)> class EepromValue : public EepromStreamValue
{
	public:
		
		EepromValue(eptr_t offset) : EepromStreamValue(offset) {}
		
		T read() {
			T result;
			EepromDataIn in(this->_offset, _size);
			in.read((uint8_t*)&result, _size);
			return result;
		}
		
		void write(T t) {
			EepromDataOut out(this->_offset, _size);
			out.write(&t, _size);
		}
		
		uint8_t streamSize() { return _size; }
};


/**
 * Provides a streamable value to eeprom. The size is dynamic, unlike EepromStreamValue, where the size is
 * known at compile time. 
 * mdm: I forget why I wrote this - it's presently unused.
 */
class EepromDynamicStreamValue : public EepromBaseValue
{
	private:
		eptr_t _offset;
		uint8_t _size;
		
	public:	
		EepromDynamicStreamValue(eptr_t offset, uint8_t size) : _offset(offset), _size(size) {}
	
		void writeFrom(DataIn& in) {
			_writeFrom(in, _offset, _size);
		}
		
		void readTo(DataOut& out) {
			_readTo(out, _offset, _size);
		}
		
		uint8_t streamSize() { return _size; }
};
#endif