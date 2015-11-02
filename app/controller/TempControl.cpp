/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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



#include "Brewpi.h"
#include "temperatureFormats.h"
#include "Board.h"
#include "TempControl.h"
#include "PiLink.h"
#include "Ticks.h"
#include "TempSensorBasic.h"
#include "TempSensorMock.h"
#include "TempSensorDisconnected.h"
#include "ModeControl.h"
#include "EepromManager.h"
#include "fixstl.h"
#include "defaultDevices.h"

#define DISABLED_TEMP temp_t::disabled()

TempControl tempControl;

ControlConstants const ccDefaults PROGMEM =
{
    // Do Not change the order of these initializations!
    'C', // tempFormat
    5.0, // heater1_kp
    120, // heater1_ti
    0, // heater1_td
    2, // heater1_infilt
    4, // heater1_dfilt
    5.0, // heater2_kp
    120, // heater2_ti
    0, // heater2_td
    2, // heater2_infilt
    4, // heater2_dfilt
    3.0, // cooler_kp
    600, // cooler_ti
    60, // cooler_td
    2, // cooler_infilt
    4, // cooler_dfilt
    2.0, // beer2fridge_kp
    7200, // beer2fridge_ti
    50, // beer2fridge_td
    4, // beer2fridge_infilt
    4, // beer2fridge_dfilt
    120, // minCoolTime
    180, // minCoolIdleTime
    4, // heater1PwmPeriod
    4, // heater2PwmPeriod
    900, // coolerPwmPeriod
};

TempControl::TempControl()
{
    lastHeatTime = 0;
    lastCoolTime = 0;
    lastIdleTime = 0;
};

tcduration_t TempControl::timeSinceCooling(void)
{
    return ticks.timeSince(lastCoolTime);
}

tcduration_t TempControl::timeSinceHeating(void)
{
    return ticks.timeSince(lastHeatTime);
}

tcduration_t TempControl::timeSinceIdle(void)
{
    return ticks.timeSince(lastIdleTime);
}

void TempControl::loadDefaultSettings()
{
#if BREWPI_EMULATE
    setMode(MODE_BEER_CONSTANT);
#else
    setMode(MODE_TEST);
#endif

    setBeerTemp(DISABLED_TEMP); // start with no temp settings
    setFridgeTemp(DISABLED_TEMP);
}

void TempControl::storeConstants(eptr_t offset)
{
    eepromAccess.writeBlock(offset, (void *) &cc, sizeof(ControlConstants));
}

void TempControl::loadConstants(eptr_t offset)
{
    eepromAccess.readBlock((void *) &cc, offset, sizeof(ControlConstants));
    // TODO init Control actuators and filters
}

// write new settings to EEPROM to be able to reload them after a reset
// The update functions only write to EEPROM if the value has changed
void TempControl::storeSettings(eptr_t offset)
{
    eepromAccess.writeBlock(offset, (void *) &cs, sizeof(ControlSettings));

    storedBeerSetting = cs.beerSetting;
}

void TempControl::loadSettings(eptr_t offset)
{
    eepromAccess.readBlock((void *) &cs, offset, sizeof(ControlSettings));
    logDebug("loaded settings");

    storedBeerSetting = cs.beerSetting;

    setMode(cs.mode, true);    // force the mode update
    setBeerTemp(cs.beerSetting);
    setFridgeTemp(cs.fridgeSetting);
}

void TempControl::loadDefaultConstants(void)
{
    memcpy_P((void *) &tempControl.cc, (void *) &ccDefaults, sizeof(ControlConstants));
}


void TempControl::setMode(char newMode,
                          bool force)
{
    logDebug("TempControl::setMode from %c to %c", cs.mode, newMode);

    if (newMode == MODE_OFF)
    {
        setBeerTemp(DISABLED_TEMP);
        setFridgeTemp(DISABLED_TEMP);
    }
    eepromManager.storeTempSettings();
}

void TempControl::setBeerTemp(temp_t newTemp) {
    control.beer1Set->write(newTemp);
    cs.beerSetting = newTemp;

    if ((cs.mode != MODE_BEER_PROFILE) ||
            (storedBeerSetting - newTemp) > temp_t(0.25) ||
            (newTemp - storedBeerSetting) > temp_t(0.25))
    {
        // more than 1/4 degree C difference with EEPROM
        // Do not store settings every time in profile mode, because EEPROM has limited number of write cycles.
        // A temperature ramp would cause a lot of writes
        // If Raspberry Pi is connected, it will update the settings anyway. This is just a safety feature.
        eepromManager.storeTempSettings();
    }
}

void TempControl::setFridgeTemp(temp_t newTemp) {
    control.fridgeSet->write(newTemp);
    cs.fridgeSetting = newTemp;
    eepromManager.storeTempSettings();
}

void TempControl::applyFilterSetting(uint8_t setting, uint8_t * target){
    if(target == &cc.heater1_infilt){
        control.heater1Pid->setInputFilter(setting);
    }
    else if(target == &cc.heater1_dfilt){
        control.heater1Pid->setDerivativeFilter(setting);
    }
    else if(target == &cc.heater2_infilt){
        control.heater2Pid->setInputFilter(setting);
    }
    else if(target == &cc.heater2_dfilt){
        control.heater2Pid->setDerivativeFilter(setting);
    }
    else if(target == &cc.cooler_infilt){
        control.coolerPid->setInputFilter(setting);
    }
    else if(target == &cc.cooler_dfilt){
        control.coolerPid->setDerivativeFilter(setting);
    }
    else if(target == &cc.beer2fridge_infilt){
        control.beerToFridgePid->setInputFilter(setting);
    }
    else if(target == &cc.beer2fridge_dfilt){
        control.beerToFridgePid->setDerivativeFilter(setting);
    }
}

control_mode_t ModeControl_GetMode()
{
    return tempControl.getMode();
}

control_mode_t ModeControl_SetMode(control_mode_t mode)
{
    control_mode_t prev = tempControl.getMode();

    tempControl.setMode(mode, true);

    return prev;
}
