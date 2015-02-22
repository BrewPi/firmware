#pragma once

#ifdef ARDUINO
#include "OneWirePin.h"
typedef OneWirePin OneWireDriver;

#else


#define ONEWIRE_DS2482
//#define ONEWIRE_PIN

#if defined(ONEWIRE_DS2482)

#include "DS2482.h"

typedef DS2482 OneWireDriver;

#elif defined(ONEWIRE_PIN)

#include "OneWirePin.h"

typedef OneWirePin OneWireDriver;

#else

#include "OneWireNull.h"

typedef OneWireNull OneWireDriver;

#endif

#endif