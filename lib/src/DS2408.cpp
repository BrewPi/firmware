/* 
 * File:   DS2408.cpp
 * Author: Elco
 * 
 * Created on 3 december 2014, 22:29
 */

#include "Brewpi.h"
#include "DS2408.h"
#include "Logger.h"

#ifdef BREWPI_DS2408
uint8_t DS2408::accessRead()    /* const */
{
    oneWire -> reset();
    oneWire -> select(address);
    oneWire -> write(ACCESS_READ);

    uint8_t data;
    data = oneWire -> read();

    return data;
}

bool DS2408::accessWrite(uint8_t b,
                         uint8_t maxTries)
{
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

void DS2408::update(){
    oneWire -> reset();
    oneWire -> select(address);

    // Compute the 1-Wire CRC16 and compare it against the received CRC.
    // Put everything in a buffer so we can compute the CRC easily.
    uint8_t buf[13];
    buf[0] = READ_PIO_REG;    // Read PIO Registers
    buf[1] = ADDRESS_PIO_STATE_LOWER;    // LSB address
    buf[2] = ADDRESS_UPPER;    // MSB address
    oneWire -> write_bytes(buf, 3);    // Write 3 cmd bytes
    oneWire -> read_bytes(buf+3, 10);  // Read 6 data bytes, 2 0xFF, 2 CRC16
    // last 2 bytes of buffer contain CRC
    uint16_t crc = *( (uint16_t*) &buf[11]);
    bool success = (oneWire -> crc16(buf, 11, crc) == 0);
    if(success){
        memcpy(&regCache, &buf[3], sizeof(regCache));
    }
    if(connected && !success){
        connected = false;
        char addressString[17];
        printBytes(address, 8, addressString);
        logWarningString(DS2408_DISCONNECTED, addressString);
    }
    else if (!connected && success){
        connected = true;
        char addressString[17];
        printBytes(address, 8, addressString);
        logInfoString(DS2408_CONNECTED, addressString);
    }
    oneWire -> reset();
}
#endif
