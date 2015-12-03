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

#pragma once

#include "Brewpi.h"
#include "temperatureFormats.h"
#include "Platform.h"
#include "EepromTypes.h"
#include "ModeControl.h"
#include "Ticks.h"
#include "Control.h"

// These two structs are stored in and loaded from EEPROM
struct ControlSettings {
    control_mode_t mode;
    temp_t beerSetting;
    temp_t fridgeSetting;
};

struct ControlConstants {
    char tempFormat;

    //settings for heater 1
    temp_long_t heater1_kp;
    uint16_t heater1_ti;
    uint16_t heater1_td;
    uint8_t heater1_infilt;
    uint8_t heater1_dfilt;

    //settings for heater 2
    temp_long_t heater2_kp;
    uint16_t heater2_ti;
    uint16_t heater2_td;
    uint8_t heater2_infilt;
    uint8_t heater2_dfilt;

    //settings for cooler
    temp_long_t cooler_kp;
    uint16_t cooler_ti;
    uint16_t cooler_td;
    uint8_t cooler_infilt;
    uint8_t cooler_dfilt;

    //settings for beer2fridge PID
    temp_long_t beer2fridge_kp;
    uint16_t beer2fridge_ti;
    uint16_t beer2fridge_td;
    uint8_t beer2fridge_infilt;
    uint8_t beer2fridge_dfilt;
    temp_t beer2fridge_pidMax;

    uint16_t minCoolTime;
    uint16_t minCoolIdleTime;
    uint16_t heater1PwmPeriod;
    uint16_t heater2PwmPeriod;
    uint16_t coolerPwmPeriod;
    uint16_t mutexDeadTime;
};

#define EEPROM_TC_SETTINGS_BASE_ADDRESS 0
#define EEPROM_CONTROL_SETTINGS_ADDRESS (EEPROM_TC_SETTINGS_BASE_ADDRESS+sizeof(uint8_t))
#define EEPROM_CONTROL_CONSTANTS_ADDRESS (EEPROM_CONTROL_SETTINGS_ADDRESS+sizeof(ControlSettings))

enum states {
    IDLE,						// 0
    STATE_OFF,					// 1
    DOOR_OPEN,					// 2 used by the Display only
    HEATING,					// 3
    COOLING,					// 4
    NUM_STATES                  // 5
};

class TempControl {
public:

    TempControl();
    ~TempControl() {
    }
    ;

    void loadSettings(eptr_t offset);
    void storeSettings(eptr_t offset);
    void loadDefaultSettings(void);
    void loadConstants(eptr_t offset);
    void storeConstants(eptr_t offset);
    void loadDefaultConstants(void);

    void loadSettingsAndConstants(void);
    void updateConstants(void); // copy tempControl to control

    tcduration_t timeSinceCooling(void);
    tcduration_t timeSinceHeating(void);
    tcduration_t timeSinceIdle(void);

    temp_t getBeerTemp(void) {
        return control.beer1Sensor->read();
    }
	void setMode(control_mode_t newMode, bool store);
	control_mode_t getMode(void) {
		return cs.mode;
	}
    temp_t getBeerSetting(void) {
        return control.beer1Set->read();
    }
    states getState(void) {
        // For cooling, show actual compressor pin ON state
        // For heating, show heating when PWM is active
        if (control.coolerMutex->isActive()) {
            lastCoolTime = ticks.seconds();
            return COOLING;
        } else if (control.heater1Mutex->isActive()) {
            lastHeatTime = ticks.seconds();
            return HEATING;
        } else if (timeSinceHeating() <= 2*control.heater1->getPeriod()){
            return HEATING; // in low period of PWM
        }
        else{
            lastIdleTime = ticks.seconds();
            return IDLE;
        }
    }
    bool modeIsBeer(void) {
        return (cs.mode == MODE_BEER_CONSTANT || cs.mode == MODE_BEER_PROFILE);
    }
    void setBeerTemp(temp_t newTemp, bool store);

    temp_t getFridgeTemp(void) {
        return control.fridgeSensor->read();
    }
    temp_t getFridgeSetting(void) {
        return control.fridgeSet->read();
    }
    void setFridgeTemp(temp_t newTemp, bool store);

    temp_t getRoomTemp(void) {
        return control.beer2Sensor->read();
    }


public:
    // Control parameters
    ControlConstants cc;
    ControlSettings cs;

private:
    // keep track of beer setting stored in EEPROM
    // Timers
    tcduration_t lastIdleTime;
    tcduration_t lastHeatTime;
    tcduration_t lastCoolTime;
    temp_t storedBeerSetting;
};

extern TempControl tempControl;
