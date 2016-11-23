/*
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of Controlbox.
 * 
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Controlbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "EepromTypes.h"
#include "assert.h"
#include "string.h"
#include "EepromAccess.h"
#include <iostream>


template <size_t eeprom_size>
class ArrayEepromAccess : public EepromAccess
{
public:
	ArrayEepromAccess()
	{
		memset(data, -1, eepromLength());
	}

	void load(std::istream& in)
	{
        unsigned offset = 0;
        in.read((char*)data, length());
	}

	void save(std::ostream& out)
	{
		out.write((const char*)eepromData(), length());
        clearChanged();
	}


	uint8_t readByte(eptr_t offset) const override
	{
		if (isValidRange(offset, 1))
			return data[offset];
		return 0;
	}

	void writeByte(eptr_t offset, uint8_t value) override
	{
		if (isValidRange(offset, 1))
		{
			data[offset] = value;
			flagChanged();
		}
	}

	void readBlock(void* target, eptr_t offset, uint16_t size) const override
	{
		if (isValidRange(offset, size))
			memcpy((uint8_t*) target, &data[offset], size);
	}

	void writeBlock(eptr_t target, const void* source, uint16_t size) override
	{
		if (isValidRange(target, size))
		{
			memcpy(&data[target], (const uint8_t*) source, size);
			flagChanged();
		}
	}

	const uint8_t* eepromData() const
	{
		return &data[0];
	}
	
	uint16_t eepromLength() const
	{
		return eeprom_size;
	}

	size_t length() const override
	{
		return eepromLength();
	}

	/**
	 * Determines if the contents have changed since the last call to change.
	 * @return
	 */
	bool hasChanged()
	{
		bool result = changed;
		changed = false;
		return result;
	}

protected:
	void flagChanged()
	{
		changed = true;
	}

    void clearChanged()
    {
        changed = false;
    }

	bool isValidRange(uint16_t offset, uint16_t size) const
	{
		assert(sizeof(ArrayEepromAccess) > eeprom_size);
		bool valid = offset >= 0 && offset < eepromLength()
				&& size <= eepromLength() && offset + size <= eepromLength();
		return valid;
	}
private:
	uint8_t data[eeprom_size];
	bool changed;
};

