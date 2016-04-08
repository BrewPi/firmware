#pragma once

#include "ArrayEepromAccess.h"

typedef ArrayEepromAccess<1024> EepromAccess;

#include "EepromTypes.h"
extern EepromAccess eepromAccess;

