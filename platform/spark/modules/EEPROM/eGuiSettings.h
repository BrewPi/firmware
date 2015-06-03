/*
 * Copyright 2015 BrewPi/Elco Jacobs/Matthew McGowan.
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

#ifndef EGUISETTINGS_H
#define	EGUISETTINGS_H

extern "C" {
#include "d4d.h"
}

#include "flashee-eeprom.h"
#include "SparkEepromRegions.h"


// Reserve 32 bytes for touch calibration settings to have room for growth
union calibrationSettingsArea {
    D4D_TOUCHSCREEN_CALIB touchCalib;
    unsigned char reserved[32];
};

// relative addresses in flashee block
#define TOUCH_CALIB_ADDRESS 0
#define NEXT_ADDRESS (TOUCH_CALIB_ADDRESS + sizeof(calibrationSettingsArea)

class eGuiSettingsClass {
    Flashee::FlashDevice* flash;

public:

    void init() {
#if PLATFORM_ID==0        
        flash = Flashee::Devices::createAddressErase(4096 * EEPROM_EGUI_SETTINGS_START_BLOCK, 4096 * EEPROM_EGUI_SETTINGS_END_BLOCK);
#elif PLATFORM_ID==6
        flash = Flashee::Devices::createEepromDevice(EEPROM_EGUI_SETTINGS_START_BLOCK, EEPROM_EGUI_SETTINGS_END_BLOCK);
#else
#error Unknown Product ID
#endif
    };

    /**
     * Checks whether valid touch screen calibration data is stored in flash memory
     * and loads it in eGUI
     * @return true if valid data was found, false if not and calibration is needed
     */
    bool loadTouchCalib() {
        D4D_TOUCHSCREEN_CALIB calib;
        flash->read(&calib, TOUCH_CALIB_ADDRESS, sizeof (D4D_TOUCHSCREEN_CALIB));
        if (calib.ScreenCalibrated != 1) {
            return false;
        }
        D4D_TCH_SetCalibration(calib);
        return true;
    };

    /**
     * Stores current touch screen calibration data from eGUI to flash memory
     */
    void storeTouchCalib() {
        D4D_TOUCHSCREEN_CALIB calib = D4D_TCH_GetCalibration();
        flash->write(&calib, TOUCH_CALIB_ADDRESS, sizeof (D4D_TOUCHSCREEN_CALIB));
    };
};

extern eGuiSettingsClass eGuiSettings;

#endif	/* EGUISETTINGS_H */

