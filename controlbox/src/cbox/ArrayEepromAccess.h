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

#include "EepromAccess.h"
#include <cstdint>
#include <cstring>

/**
 * This is here in case we want to not persist eeprom.
 * It was used as a placeholder until the virtual device HAL
 * supported eeprom persistence.
 */
template <size_t eeprom_size>
class ArrayEepromAccess : public EepromAccess {
public:
    ArrayEepromAccess()
        : changed(false)
    {
        memset(data, -1, length());
    }
    virtual ~ArrayEepromAccess() = default;

    virtual uint8_t readByte(uint16_t offset) const override final
    {
        if (isValidRange(offset, 1))
            return data[offset];
        return 0;
    }

    virtual void writeByte(uint16_t offset, uint8_t value) override final
    {
        if (isValidRange(offset, 1)) {
            data[offset] = value;
            flagChanged();
        }
    }

    virtual void readBlock(void* target, uint16_t offset, uint16_t size) const override final
    {
        if (isValidRange(offset, size))
            memcpy((uint8_t*)target, &data[offset], size);
    }

    virtual void writeBlock(uint16_t target, const void* source, uint16_t size) override final
    {
        if (isValidRange(target, size)) {
            memcpy(&data[target], (const uint8_t*)source, size);
            flagChanged();
        }
    }

    const uint8_t* eepromData() const
    {
        return &data[0];
    }

    virtual uint16_t length() const override final
    {
        return eeprom_size;
    }

    virtual void clear() override final
    {
        memset(&data, 0, sizeof(data));
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

    bool isValidRange(uint16_t offset, uint16_t size) const
    {
        bool valid = offset >= 0 && offset < length()
                     && size <= length() && offset + size <= length();
        return valid;
    }

private:
    uint8_t data[eeprom_size];
    bool changed;
};
