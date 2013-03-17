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
struct jsonKeys {
	// settings
	static const char* mode;
	static const char * beerSetting;
	static const char * fridgeSetting;
	static const char * heatEstimator;
	static const char * coolEstimator;
	// constants
	static const char * tempFormat;
	static const char * tempSettingMin;
	static const char * tempSettingMax;
	static const char * pidMax;
	static const char * KpHeat;
	static const char * KpCool;
	static const char * Ki;
	static const char * KdCool;
	static const char * KdHeat;
	static const char * iMaxSlope;
	static const char * iMinSlope;
	static const char * iMaxError;
	static const char * idleRangeHigh;
	static const char * idleRangeLow;
	static const char * heatingTargetUpper;
	static const char * heatingTargetLower;
	static const char * coolingTargetUpper;
	static const char * coolingTargetLower;
	static const char * maxHeatTimeForEstimate;
	static const char * maxCoolTimeForEstimate;
	static const char * pidFilter;
	static const char * fridgeFastFilter;
	static const char * fridgeSlowFilter;
	static const char * fridgeSlopeFilter;
	static const char * beerFastFilter;
	static const char * beerSlowFilter;
	static const char * beerSlopeFilter;
	// variables
	static const char * beerDiff;
	static const char * diffIntegral;
	static const char * beerSlope;
	static const char * p;
	static const char * i;
	static const char * d;
	static const char * Kp;
	static const char * Kd;
	static const char * estimatedPeak;
	static const char * negPeakSetting;
	static const char * posPeakSetting;
	static const char * negPeak;
	static const char * posPeak;
	
};

#if 0
// These will be placed in data memory, but there's plenty left.
// By defining them in this struct, the only have to be stored once.
const jsonKeysContainer jsonKeys = {
};
#endif
#endif /* JSON_H_ */