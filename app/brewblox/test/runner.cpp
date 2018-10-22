#define CATCH_CONFIG_MAIN

#include "deviceid_hal.h"
#include "testinfo.h"
#include <catch.hpp>

TestInfo testInfo;

void
handleReset(bool)
{
    ++testInfo.rebootCount;
};

/* mock implementation for device ID */
unsigned
HAL_device_ID(uint8_t* dest, unsigned destLen)
{
    if (dest != nullptr && destLen != 0)
        memset(dest, 0, destLen);
    return destLen;
}