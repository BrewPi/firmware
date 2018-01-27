#pragma once
#include "ValuesEeprom.h"

/**
 * Streams an eeprom value of a given fixed size.
 */
class EepromBlock : public EepromBaseValue
{
	protected:
		eptr_t _offset;
		uint8_t _size;

	public:
	EepromBlock(
#if !CONTROLBOX_STATIC
			EepromAccess& ea,
#endif
			eptr_t offset, uint8_t size)
		: EepromBaseValue(cb_nonstatic_decl(ea)), _offset(offset), _size(size) {}

	void readTo(DataOut& out) {
		_readTo(out, _offset, _size);
	}

	void writeFrom(DataIn& dataIn) {
		_writeFrom(dataIn, _size, _offset);
	}

	eptr_t eeprom_offset() { return _offset; }
	uint8_t readStreamSize() { return _size; }
};

