#pragma once

#include "OneWireLowLevelInterface.h"

class OneWireNull : public OneWireLowLevelInterface {
public:
    OneWireNull(uint8_t pin){};
    virtual ~OneWireNull() = default;

    virtual bool init() override final { return false; }
    virtual uint8_t pinNr() override final { return 0; }

    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    virtual bool reset(void) override final { return true; }

    virtual void write(uint8_t v, uint8_t power = 0) override final {}

    // virtual void write_bytes(const void* buf, uint16_t count, bool power = 0) override final {}

    virtual uint8_t read(void) override final { return 0; }

    // virtual void read_bytes(uint8_t* buf, uint16_t count) override final {}

    // Write a bit. The bus is always left powered at the end, see
    // note in write() about that.
    virtual void write_bit(uint8_t v) override final {}

    // Read a bit.
    virtual uint8_t read_bit(void) override final { return 0; }

    virtual uint8_t search_triplet(uint8_t*, uint8_t*, uint8_t*) override final { return 0; }
};
