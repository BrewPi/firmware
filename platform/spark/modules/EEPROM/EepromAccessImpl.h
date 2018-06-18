#pragma once

#include "eeprom_hal.h"

#include "EepromTypes.h"
#include <algorithm>

class SparkEepromAccess
{
public:
    SparkEepromAccess() = default;
    ~SparkEepromAccess() = default;
    
    uint8_t readByte(eptr_t offset) {
    	return HAL_EEPROM_Read(offset);
    }
    void writeByte(eptr_t offset, uint8_t value) {
        HAL_EEPROM_Write(offset, value);
    }

    void readBlock(void* target, eptr_t offset, uint16_t size) {
    	uint16_t end = std::min(uint16_t(offset) + size, uint16_t(0) + length());
    	unsigned char * readTo = reinterpret_cast<unsigned char *>(target);
        for(eptr_t idx = offset; idx < end; idx++){
        	*readTo++ = HAL_EEPROM_Read(idx);

        }
    }
    void writeBlock(eptr_t target, const void* source, uint16_t size) {
    	eptr_t end = std::min(target + size, eptr_t(0) + length());
    	const unsigned char * readFrom = reinterpret_cast<const unsigned char *>(source);
		for(eptr_t idx = target; idx < end; idx++){
			unsigned char value = *readFrom++;
			HAL_EEPROM_Write(idx, value);
		}
    }

    uint16_t length() {
        return HAL_EEPROM_Length();
    }

    void clear() {
        HAL_EEPROM_Clear();
    }
};

typedef SparkEepromAccess EepromAccess;


