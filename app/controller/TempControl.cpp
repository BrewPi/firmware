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

TempControl                   tempControl;

#if TEMP_CONTROL_STATIC

// These sensors are switched out to implement multi-chamber.
TempSensorBasic * TempControl::beerSensor = &defaultTempSensor;
TempSensorBasic * TempControl::fridgeSensor = &defaultTempSensor;
TempSensorBasic * TempControl::ambientSensor = &defaultTempSensor;
Actuator *        TempControl::light         = &defaultActuator;
Actuator *        TempControl::fan           = &defaultActuator;
ActuatorOnOff *   TempControl::chamberCoolerLimiter;
BoolActuator     cameraLightState;
ActuatorPwm *     TempControl::chamberHeater;
ActuatorPwm *     TempControl::chamberCooler;
ActuatorPwm *     TempControl::beerHeater;
AutoOffActuator   TempControl::cameraLight(600, &cameraLightState);    // timeout 10 min
Sensor<bool> *    TempControl::door = &defaultSensor;

// Control parameters
ControlConstants TempControl::cc;
ControlSettings  TempControl::cs;
ControlVariables TempControl::cv;

// State variables
states TempControl::state;
bool   TempControl::doorOpen;

// keep track of beer setting stored in EEPROM
temp_t TempControl::storedBeerSetting;

// Timers
tcduration_t    TempControl::lastIdleTime;
tcduration_t    TempControl::lastHeatTime;
tcduration_t    TempControl::lastCoolTime;

temp_long_t TempControl::fridgeIntegrator;
#endif

ControlConstants const ccDefaults PROGMEM =
{
    // Do Not change the order of these initializations!

    /* tempFormat */
    'C',

    /* tempSettingMin */
    1.0,

    /* tempSettingMax */
    127.0,

    // control defines, also in fixed point format (7 int bits, 9 frac bits), so multiplied by 2^9=512

    /* Kp */
    5.0,

    /* Ki */
    0.25,

    /* Kd */
    1.5,

    // Stay Idle when fridge temperature is in this range

    /* idleRangeHigh */
    0.1,

    /* idleRangeLow */
    -0.1,

    // Set filter coefficients. This is the b value. See FilterFixed.h for delay times.
    // The delay time is 3.33 * 2^b * number of cascades

    /* fridgeFastFilter */
    0u,

    /* fridgeSlowFilter */
    2u,

    /* fridgeSlopeFilter */
    2u,

    /* beerFastFilter */
    1u,

    /* beerSlowFilter */
    2u,

    /* beerSlopeFilter */
    2u,

    /* lightAsHeater */
    0,

    /* rotaryHalfSteps */
    0,

    /* pidMax */
    10.0,

    /* heatPwmPeriod */
    4, // 4 seconds

    /* coolPwmPeriod */
    600, // 1 minutes

    /* minCoolTime*/
    120, // 2 minutes

    /* minCoolIdleTime*/
    180, // 3 minutes

    /* fridgePwmKpHeat */
    20.0,

    /* fridgePwmKiHeat */
    5.0,

    /* fridgePwmKdHeat */
    -0.5,

    /* fridgePwmKpCool */
    20.0,

    /* fridgePwmKiCool */
    2.0,

    /* fridgePwmKdCool */
    -0.5,

    /* beerPwmKpHeat */
    10.0,

    /* beerPwmKiHeat */
    4.0,

    /* beerPwmKdHeat */
    -1.0
};

TempControl::TempControl()
{
};


void TempControl::init(void)
{
    state   = IDLE;
    cs.mode = MODE_OFF;

    cameraLight.setActive(false);

    if (chamberHeater == NULL)
    {
        chamberHeater = new ActuatorPwm(&defaultActuator, ccDefaults.heatPwmPeriod);
    }

    if (chamberCooler == NULL)
    {
        chamberCoolerLimiter = new ActuatorOnOff(&defaultActuator, ccDefaults.minCoolTime, ccDefaults.minCoolIdleTime); // time limited actuator
        chamberCooler = new ActuatorPwm(chamberCoolerLimiter, ccDefaults.coolPwmPeriod);
    }

    if (beerHeater == NULL)
    {
        beerHeater = new ActuatorPwm(&defaultActuator, ccDefaults.heatPwmPeriod);
    }

    updateTemperatures();

    // Do not allow heating/cooling directly after reset.
    // A failing script + CRON + Arduino uno (which resets on serial connect) could damage the compressor
    // For test purposes, set these to -3600 to eliminate waiting after reset
    lastHeatTime = 0;
    lastCoolTime = 0;
    fridgeIntegrator = 0;
}



void TempControl::updateTemperatures(void)
{
    // TODO
    // Read ambient sensor to keep the value up to date. If no sensor is connected, this does nothing.
    // This prevents a delay in serial response because the value is not up to date.
    if (ambientSensor -> read() == TEMP_SENSOR_DISCONNECTED)
    {
        ambientSensor -> init();    // try to reconnect a disconnected, but installed sensor
    }
}

void TempControl::updatePID(void)
{
}

void TempControl::updateState(void)
{
    /* // update state
    bool stayIdle    = false;
    bool newDoorOpen = door -> sense();

    if (newDoorOpen != doorOpen)
    {
        doorOpen = newDoorOpen;

        piLink.printFridgeAnnotation(PSTR("Fridge door %S"), doorOpen ? PSTR("opened") : PSTR("closed"));
    }

    if (cs.mode == MODE_OFF)
    {
        state    = STATE_OFF;
        stayIdle = true;
    }

    // stay idle when one of the required sensors is disconnected, or the fridge setting is INVALID_TEMP
    if (isDisabledOrInvalid(cs.fridgeSetting) ||!fridgeSensor -> isConnected()
            || (!beerSensor -> isConnected() && tempControl.modeIsBeer()))
    {
        state    = IDLE;
        stayIdle = true;
    }

    temp_t     fridgeFast   = fridgeSensor -> readFastFiltered();
    temp_t     beerFast     = beerSensor -> readFastFiltered();
    ticks_seconds_t secs         = ticks.seconds();

    switch (state)
    {
        case IDLE :
        case STATE_OFF :
        {
            lastIdleTime = secs;

            // set waitTime to zero. It will be set to the maximum required waitTime below when wait is in effect.
            if (stayIdle)
            {
                break;
            }

            if (fridgePidResultHeat() > 0)
            {
                if (tempControl.chamberHeater -> getBareActuator() != &defaultActuator)
                {
                    state = HEATING;
                }
            }
            else if (fridgePidResultCool() < 0)
            {
                if (tempControl.chamberCooler -> getBareActuator() != &defaultActuator)
                {
                    state = COOLING;
                }
            }
            else
            {
                state = IDLE;    // within IDLE range, always go to IDLE

                break;
            }
        }

        break;

        case COOLING :
        {
            if (tempControl.chamberCooler -> getBareActuator() == &defaultActuator)
            {
                state = IDLE;    // cooler uninstalled
                break;
            }
            if (chamberCooler->getPwm() == 0) // set state to IDLE when cooler PWM has gone to zero
            {
                state = IDLE;
                break;
            }
            lastCoolTime    = secs;
        }

        break;

        case HEATING :
        {
            if (tempControl.chamberHeater -> getBareActuator() == &defaultActuator)
            {
                state = IDLE;    // heater uninstalled
                break;
            }
            if (chamberHeater->getPwm() == 0) // set state to IDLE when heater PWM has gone to zero
            {
                state = IDLE;
                break;
            }
            lastHeatTime    = secs;
        }

        break;

        case DOOR_OPEN :
            break;    // do nothing
    }*/
}


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

    cs.beerSetting   = DISABLED_TEMP;            // start with no temp settings
    cs.fridgeSetting = DISABLED_TEMP;
}

void TempControl::storeConstants(eptr_t offset)
{
    eepromAccess.writeBlock(offset, (void *) &cc, sizeof(ControlConstants));
}

void TempControl::loadConstants(eptr_t offset)
{
    eepromAccess.readBlock((void *) &cc, offset, sizeof(ControlConstants));
    chamberHeater -> setPeriod(cc.heatPwmPeriod);
    beerHeater -> setPeriod(cc.heatPwmPeriod);
    chamberCooler -> setPeriod(cc.coolPwmPeriod);
    chamberCoolerLimiter -> setTimes(cc.minCoolTime, cc.minCoolIdleTime);
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
}

void TempControl::loadDefaultConstants(void)
{
    memcpy_P((void *) &tempControl.cc, (void *) &ccDefaults, sizeof(ControlConstants));
}


void TempControl::setMode(char newMode,
                          bool force)
{
    logDebug("TempControl::setMode from %c to %c", cs.mode, newMode);

    if ((newMode != cs.mode))
    {
        state = IDLE;
        force = true;
    }

    if (force)
    {
        cs.mode = newMode;

        if (newMode == MODE_OFF)
        {
            cs.beerSetting   = DISABLED_TEMP;
            cs.fridgeSetting = DISABLED_TEMP;
        }

        eepromManager.storeTempSettings();
    }
}

temp_t TempControl::getBeerTemp(void)
{
    if (beerSensor -> isConnected())
    {
        return beerSensor -> read();
    }
    else
    {
        return temp_t::invalid();
    }
}

temp_t TempControl::getBeerSetting(void)
{
    return cs.beerSetting;
}

temp_t TempControl::getFridgeTemp(void)
{
    if (fridgeSensor -> isConnected())
    {
        return fridgeSensor -> read();
    }
    else
    {
        return temp_t::invalid();
    }
}

temp_t TempControl::getFridgeSetting(void)
{
    return cs.fridgeSetting;
}

void TempControl::setBeerTemp(temp_t newTemp)
{
    temp_t oldBeerSetting = cs.beerSetting;

    cs.beerSetting = newTemp;

    updatePID();
    updateState();

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

void TempControl::setFridgeTemp(temp_t newTemp)
{
    cs.fridgeSetting = newTemp;

    updatePID();
    updateState();
    eepromManager.storeTempSettings();
}

bool TempControl::stateIsCooling(void)
{
    return state == COOLING;
}

bool TempControl::stateIsHeating(void)
{
    return state == HEATING;
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
