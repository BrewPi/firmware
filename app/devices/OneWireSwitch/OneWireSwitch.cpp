/*
 * File:   OneWireSwitch.cpp
 * Author: Elco
 *
 * Created on 15 december 2014, 21:38
 */

#include "OneWireSwitch.h"

OneWireSwitch::OneWireSwitch(){}

OneWireSwitch::~OneWireSwitch(){}

/*
 * Initializes this OneWire slave.
 * /param oneWire The oneWire bus the device is connected to
 * /param address The oneWire address of the device to use.
 */
void OneWireSwitch::init(OneWire *     oneWire,
                         DeviceAddress address)
{
    this -> oneWire = oneWire;

    memcpy(this -> address, address, sizeof(DeviceAddress));
}

DeviceAddress & OneWireSwitch::getDeviceAddress()
{
    return address;
}

bool OneWireSwitch::validAddress(OneWire *     oneWire,
                                 DeviceAddress deviceAddress)
{
    return deviceAddress[0] && (oneWire -> crc8(deviceAddress, 7) == deviceAddress[7]);
}


uint8_t OneWireSwitch::accessRead()    /* const */
{
#define ACCESS_READ 0xF5
    oneWire -> reset();
    oneWire -> select(address);
    oneWire -> write(ACCESS_READ);

    uint8_t data;
    data = oneWire -> read();

    return data;
}

/*
 *    Writes the state of all PIOs in one operation.
 *    /param b pio data - PIOA is bit 0 (lsb), PIOB is bit 1
 *    /param maxTries the maximum number of attempts before giving up.
 *    /return true on success
 */
bool OneWireSwitch::accessWrite(uint8_t b,
                                uint8_t maxTries)
{
#define ACCESS_WRITE 0x5A
#define ACK_SUCCESS 0xAA
#define ACK_ERROR 0xFF

    // b |= 0xFC;        /* Upper 6 bits should be set to 1's */
    uint8_t ack = 0;

    do{
        oneWire -> reset();
        oneWire -> select(address);
        oneWire -> write(ACCESS_WRITE);
        oneWire -> write(b);

        /* data is sent again, inverted to guard against transmission errors */
        oneWire -> write(~b);

        /* Acknowledgement byte, 0xAA for success, 0xFF for failure. */
        ack = oneWire -> read();

        if (ack == ACK_SUCCESS){
            oneWire -> read();    // status byte sent after ack
        }
    } while ((ack != ACK_SUCCESS) && (maxTries-- > 0));

    oneWire -> reset();

    return ack == ACK_SUCCESS;
}
