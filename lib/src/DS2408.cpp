/*
 * Copyright 2018 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DS2408.h"
#include "Logger.h"
#include <cstring>

uint8_t
DS2408::accessRead() /* const */
{
    oneWire.reset();
    oneWire.select(address.asUint8ptr());
    oneWire.write(ACCESS_READ);

    uint8_t data;
    data = oneWire.read();

    return data;
}

bool
DS2408::accessWrite(uint8_t b,
                    uint8_t maxTries)
{
    // b |= 0xFC;        /* Upper 6 bits should be set to 1's */
    uint8_t ack = 0;

    do {
        oneWire.reset();
        oneWire.select(address.asUint8ptr());
        oneWire.write(ACCESS_WRITE);
        oneWire.write(b);

        /* data is sent again, inverted to guard against transmission errors */
        oneWire.write(~b);

        /* Acknowledgement byte, 0xAA for success, 0xFF for failure. */
        ack = oneWire.read();

        if (ack == ACK_SUCCESS) {
            oneWire.read(); // status byte sent after ack
        }
    } while ((ack != ACK_SUCCESS) && (maxTries-- > 0));

    oneWire.reset();

    return ack == ACK_SUCCESS;
}

void
DS2408::update()
{
    oneWire.reset();
    oneWire.select(address.asUint8ptr());

    // Compute the 1-Wire CRC16 and compare it against the received CRC.
    // Put everything in one buffer so we can compute the CRC easily.
    uint8_t buf[13];

    buf[0] = READ_PIO_REG;            // Read PIO Registers
    buf[1] = ADDRESS_PIO_STATE_LOWER; // LSB address
    buf[2] = ADDRESS_UPPER;           // MSB address
    oneWire.write_bytes(buf, 3);      // Write 3 cmd bytes
    oneWire.read_bytes(&buf[3], 10);  // Read 6 data bytes, 2 0xFF, CRC16

    bool success = oneWire.check_crc16(buf, 11, &buf[11]);

    if (success) {
        std::memcpy(&m_regCache, &buf[3], sizeof(m_regCache));
    }

    if (success != m_connected) {
        if (success) {
            CL_LOG_INFO("DS2408 connected ") << address.toString();
        } else {
            CL_LOG_WARN("DS2408 disconnected ") << address.toString();
        }
        m_connected = success;
    }

    oneWire.reset();
}
