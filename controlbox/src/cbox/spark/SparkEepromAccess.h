#pragma once
#include "../EepromAccess.h"
#include "eeprom_hal.h"
#include <cstdint>

namespace cbox {

class SparkEepromAccess : public EepromAccess {
private:
    static const uint16_t MAGIC_BYTES;

public:
    SparkEepromAccess() = default;
    ~SparkEepromAccess() = default;

    virtual uint8_t readByte(uint16_t offset) const override final
    {
        return HAL_EEPROM_Read(offset);
    }
    virtual void writeByte(uint16_t offset, uint8_t value) override final
    {
        HAL_EEPROM_Write(offset, value);
    }

    virtual void readBlock(void* target, uint16_t offset, uint16_t size) const override final
    {
        HAL_EEPROM_Get(offset, target, size);
    }
    virtual void writeBlock(uint16_t target, const void* source, uint16_t size) override final
    {
        HAL_EEPROM_Put(target, source, size);
    }
    virtual uint16_t length() const override final
    {
        return HAL_EEPROM_Length();
    }

    virtual void clear() override final
    {
        HAL_EEPROM_Clear();
    }
};

} // end namespace cbox
