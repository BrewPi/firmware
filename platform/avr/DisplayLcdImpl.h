/*
 * DisplayLcdImpl.h
 *
 * Created: 07/01/2015 06:01:02
 *  Author: mat
 */ 

#pragma once

#include "SpiLcd.h"
#include "OLEDFourBit.h"
#include "NullLcdDriver.h"

#if BREWPI_EMULATE || !BREWPI_LCD || !ARDUINO
typedef NullLcdDriver LcdDriver;
#elif !BREWPI_SHIFT_LCD
typedef OLEDFourBit LcdDriver;
#else
typedef SpiLcd		LcdDriver;
#endif

