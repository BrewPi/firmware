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
    5.0, // cooler_kp
    1800, // cooler_ti
    60, // cooler_td
    2, // cooler_infilt
    4, // cooler_dfilt
    2.0, // beer2fridge_kp
    7200, // beer2fridge_ti
    50, // beer2fridge_td
    4, // beer2fridge_infilt
    4, // beer2fridge_dfilt
    10, // beer2fridge_pidMax
    120, // minCoolTime
    180, // minCoolIdleTime
    4, // heater1PwmPeriod
    4, // heater2PwmPeriod
    1200, // coolerPwmPeriod
    1800, // mutexDeadTime
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
    setMode(MODE_BEER_CONSTANT, false);
#else
    setMode(MODE_TEST, false);
#endif

    setBeerTemp(DISABLED_TEMP, false); // start with no temp settings
    setFridgeTemp(DISABLED_TEMP, false);
}

void TempControl::storeConstants(eptr_t offset)
{
    eepromAccess.writeBlock(offset, (void *) &cc, sizeof(ControlConstants));
    updateConstants();
}

void TempControl::loadConstants(eptr_t offset)
{
    eepromAccess.readBlock((void *) &cc, offset, sizeof(ControlConstants));
    updateConstants();
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

    setMode(cs.mode, false);
    setBeerTemp(cs.beerSetting, false);
    setFridgeTemp(cs.fridgeSetting, false);
}


void TempControl::loadDefaultConstants(void)
{
    memcpy_P((void *) &tempControl.cc, (void *) &ccDefaults, sizeof(ControlConstants));
    updateConstants();
}


void TempControl::setMode(char newMode,
                          bool store)
{
    logDebug("TempControl::setMode from %c to %c", cs.mode, newMode);

    if (newMode == MODE_OFF)
    {
        setBeerTemp(DISABLED_TEMP, true);
        setFridgeTemp(DISABLED_TEMP, true);
        control.heater1Pid->disable(true);
        control.coolerPid->disable(true);
        control.beerToFridgePid->disable(false);
    }
    else if (newMode == MODE_BEER_CONSTANT || newMode == MODE_BEER_PROFILE){
        control.heater1Pid->enable();
        control.coolerPid->enable();
        control.beerToFridgePid->enable();
    }
    else if (newMode == MODE_FRIDGE_CONSTANT){
        setBeerTemp(DISABLED_TEMP, true);
        control.heater1Pid->enable();
        control.coolerPid->enable();
        control.beerToFridgePid->disable(false);
    }
    cs.mode = newMode;
    if(store){
        eepromManager.storeTempSettings();
    }
}

void TempControl::setBeerTemp(temp_t newTemp, bool store) {
    control.beer1Set->write(newTemp);
    cs.beerSetting = newTemp;

    if (store && ((cs.mode != MODE_BEER_PROFILE) ||
            (storedBeerSetting - newTemp) > temp_t(0.25) ||
            (newTemp - storedBeerSetting) > temp_t(0.25)))
    {
        // more than 1/4 degree C difference with EEPROM
        // Do not store settings every time in profile mode, because EEPROM has limited number of write cycles.
        // A temperature ramp would cause a lot of writes
        // If Raspberry Pi is connected, it will update the settings anyway. This is just a safety feature.
        eepromManager.storeTempSettings();
    }
}

void TempControl::setFridgeTemp(temp_t newTemp, bool store) {
    control.fridgeSet->write(newTemp);
    cs.fridgeSetting = newTemp;
    if(store){
        eepromManager.storeTempSettings();
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

// loads settings in tempControl to control, overwriting all existing settings
// This is temporary fix, until settings are stored elsewhere
// Overwriting with the same value should not have any side effects
// updating all settings when only one has changed is a temporary fix. TODO
void TempControl::updateConstants()
{
    //settings for heater 1
    control.heater1Pid->Kp = cc.heater1_kp;
    control.heater1Pid->Ti = cc.heater1_ti;
    control.heater1Pid->Td = cc.heater1_td;

    //settings for heater 2
    control.heater2Pid->Kp = cc.heater2_kp;
    control.heater2Pid->Ti = cc.heater2_ti;
    control.heater2Pid->Td = cc.heater2_td;

    //settings for cooler
    control.coolerPid->Kp = cc.cooler_kp;
    control.coolerPid->Ti = cc.cooler_ti;
    control.coolerPid->Td = cc.cooler_td;

    //settings for beer2fridge PID
    control.beerToFridgePid->Kp = cc.beer2fridge_kp;
    control.beerToFridgePid->Ti = cc.beer2fridge_ti;
    control.beerToFridgePid->Td = cc.beer2fridge_td;

    control.cooler->setPeriod(cc.coolerPwmPeriod);
    control.heater1->setPeriod(cc.heater1PwmPeriod);
    control.heater2->setPeriod(cc.heater2PwmPeriod);

    control.coolerTimeLimited->setTimes(cc.minCoolTime, cc.minCoolIdleTime);

    control.heater1Pid->setInputFilter(cc.heater1_infilt);
    control.heater1Pid->setDerivativeFilter(cc.heater1_dfilt);
    control.heater2Pid->setInputFilter(cc.heater2_infilt);
    control.heater2Pid->setDerivativeFilter(cc.heater2_dfilt);
    control.coolerPid->setInputFilter(cc.cooler_infilt);
    control.coolerPid->setDerivativeFilter(cc.cooler_dfilt);
    control.beerToFridgePid->setInputFilter(cc.beer2fridge_infilt);
    control.beerToFridgePid->setDerivativeFilter(cc.beer2fridge_dfilt);
    control.fridgeSetPointActuator->setMin(-cc.beer2fridge_pidMax);
    control.fridgeSetPointActuator->setMax(cc.beer2fridge_pidMax);
    control.mutex->setDeadTime(cc.mutexDeadTime * 1000);
}
