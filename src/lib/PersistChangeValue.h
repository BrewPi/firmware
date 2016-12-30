/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Controlbox Firmware.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SystemProfile.h"
#include "ValuesEeprom.h"

/**
 * A value that saves the state to eeprom when the difference between the persisted value and the current
 * value is greater than a given threshold.
 */
class PersistChangeValue : EepromValue
{
	int16_t currentValue;

public:
#if !CONTROLBOX_STATIC
	PersistChangeValue(EepromAccess& ea) : EepromValue(ea) {}
#endif

	int16_t difference() {
		return readPointer(eepromAccess, eeprom_offset()+2);
	}

	int16_t savedValue() {
		return readPointer(eepromAccess, eeprom_offset());
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
			writePointer(eepromAccess, eeprom_offset(), currentValue);
	}

	uint8_t readStreamSize() {
		// todo - why is it hard-coded to 2 bytes?
		return 2;
	}

	/**
	 * 2 bytes: current value
	 * 2 bytes: difference threshold to save
	 */
	static Object* create(ObjectDefinition& def) {
		return new_object(PersistChangeValue(cb_nonstatic_decl(def.eepromAccess())));
	}
};
