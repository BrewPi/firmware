/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// have to use two levels of macro expansion to convert a symbol to a string. see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
#define stringify(s) _stringifyDo(s)
#define _stringifyDo(s) #s


// Most pins are only conditionally defined here, allowing definitions to be provided in Config.h for
// local overrides
enum {
	BREWPI_SHIELD_DIY,
	BREWPI_SHIELD_REV_A,
	BREWPI_SHIELD_REV_C,
	BREWPI_SHIELD_SPARK_V1,
	BREWPI_SHIELD_SPARK_V2
};

#define BREWPI_BOARD_LEONARDO 'l'
#define BREWPI_BOARD_STANDARD 's'
#define BREWPI_BOARD_MEGA 'm'
#define BREWPI_BOARD_SPARKCORE 'x'
#define BREWPI_BOARD_PHOTON 'y'
#define BREWPI_BOARD_UNKNOWN '?'

bool platform_init();

/*
 * Defines global config for the brewpi project. This file is included in every file in the project to ensure conditional
 * compilation directives are recognized.
 * 
 * ConfigDefault.h contains the default settings, and produces a standard Hex file.
 * To customize the build, users may add settings to Config.h, or define symbols in the project.
 */

#ifdef ARDUINO
#include "Config.h"
#else
#include <Config.h>                     // use search path rather than current directory, so that config.h is found elsewhere
#endif
#include "ConfigDefault.h"

#ifdef ARDUINO
#include "AppConfig.h"
#else
#include <AppConfig.h>
#endif
#include "AppConfigDefault.h"

