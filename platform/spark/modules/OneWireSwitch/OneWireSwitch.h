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
    bool isConnected();

protected:
    OneWire* oneWire;
    DeviceAddress address;

public:    
    /*
     * Read all values at once, both current state and sensed values. The read performs data-integrity checks.
     * Returns a negative result if the device cannot be read successfully within the given number of tries.
     * The lower 4-bits are the values as described under PIO ACCESSS READ [F5h] in the ds2413 datasheet:	 
     * b0: PIOA state
     * b1: PIOA output latch state
     * b2: PIOB state
     * b3: PIOB output latch state
     */
    uint8_t accessRead(uint8_t maxTries = 3);

    /*
     * Writes the state of all PIOs in one operation.
     * /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1	 
     * /param maxTries the maximum number of attempts before giving up.
     * /return true on success
     */
    bool accessWrite(uint8_t b, uint8_t maxTries = 3);
};


