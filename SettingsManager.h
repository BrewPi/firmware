/*
 * SettingsManager.h
 *
 * Created: 22/04/2013 16:13:19
 *  Author: mat
 */ 

#pragma once

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
	void init() {
		
		eepromManager.init();
		eptr_t pv = eepromManager.startSettings();		// begin iteration
		if (pv) {			
			// for now, assume 1 chamber. Will add a while loping later to support multiple chambers
			eepromManager.applySettings(pv);
			pv = eepromManager.nextSettings(pv);
			assert(pv==NULL);
		}
		else {
			piLink.debugMessage(PSTR("Settings not available - entering OFF mode."));
			
			// for multichamber, set number of chambers to 1
									
			tempControl.loadDefaultSettings();
			tempControl.loadDefaultConstants();			
			deviceManager.loadDefaultDevices();
		}
				
	}
	
	
};

extern SettingsManager settingsManager;

