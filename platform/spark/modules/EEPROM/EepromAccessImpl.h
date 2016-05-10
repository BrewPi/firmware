#pragma once

#if PLATFORM_ID!=3

#include "flashee-eeprom.h"

#include "EepromTypes.h"

namespace Flashee {
    class FlashDevice;
};

class SparkEepromAccess
{
    Flashee::FlashDevice* flash;
public:
    void init();
    
    uint8_t readByte(eptr_t offset) {
        uint8_t value;
        readBlock(&value, offset, 1);
        return value;
    }
    void writeByte(eptr_t offset, uint8_t value) {
        writeBlock(offset, &value, 1);
    }

    void readBlock(void* target, eptr_t offset, uint16_t size) {
        flash->read(target, offset, size);
    }
    void writeBlock(eptr_t target, const void* source, uint16_t size) {
        flash->write(source, target, size);
    }
    
    size_t length() {
        return flash->length();
    }
};

typedef SparkEepromAccess EepromAccess;

#else

#include "ArrayEepromAccess.h"

typedef ArrayEepromAccess<1024> EepromAccess;


#endif
