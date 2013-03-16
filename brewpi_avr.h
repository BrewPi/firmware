/*
 * brewpi_avr.h
 *
 * Created: 15/03/2013 09:27:15
 *  Author: mat
 */ 

#ifndef BREWPI_AVR_H_
#define BREWPI_AVR_H_

#include "Display.h"
#include "LcdDisplay.h"


#if BREWPI_EMULATE
typedef NullDisplay DisplayType;
#else
typedef LcdDisplay DisplayType;
#endif



#endif /* BREWPI_AVR_H_ */