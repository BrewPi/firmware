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

// Use a struct to only store the JSON keys in program memory once
struct jsonKeysContainer{
	// settings
	const char * mode;
	const char * beerSetting;
	const char * fridgeSetting;
	const char * heatEstimator;
	const char * coolEstimator;
	// constants
	const char * tempFormat;
	const char * tempSettingMin;
	const char * tempSettingMax;
	const char * pidMax;
	const char * KpHeat;
	const char * KpCool;
	const char * Ki;
	const char * KdCool;
	const char * KdHeat;
	const char * iMaxSlope;
	const char * iMinSlope;
	const char * iMaxError;
	const char * idleRangeHigh;
	const char * idleRangeLow;
	const char * heatingTargetUpper;
	const char * heatingTargetLower;
	const char * coolingTargetUpper;
	const char * coolingTargetLower;
	const char * maxHeatTimeForEstimate;
	const char * maxCoolTimeForEstimate;
	const char * pidFilter;
	const char * fridgeFastFilter;
	const char * fridgeSlowFilter;
	const char * fridgeSlopeFilter;
	const char * beerFastFilter;
	const char * beerSlowFilter;
	const char * beerSlopeFilter;
	// variables
	const char * beerDiff;
	const char * diffIntegral;
	const char * beerSlope;
	const char * p;
	const char * i;
	const char * d;
	const char * Kp;
	const char * Kd;
	const char * estimatedPeak;
	const char * negPeakSetting;
	const char * posPeakSetting;
	const char * negPeak;
	const char * posPeak;
};

// These will be placed in data memory, but there's plenty left.
// By defining them in this struct, the only have to be stored once.
const jsonKeysContainer jsonKeys = {
	// settings
	"mode",
	"beerSetting",
	"fridgeSetting",
	"heatEstimator",
	"coolEstimator",
	// constants
	"tempFormat",
	"tempSettingMin",
	"tempSettingMax",
	"pidMax",
	"KpHeat",
	"KpCool",
	"Ki",
	"KdCool",
	"KdHeat",
	"iMaxSlope",
	"iMinSlope",
	"iMaxError",
	"idleRangeHigh",
	"idleRangeLow",
	"heatingTargetUpper",
	"heatingTargetLower",
	"coolingTargetUpper",
	"coolingTargetLower",
	"maxHeatTimeForEstimate",
	"maxCoolTimeForEstimate",
	"pidFilter",
	"fridgeFastFilter",
	"fridgeSlowFilter",
	"fridgeSlopeFilter",
	"beerFastFilter",
	"beerSlowFilter",
	"beerSlopeFilter",
	// variables
	"beerDiff",
	"diffIntegral",
	"beerSlope",
	"p",
	"i",
	"d",
	"Kp",
	"Kd",
	"estimatedPeak",
	"negPeakSetting",
	"posPeakSetting",
	"negPeak",
	"posPeak"
};

#endif /* JSON_H_ */