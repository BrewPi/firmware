/*
 * FastDigitalPin.h
 *
 * Created: 25/03/2013 02:31:29
 *  Author: mat
 */ 

#pragma once

#ifndef FAST_DIGITAL_PIN
#define FAST_DIGITAL_PIN 1
#endif

// optimization required in order to resolve pin numbers to compile time constants.
#define USE_FAST_DIGITAL_PIN FAST_DIGITAL_PIN && __OPTIMIZE__

#if USE_FAST_DIGITAL_PIN
	#include "DigitalPin.h"
#else	
	#define fastPinMode pinMode
	#define fastDigitalWrite digitalWrite
	#define fastDigitalRead digitalRead
#endif

