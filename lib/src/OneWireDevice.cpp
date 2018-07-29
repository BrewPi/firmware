/*
 * File:   OneWireSwitch.cpp
 * Author: Elco
 *
 * Created on 15 december 2014, 21:38
 */

#include "../inc/OneWireDevice.h"
#include "OneWire.h"
#include "OneWireAddress.h"
#include "string.h" // for memcpy

/**
 * Constructor
 * /param oneWire_ The oneWire bus the device is connected to
 * /param address_ The oneWire address of the device to use.
 */
OneWireDevice::OneWireDevice(OneWire * oneWire_, OneWireAddress address_)
{
    this -> oneWire = oneWire_;
    this -> address = address_;
}

/**
 * Get the device address
 * @return device address
 */
OneWireAddress OneWireDevice::getDeviceAddress()
{
    return address;
}

/**
 * Checks if the address is valid by performing a crc8 check on it
 * @return bool, true if valid
 */
bool OneWireDevice::validAddress(OneWire *     oneWire_,
                                 OneWireAddress address_)
{
    return address_[0] && (oneWire -> crc8(address_, 7) == address_[7]);
}

