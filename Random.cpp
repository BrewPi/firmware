/*
 * Random.cpp
 *
 * Created: 11/04/2013 10:29:11
 *  Author: mat
 */ 

#ifndef BREWPI_RANDOM
#define BREWPI_RANDOM 1
#endif

#if BREWPI_RANDOM
// Have to include this separately from Arduino.h, since it redefines makeWord()
#include <WMath.cpp>

#endif