#pragma once

#include "flashee-eeprom.h"
#include "SparkEepromRegions.h"

class SparkEepromAccess
{
    Flashee::FlashDevice* flash;
public:
    SparkEepromAccess() 
    {
        flash = Flashee::Devices::createAddressErase(4096*EEPROM_CONTROLLER_START_BLOCK, 4096*EEPROM_CONTROLLER_END_BLOCK);
    }
    
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
};

typedef SparkEepromAccess EepromAccess;