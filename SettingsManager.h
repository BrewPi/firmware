/*
 * SettingsManager.h
 *
 * Created: 22/04/2013 16:13:19
 *  Author: mat
 */ 

#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include "Brewpi.h"
#include <assert.h>
#include "EepromManager.h"
#include "DeviceManager.h"

/*
 * Manages the settings and devices for multiple carboys and multiple chambers.
 * This is the soul of brewpi.
 *
 * The manager hides the persistence of the settings, and uses the code closest to the settings to provide
 * useful defaults. 
 */
class SettingsManager
{
public:	
	/**
	 * Initialize settings. This attempts to read from persisted settings and apply settings from there.
	 * If that's not possible, defaults are used.
	 */
	static void loadSettings();
	
};

extern SettingsManager settingsManager;

#endif