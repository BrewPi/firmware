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

#include <inttypes.h>

enum menuPages{
	MENU_TOP,
	MENU_PROFILE_SETTING,
	MENU_BEER_SETTING,
	MENU_FRIDGE_SETTING,
	MENU_PROFILE
};

#define MENU_TIMEOUT 10000ul

/*
 * Menu is the controller in a MVC triad, with TempControl providing the model and PiLink providing the view.  
 */

class TempControl; 
class PiLink;
class Menu{
	public:
	Menu(TempControl& control, PiLink& link) : tempControl(control), piLink(link) {};
	void pickSettingToChange(void);
	void pickMode(void);
	void pickBeerSetting(void);
	void pickFridgeSetting(void);
	
	~Menu(){};
	private:
	TempControl& tempControl;		
	PiLink& piLink;
};

extern Menu menu;

#endif /* MENU_H_ */