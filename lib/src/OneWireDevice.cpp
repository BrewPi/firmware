/*
 * File:   OneWireSwitch.cpp
 * Author: Elco
 *
 * Created on 15 december 2014, 21:38
 */

#include "../inc/OneWireDevice.h"
#include "../inc/OneWire.h"
#include "../inc/OneWireAddress.h"

/**
 * Constructor
 * /param oneWire_ The oneWire bus the device is connected to
 * /param address_ The oneWire address of the device to use.
 */
OneWireDevice::OneWireDevice(OneWire& oneWire_, const OneWireAddress& address_)
    : oneWire(oneWire_)
    , address(address_)
{
}

/**
 * Get the device address
 * @return device address
 */
OneWireAddress
OneWireDevice::getDeviceAddress() const
{
    return address;
}

/**
 * Checks if the address is valid by performing a crc8 check on it
 * @return bool, true if valid
 */
bool
OneWireDevice::validAddress() const
{
    const uint8_t* addr = address.asUint8ptr();

    return addr[0] && (OneWire::crc8(addr, 7) == addr[7]);
}
