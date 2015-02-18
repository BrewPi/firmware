/**
 ******************************************************************************
 * @file    OneWireNull.h
 * @authors mat
 * @date    05 February 2015
 ******************************************************************************
*/

#ifndef ONEWIRENULL_H
#define	ONEWIRENULL_H

class OneWireNull {
public:
    OneWireNull(uint8_t pin) {}

    uint8_t init() { return 0; }
    uint8_t pinNr() { return 0; }
    
    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    uint8_t reset(void) { return 0; }

    void write(uint8_t v, uint8_t power = 0) {}

    void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0) {}

    uint8_t read(void) { return 0; }

    void read_bytes(uint8_t *buf, uint16_t count) {}

    // Write a bit. The bus is always left powered at the end, see
    // note in write() about that.
    void write_bit(uint8_t v) {}

    // Read a bit.
    uint8_t read_bit(void) { return 0; }
    
};


#endif	/* ONEWIRENULL_H */

