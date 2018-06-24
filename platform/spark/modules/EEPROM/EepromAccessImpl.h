#pragma once

#include "application.h"
#include "EepromTypes.h"

class SparkEepromAccess
{
private:
static const uint16_t MAGIC_BYTES;

public:
    SparkEepromAccess() = default;
    ~SparkEepromAccess() = default;
    
    static bool init(){
    	uint16_t magic = 0;
    	EEPROM.get(0, magic);
    	if(magic != MAGIC_BYTES){
    		clear();
    		return true;
    	}
    	return false;
    }

    static uint8_t readByte(eptr_t offset) {
    	return EEPROM.read(offset);
    }
    static void writeByte(eptr_t offset, uint8_t value) {
        EEPROM.write(offset, value);
    }

    template <typename T>
    static T &get(eptr_t offset, T &t )
    {
    	return EEPROM.get<T>(offset, t);
    }

    template <typename T>
    static const T &put(eptr_t offset, T &t ){
    	return EEPROM.put<T>(offset, t);
    }

    static uint16_t length() {
        return EEPROM.length();
    }

    static void clear() {
    	EEPROM.clear();
    	EEPROM.put(0, MAGIC_BYTES);
    }
};

extern SparkEepromAccess eepromAccess;

