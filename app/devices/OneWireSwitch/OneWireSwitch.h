/* 
 * File:   OneWireSlave.h
 * Author: Elco
 *
 * Created on 15 december 2014, 21:38
 */

#pragma once

#include "OneWire.h"
#include <string.h>

typedef uint8_t DeviceAddress[8];

class OneWireSwitch {
public:
    OneWireSwitch();
    OneWireSwitch(const OneWireSwitch& orig);
    virtual ~OneWireSwitch();

    void init(OneWire* oneWire, DeviceAddress address);
    DeviceAddress& getDeviceAddress();
    bool validAddress(OneWire* oneWire, DeviceAddress deviceAddress);

protected:
    OneWire* oneWire;
    DeviceAddress address;

public:    
    /*
     * Read all values at once, both current state and sensed values for the DS2413.
     * Output state of 8 pins for the DS2408.
     */
    uint8_t accessRead();

    /*
     * Writes the state of all PIOs in one operation.
     * /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1 for DS2413. All bits are used for DS2408
     * /param maxTries the maximum number of attempts before giving up.
     * /return true on success
     */
    bool accessWrite(uint8_t b, uint8_t maxTries = 3);
};


