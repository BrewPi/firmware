/*
 * Copyright 2012 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later v7ersion.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "brewpi_avr.h"
#include "SettingsManager.h"
#include "TempControl.h"
#include "PiLink.h"

void SettingsManager::loadSettings()
{
	DEBUG_MSG(PSTR("loading settings"));
	eepromManager.init();

	// for multichamber, set number of chambers to 1
	tempControl.loadDefaultSettings();
	tempControl.loadDefaultConstants();
	
	if (!eepromManager.applySettings())
	{
		deviceManager.setupUnconfiguredDevices();
		piLink.debugMessage(PSTR("EEPROM Settings not available. Starting in safe mode."));
	}
}

SettingsManager settingsManager;