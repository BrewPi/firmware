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
#define EGUISETTINGS_H

extern "C" {
#include "d4d.h"
}

#include "EepromAccessImpl.h"
#include "EepromFormat.h"

class eGuiSettingsClass {
public:
    /**
     * Checks whether valid touch screen calibration data is stored in flash memory
     * and loads it in eGUI
     * @return true if valid data was found, false if not and calibration is needed
     */
    bool loadTouchCalib()
    {
        D4D_TOUCHSCREEN_CALIB calib;
        eepromAccess.get(offsetof(EepromFormat, eGuiSettings), calib);
        if (calib.ScreenCalibrated != 1) {
            return false;
        }
        D4D_TCH_SetCalibration(calib);
        return true;
    };

    /**
     * Stores current touch screen calibration data from eGUI to flash memory
     */
    void storeTouchCalib()
    {
        D4D_TOUCHSCREEN_CALIB calib = D4D_TCH_GetCalibration();
        eepromAccess.put(offsetof(EepromFormat, eGuiSettings), calib);
    };
};

extern eGuiSettingsClass eGuiSettings;

#endif /* EGUISETTINGS_H */
