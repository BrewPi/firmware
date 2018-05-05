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
#ifndef stringify
#define stringify(s) _stringifyDo(s)
#define _stringifyDo(s) #s
#endif

/*
 * Defines global config for the brewpi project. This file is included in every file in the project to ensure conditional
 * compilation directives are recognized.
 * 
 * ConfigDefault.h contains the default settings, and produces a standard Hex file.
 * To customize the build, users may add settings to Config.h, or define symbols in the project.
 */


#include <Config.h>
#include "ConfigDefault.h"

#include <AppConfig.h>
#include "AppConfigDefault.h"

#define VERSION_STRING "0.5.5"
