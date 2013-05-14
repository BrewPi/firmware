
/**
 * To use the display, include this file. It takes care of setting DisplayType to the appropriate type of display according
 * to the compile-time config.
 */
#pragma once

#include "DisplayBase.h"
#include "DisplayLcd.h"

#if BREWPI_EMULATE || !BREWPI_LCD
typedef NullDisplay DisplayType;
#else
typedef LcdDisplay DisplayType;
#endif

extern DisplayType DISPLAY_REF display;

