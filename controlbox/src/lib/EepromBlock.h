#pragma once
#include "ValuesEeprom.h"

namespace cbox {

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
		: EepromBaseValue(cb_nonstatic_decl(ea)), _offset(offset), _size(size)
	{}

	virtual void readTo(DataOut& out) override final
    {
		_readTo(out, _offset, _size);
	}

	virtual void writeFrom(DataIn& dataIn) override final
    {
		_writeFrom(dataIn, _size, _offset);
	}

	virtual uint8_t readStreamSize() override final
    {
	    return _size;
    }

	eptr_t eeprom_offset()
    {
	    return _offset;
    }
};

}
