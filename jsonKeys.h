/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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

#include <avr/pgmspace.h>

#ifndef JSON_H_
#define JSON_H_

static const char JSONKEY_mode[] PROGMEM = "mode";
static const char JSONKEY_beerSetting[] PROGMEM = "beerSetting";
static const char JSONKEY_fridgeSetting[] PROGMEM = "fridgeSetting";
static const char JSONKEY_heatEstimator[] PROGMEM = "heatEstimator";
static const char JSONKEY_coolEstimator[] PROGMEM = "coolEstimator";

// constant;
static const char JSONKEY_tempFormat[] PROGMEM = "tempFormat";
static const char JSONKEY_tempSettingMin[] PROGMEM = "tempSettingMin";
static const char JSONKEY_tempSettingMax[] PROGMEM = "tempSettingMax";
static const char JSONKEY_pidMax[] PROGMEM = "pidMax";
static const char JSONKEY_KpHeat[] PROGMEM = "KpHeat";
static const char JSONKEY_KpCool[] PROGMEM = "KpCool";
static const char JSONKEY_Ki[] PROGMEM = "Ki";
static const char JSONKEY_KdCool[] PROGMEM = "KdCool";
static const char JSONKEY_KdHeat[] PROGMEM = "KdHeat";
static const char JSONKEY_iMaxSlope[] PROGMEM = "iMaxSlope";
static const char JSONKEY_iMinSlope[] PROGMEM = "iMinSlope";
static const char JSONKEY_iMaxError[] PROGMEM = "iMaxError";
static const char JSONKEY_idleRangeHigh[] PROGMEM = "idleRangeHigh";
static const char JSONKEY_idleRangeLow[] PROGMEM = "idleRangeLow";
static const char JSONKEY_heatingTargetUpper[] PROGMEM = "heatingTargetUpper";
static const char JSONKEY_heatingTargetLower[] PROGMEM = "heatingTargetLower";
static const char JSONKEY_coolingTargetUpper[] PROGMEM = "coolingTargetUpper";
static const char JSONKEY_coolingTargetLower[] PROGMEM = "coolingTargetLower";
static const char JSONKEY_maxHeatTimeForEstimate[] PROGMEM = "maxHeatTimeForEstimate";
static const char JSONKEY_maxCoolTimeForEstimate[] PROGMEM = "maxCoolTimeForEstimate";
static const char JSONKEY_pidFilter[] PROGMEM = "pidFilter";
static const char JSONKEY_fridgeFastFilter[] PROGMEM = "fridgeFastFilter";
static const char JSONKEY_fridgeSlowFilter[] PROGMEM = "fridgeSlowFilter";
static const char JSONKEY_fridgeSlopeFilter[] PROGMEM = "fridgeSlopeFilter";
static const char JSONKEY_beerFastFilter[] PROGMEM = "beerFastFilter";
static const char JSONKEY_beerSlowFilter[] PROGMEM = "beerSlowFilter";
static const char JSONKEY_beerSlopeFilter[] PROGMEM = "beerSlopeFilter";

// variable;
static const char JSONKEY_beerDiff[] PROGMEM = "beerDiff";
static const char JSONKEY_diffIntegral[] PROGMEM = "diffIntegral";
static const char JSONKEY_beerSlope[] PROGMEM = "beerSlope";
static const char JSONKEY_p[] PROGMEM = "p";
static const char JSONKEY_i[] PROGMEM = "i";
static const char JSONKEY_d[] PROGMEM = "d";
static const char JSONKEY_Kp[] PROGMEM = "Kp";
static const char JSONKEY_Kd[] PROGMEM = "Kd";
static const char JSONKEY_estimatedPeak[] PROGMEM = "estimatedPeak";
static const char JSONKEY_negPeakSetting[] PROGMEM = "negPeakSetting";
static const char JSONKEY_posPeakSetting[] PROGMEM = "posPeakSetting";
static const char JSONKEY_negPeak[] PROGMEM = "negPeak";
static const char JSONKEY_posPeak[] PROGMEM = "posPeak";

#endif /* JSON_H_ */