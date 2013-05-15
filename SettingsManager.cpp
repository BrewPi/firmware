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

#include "Brewpi.h"
#include "SettingsManager.h"
#include "TempControl.h"
#include "PiLink.h"
#include "TempSensorExternal.h"

void SettingsManager::loadSettings()
{
	DEBUG_MSG_3(PSTR("loading settings"));

#if !BREWPI_SIMULATE
	if (!eepromManager.applySettings())
	{
		tempControl.loadDefaultSettings();
		tempControl.loadDefaultConstants();
	
		deviceManager.setupUnconfiguredDevices();
		DEBUG_MSG_1(PSTR("EEPROM Settings not available. Starting in safe mode."));
	}
#else
	
	static ExternalTempSensor* ambient = new ExternalTempSensor(true);
	static ExternalTempSensor* beer = new ExternalTempSensor(true);
	static ExternalTempSensor* fridge = new ExternalTempSensor(true);
	
	tempControl.ambientSensor = ambient;
	tempControl.fridgeSensor->setSensor(fridge);
	tempControl.beerSensor->setSensor(beer);
		
#endif	
}

SettingsManager settingsManager;