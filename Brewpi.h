/*
 * brewpi_avr.h
 *
 * Created: 15/03/2013 09:27:15
 *  Author: mat
 */ 

#ifndef BREWPI_AVR_H
#define BREWPI_AVR_H


// have to use two levels of macro expansion to convert a symbol to a string. see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
#define stringify(s) _stringifyDo(s)
#define _stringifyDo(s) #s


/*
 * Defines global config for the brewpi project. This file is included in every file in the project to ensure conditional
 * compilation directives are recognized.
 * 
 * ConfigDefault.h contains the default settings, and produces a standard Hex file.
 * To customize the build, users may add settings to ConfigLocal.h, or define symbols in the project.
 */

#include "ConfigLocal.h"
#include "ConfigDefault.h"

#include <Arduino.h>

#endif