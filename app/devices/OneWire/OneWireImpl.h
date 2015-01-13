#pragma once

#define ONEWIRE_DS2482

#ifdef ONEWIRE_DS2482

#include "DS2482.h"

class OneWireImpl : public DS2482 {
public:

    OneWireImpl(uint8_t config) : DS2482(config) {}
    virtual ~OneWireImpl(){};
};

#elif defined(ONEWIRE_PIN)

#include "OneWirePin.h"

class OneWireImpl : public OneWirePin {

    OneWireImpl(uint8_t config) : OneWirePin(config) {}
    virtual ~OneWireImpl(){};
};

#endif