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

#ifdef ARDUINO
#include <avr/pgmspace.h>
#endif

// settings
static const char JSONKEY_mode[] PROGMEM = "mode";
static const char JSONKEY_beerSetting[] PROGMEM = "beerSet";
static const char JSONKEY_fridgeSetting[] PROGMEM = "fridgeSet";

// constant;
static const char JSONKEY_tempFormat[] PROGMEM = "tempFormat";

static const char JSONKEY_heater1_kp[] PROGMEM = "heater1_kp";
static const char JSONKEY_heater1_ti[] PROGMEM = "heater1_ti";
static const char JSONKEY_heater1_td[] PROGMEM = "heater1_td";
static const char JSONKEY_heater1_infilt[] PROGMEM = "heater1_infilt";
static const char JSONKEY_heater1_dfilt[] PROGMEM = "heater1_dfilt";

static const char JSONKEY_heater2_kp[] PROGMEM = "heater2_kp";
static const char JSONKEY_heater2_ti[] PROGMEM = "heater2_ti";
static const char JSONKEY_heater2_td[] PROGMEM = "heater2_td";
static const char JSONKEY_heater2_infilt[] PROGMEM = "heater2_infilt";
static const char JSONKEY_heater2_dfilt[] PROGMEM = "heater2_dfilt";

static const char JSONKEY_cooler_kp[] PROGMEM = "cooler_kp";
static const char JSONKEY_cooler_ti[] PROGMEM = "cooler_ti";
static const char JSONKEY_cooler_td[] PROGMEM = "cooler_td";
static const char JSONKEY_cooler_infilt[] PROGMEM = "cooler_infilt";
static const char JSONKEY_cooler_dfilt[] PROGMEM = "cooler_dfilt";

static const char JSONKEY_beer2fridge_kp[] PROGMEM = "beer2fridge_kp";
static const char JSONKEY_beer2fridge_ti[] PROGMEM = "beer2fridge_ti";
static const char JSONKEY_beer2fridge_td[] PROGMEM = "beer2fridge_td";
static const char JSONKEY_beer2fridge_infilt[] PROGMEM = "beer2fridge_infilt";
static const char JSONKEY_beer2fridge_dfilt[] PROGMEM = "beer2fridge_dfilt";
static const char JSONKEY_beer2fridge_pidMax[] PROGMEM = "beer2fridge_pidMax";

static const char JSONKEY_minCoolTime[] PROGMEM = "minCoolTime";
static const char JSONKEY_minCoolIdleTime[] PROGMEM = "minCoolIdleTime";
static const char JSONKEY_heater1PwmPeriod[] PROGMEM = "heater1PwmPeriod";
static const char JSONKEY_heater2PwmPeriod[] PROGMEM = "heater2PwmPeriod";
static const char JSONKEY_coolerPwmPeriod[] PROGMEM = "coolerPwmPeriod";

static const char JSONKEY_mutexDeadTime[] PROGMEM = "deadTime";

static const char JSONKEY_logType[] PROGMEM = "logType";
static const char JSONKEY_logID[] PROGMEM = "logID";
