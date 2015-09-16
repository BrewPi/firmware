#pragma once

#include "Platform.h"

#ifdef ARDUINO
#include "OneWirePin.h"
typedef OneWirePin OneWireDriver;

#else

#if defined(ONEWIRE_DS248X)

#include "DS248x.h"

typedef DS248x OneWireDriver;

#elif defined(ONEWIRE_PIN)

#include "OneWirePin.h"

typedef OneWirePin OneWireDriver;

#elif defined(ONEWIRE_NULL)

#include "OneWireNull.h"

typedef OneWireNull OneWireDriver;

#else

#error No OneWire implementation defined

#endif

#endif