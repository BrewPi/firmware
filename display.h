#pragma once

#include "DisplayBase.h"
#include "LcdDisplay.h"

#if BREWPI_EMULATE || !BREWPI_LCD || !BREWPI_DISPLAY
typedef NullDisplay DisplayType;
#else
typedef LcdDisplay DisplayType;
#endif

extern DisplayType DISPLAY_REF display;

