/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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

#ifndef MENU_H_ 
#define MENU_H_

#include "brewpi_avr.h"

#ifndef BREWPI_MENU
#define BREWPI_MENU 1
#endif


#if BREWPI_MENU

#include <inttypes.h>

enum menuPages{
	MENU_TOP,
	MENU_PROFILE_SETTING,
	MENU_BEER_SETTING,
	MENU_FRIDGE_SETTING,
	MENU_PROFILE
};

#define MENU_TIMEOUT 10000ul

class Menu{
	public:
	Menu(){};
	static void pickSettingToChange(void);
	static void pickMode(void);
	static void pickBeerSetting(void);
	static void pickFridgeSetting(void);
	
	~Menu(){};
};

extern Menu menu;

#endif

#endif /* MENU_H_ */