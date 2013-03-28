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

#include "temperatureFormats.h"
#include <string.h>
#include <Arduino.h> // uses constrain function from Arduino.h
#include <limits.h>
#include "TempControl.h"

/**
 * Before (no-optimize)
				Program Memory Usage 	:	26230 bytes   80.0 % Full
				Data Memory Usage 		:	1081 bytes   42.2 % Full
 * After
				Program Memory Usage 	:	26058 bytes   79.5 % Full
				Data Memory Usage 		:	1087 bytes   42.5 % Full
 */
#define OPTIMIZE_TEMPERATURE_FORMATS_fixedPointToString 1 && OPTIMIZE_TEMPERATURE_FORMATS
#define OPTIMIZE_TEMPERATURE_FORMATS_convertAntConstrain 1 && OPTIMIZE_TEMPERATURE_FORMATS

// result can have maximum length of : sign + 3 digits integer part + point + 3 digits fraction part + '\0' = 9 bytes;
// only 1, 2 or 3 decimals allowed.
// returns pointer to the string
// fixed7_23 is used to prevent overflow
char * tempToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	if(rawValue == INT_MIN){
		strcpy_P(s, PSTR("null")); 
		return s;
	}
	if(tempControl.cc.tempFormat == 'F'){
		rawValue = (rawValue * 9) / 5 + (32 << 9); // convert to Fahrenheit first
	}
	return fixedPointToString(s, rawValue, numDecimals, maxLength);
}

char * fixedPointToString(char s[9], fixed7_9 rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	fixedPointToString(s, (fixed23_9)rawValue, numDecimals, maxLength);
}	

char * fixedPointToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	s[0] = ' ';
	if(rawValue < 0l){
		s[0] = '-';
		rawValue = -rawValue;
	}
	
	int intPart = rawValue >> 9;
	uint16_t fracPart;
#if OPTIMIZE_TEMPERATURE_FORMATS_fixedPointToString	
	const char* fmt;
	uint16_t scale;
	switch (numDecimals)
	{
		case 1:
			fmt = PSTR("%d.%01d");
			scale = 10;
			break;
		case 2:
			fmt = PSTR("%d.%02d");
			scale = 100;
			break;
		default:
			fmt = PSTR("%d.%03d");
			scale = 1000;
	}
	fracPart = ((rawValue & 0x01FF) * scale + 256) >> 9; // add 256 for rounding
	if(fracPart >= scale){
		intPart++;
		fracPart = 0;
	}
	snprintf_P(&s[1], maxLength-1, fmt,  intPart, fracPart);
#else
	if(numDecimals == 1){
		fracPart = ((rawValue & 0x01FF) * 10 + 256) >> 9; // add 256 for rounding
		if(fracPart >= 10){
			intPart++;
			fracPart = 0; // has already overflowed into integer part.
		}
		snprintf_P(&s[1], maxLength-1, PSTR("%d.%01d"), intPart, fracPart);
	}
	else if(numDecimals == 2){
		fracPart = ((rawValue  & 0x01FF) * 100 + 256) >> 9;
		if(fracPart >= 100){
			intPart++;
			fracPart = 0; // has already overflowed into integer part.
		}
		snprintf_P(&s[1], maxLength-1, PSTR("%d.%02d"), intPart, fracPart);
	}
	else{
		fracPart = ((rawValue  & 0x01FF) * 1000 + 256) >> 9;
		if(fracPart>=1000){
			intPart++;
			fracPart = 0; // has already overflowed into integer part.
		}
		snprintf_P(&s[1], maxLength-1, PSTR("%d.%03d"),  intPart, fracPart);
	}
#endif
	return s;
}

long int convertAndConstrain(long int rawTemp, byte offset)
{
	if(tempControl.cc.tempFormat == 'F'){
		rawTemp = ((rawTemp - offset) * 5) / 9; // convert to store as Celsius
	}	
	return constrain(rawTemp, INT_MIN, INT_MAX);	
}

fixed7_9 stringToTemp(char * numberString){
	fixed23_9 rawTemp = stringToFixedPoint(numberString);
#ifdef OPTIMIZE_TEMPERATURE_FORMATS_convertAntConstrain	
	return convertAndConstrain(rawTemp, 32UL<<9);	
#else	
	if(tempControl.cc.tempFormat == 'F'){
		rawTemp = ((rawTemp - (32 << 9)) * 5) / 9; // convert to store as Celsius
	}	
	return constrain(rawTemp, INT_MIN, INT_MAX);
#endif	
}

fixed23_9 stringToFixedPoint(char * numberString){
	// receive new temperature as null terminated string: "19.20"
	fixed23_9 intPart = 0;
	fixed23_9 fracPart = 0;
	
	char * fractPtr = 0; //pointer to the point in the string
	bool negative = 0;
	if(numberString[0] == '-'){
		numberString++;
		negative = true; // by processing the sign here, we don't have to include strtol
	}
	
	// find the point in the string to split in the integer part and the fraction part
	fractPtr = strchrnul(numberString, '.'); // returns pointer to the point.
		
	intPart = strtoul(numberString, NULL, 10);
	if(fractPtr != 0){
		// decimal point was found
		char * fractEndPtr;
		fractPtr++; // add 1 to pointer to skip point
		fracPart = strtoul(fractPtr,&fractEndPtr, 10);
		uint8_t numDecimals = fractEndPtr - fractPtr;
		fracPart = fracPart << 9; // 9 bits for fraction part
		while(numDecimals > 0){
			fracPart = (fracPart + 5) / 10; // divide by 10 rounded
			numDecimals--;
		}
	}
	fixed23_9 absVal = ((intPart<<9) +fracPart);
	return negative ? -absVal:absVal;
}

char * tempDiffToString(char s[9], fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength){
	if(tempControl.cc.tempFormat == 'F'){
		rawValue = (rawValue * 9) / 5; // convert to Fahrenheit first
	}
	return fixedPointToString(s, rawValue, numDecimals, maxLength);	
}

fixed7_9 stringToTempDiff(char * numberString){
	fixed23_9 rawTempDiff = stringToFixedPoint(numberString);
#ifdef OPTIMIZE_TEMPERATURE_FORMATS_convertAntConstrain
	return convertAndConstrain(rawTempDiff, 0);	
#else	
	if(tempControl.cc.tempFormat == 'F'){
		rawTempDiff = (rawTempDiff * 5) / 9; // convert to store as Celsius
	}	
	return constrain(rawTempDiff, INT_MIN, INT_MAX);	
#endif	
}

int fixedToTenths(fixed23_9 temperature){
	if(tempControl.cc.tempFormat == 'F'){
		temperature = temperature*9/5 + 32*512; // Convert to Fahrenheit fixed point first
	}
	
	return (int) ((10 * temperature + 256) / 512); // return rounded result in tenth of degrees
}

fixed7_9 tenthsToFixed(int temperature){
	if(tempControl.cc.tempFormat == 'F'){
		return (( ( (fixed23_9) temperature - 320) * 512 * 5) / 9 + 5) / 10; // convert to Celsius and return rounded result in fixed point
	}
	else{
		return ((fixed23_9) temperature * 512 + 5) / 10; // return rounded result in fixed point	
	}
}