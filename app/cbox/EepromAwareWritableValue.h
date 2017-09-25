/*
 * Copyright 2017 BrewPi
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

#include "Values.h"

/**
 * A base class for objects that want to know where in the persisted data their definition is stored.
 */
class EepromAwareWritableValue : public WritableValue
{
    eptr_t address = -1;

public:

    void rehydrated(eptr_t _address) {
        address = _address;
    }

    eptr_t eeprom_offset() { return address; }
    uint8_t eepromSize(cb_nonstatic_decl(EepromAccess& eepromAccess)) { return eepromAccess.readByte(address-1); }
};
