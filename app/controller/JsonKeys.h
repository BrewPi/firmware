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

// settings
static const char JSONKEY_mode[] = "mode";
static const char JSONKEY_beerSetting[] = "beerSet";
static const char JSONKEY_fridgeSetting[] = "fridgeSet";

// constant;
static const char JSONKEY_tempFormat[] = "tempFormat";

static const char JSONKEY_heater1_kp[] = "heater1_kp";
static const char JSONKEY_heater1_ti[] = "heater1_ti";
static const char JSONKEY_heater1_td[] = "heater1_td";
static const char JSONKEY_heater1_infilt[] = "heater1_infilt";
static const char JSONKEY_heater1_dfilt[] = "heater1_dfilt";

static const char JSONKEY_heater2_kp[] = "heater2_kp";
static const char JSONKEY_heater2_ti[] = "heater2_ti";
static const char JSONKEY_heater2_td[] = "heater2_td";
static const char JSONKEY_heater2_infilt[] = "heater2_infilt";
static const char JSONKEY_heater2_dfilt[] = "heater2_dfilt";

static const char JSONKEY_cooler_kp[] = "cooler_kp";
static const char JSONKEY_cooler_ti[] = "cooler_ti";
static const char JSONKEY_cooler_td[] = "cooler_td";
static const char JSONKEY_cooler_infilt[] = "cooler_infilt";
static const char JSONKEY_cooler_dfilt[] = "cooler_dfilt";

static const char JSONKEY_beer2fridge_kp[] = "beer2fridge_kp";
static const char JSONKEY_beer2fridge_ti[] = "beer2fridge_ti";
static const char JSONKEY_beer2fridge_td[] = "beer2fridge_td";
static const char JSONKEY_beer2fridge_infilt[] = "beer2fridge_infilt";
static const char JSONKEY_beer2fridge_dfilt[] = "beer2fridge_dfilt";
static const char JSONKEY_beer2fridge_pidMax[] = "beer2fridge_pidMax";

static const char JSONKEY_minCoolTime[] = "minCoolTime";
static const char JSONKEY_minCoolIdleTime[] = "minCoolIdleTime";
static const char JSONKEY_heater1PwmPeriod[] = "heater1PwmPeriod";
static const char JSONKEY_heater2PwmPeriod[] = "heater2PwmPeriod";
static const char JSONKEY_coolerPwmPeriod[] = "coolerPwmPeriod";

static const char JSONKEY_mutexDeadTime[] = "deadTime";

static const char JSONKEY_logType[] = "logType";
static const char JSONKEY_logID[] = "logID";
