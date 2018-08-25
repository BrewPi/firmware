#pragma once

#include "EepromTypes.h"
#include "application.h"

class SparkEepromAccess {
private:
    static const uint16_t MAGIC_BYTES;

public:
    SparkEepromAccess() = default;
    ~SparkEepromAccess() = default;

    static uint8_t readByte(eptr_t offset)
    {
        return EEPROM.read(offset);
    }
    static void writeByte(eptr_t offset, uint8_t value)
    {
        EEPROM.write(offset, value);
    }

    template <typename T>
    static T& get(eptr_t offset, T& t)
    {
        return EEPROM.get<T>(offset, t);
    }

    template <typename T>
    static const T& put(eptr_t offset, T& t)
    {
        return EEPROM.put<T>(offset, t);
    }

    static uint16_t length()
    {
        return EEPROM.length();
    }

    static void clear()
    {
        EEPROM.clear();
    }

    static void readBlock(void* target, const eptr_t offset, const uint16_t size)
    {
        EEPtr idx = offset;
        EEPtr end = offset + size;
        unsigned char* t = reinterpret_cast<unsigned char*>(target);
        while (idx < end) {
            *(t++) = *(idx++);
        }
    }

    static void writeBlock(eptr_t target, const void* source, uint16_t size)
    {
        EEPtr idx = target;
        EEPtr end = target + size;
        unsigned char const* s = reinterpret_cast<unsigned char const*>(source);
        while (idx < end) {
            *(idx++) = *(s++);
        }
    }
};
