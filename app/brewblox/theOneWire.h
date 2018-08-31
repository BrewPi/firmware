#pragma once

#include "OneWire.h"

// create a static OneWire object on first use and return a reference to it
OneWire&
theOneWire();
