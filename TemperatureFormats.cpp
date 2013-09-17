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
#include "TemperatureFormats.h"
#include <string.h>
#include <limits.h>
#include "TempControl.h"

// See header file for details about the temp format used.

// result can have maximum length of : sign + 3 digits integer part + point + 3 digits fraction part + '\0' = 9 bytes;
// only 1, 2 or 3 decimals allowed.
// returns pointer to the string
// fixed7_23 is used to prevent overflow

// this gets rid of snprintf_P
void mysnprintf_P(char* buf, int len, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt );
	vsnprintf_P(buf, len, fmt, args);
	va_end (args);
}

char * fixedPointToString(char * s, fixed7_9 rawValue, uint8_t numDecimals, uint8_t maxLength){
	return fixedPointToString(s, (fixed23_9)rawValue, numDecimals, maxLength);
}

char * fixedPointToString(char * s, fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	s[0] = ' ';
	if(rawValue < 0l){
		s[0] = '-';
		rawValue = -rawValue;
	}
	
	int intPart = rawValue >> 9;
	uint16_t fracPart;
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
	mysnprintf_P(&s[1], maxLength-1, fmt,  intPart, fracPart);
	return s;
}

fixed23_9 stringToFixedPoint(const char * numberString){
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
	
	intPart = atol(numberString);
	if(fractPtr != 0){
		// decimal point was found
		fractPtr++; // add 1 to pointer to skip point
		int8_t numDecimals = (int8_t) strlen(fractPtr);
		fracPart = atol(fractPtr);
		fracPart = fracPart << 9; // 9 bits for fraction part
		while(numDecimals > 0){
			fracPart = (fracPart + 5) / 10; // divide by 10 rounded
			numDecimals--;
		}
	}
	fixed23_9 absVal = ((intPart<<9) +fracPart);
	return negative ? -absVal:absVal;
}

fixed7_9 constrainTemp16(fixed23_9 val)
{
	/* saves just 6 bytes - a bit cryptic for general use!
	int16_t upper = val>>16;
	if (((upper+1) & ~1)==0)		// 0 or 1, so upper word was -1 or 0
		return fixed7_9(val);
	return (upper<0) ? INT_MIN : INT_MAX; // upper > 0 || upper < -1
	*/
	if(val<INT_MIN){
		return INT_MIN;
	}
	if(val>INT_MAX){
		return INT_MAX;
	}
	return val;	
}

// convertToInternalTemp receives the external temp format in fixed point and converts it to the internal format
// It scales the value for Fahrenheit and adds the offset needed for absolute temperatures. For temperature differences, use no offset.
fixed23_9 convertToInternalTemp(fixed23_9 rawTemp, bool addOffset){
	if(tempControl.cc.tempFormat == 'F'){ // value received is in F, convert to C
		rawTemp = (rawTemp) * 5 / 9;
		if(addOffset){
			rawTemp += F_OFFSET;
		}
	}
	else{
		if(addOffset){
			rawTemp += C_OFFSET;
		}		
	}
	return rawTemp;
}

// convertAndConstrain adds an offset, then scales with *9/5 for Fahrenheit. Use it without the offset argument for temperature differences
fixed23_9 convertFromInternalTemp(fixed23_9 rawTemp, bool addOffset){
	if(tempControl.cc.tempFormat == 'F'){ // value received is in F, convert to C
		if(addOffset){
			rawTemp -= F_OFFSET;
		}
		rawTemp = rawTemp * 9 / 5;
	}
	else{
		if(addOffset){
			rawTemp -= C_OFFSET;
		}
	}
	return rawTemp;
}

char * tempToString(char * s, fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength){
	if(rawValue == INT_MIN){
		strcpy_P(s, PSTR("null"));
		return s;
	}
	rawValue = convertFromInternalTemp(rawValue, true);
	return fixedPointToString(s, rawValue, numDecimals, maxLength);
}

char * tempDiffToString(char * s, fixed23_9 rawValue, uint8_t numDecimals, uint8_t maxLength){
	convertFromInternalTemp(rawValue, false);
	return fixedPointToString(s, rawValue, numDecimals, maxLength);
}

fixed7_9 stringToTemp(const char * numberString){
	fixed23_9 rawTemp = stringToFixedPoint(numberString);
	rawTemp = convertToInternalTemp(rawTemp, true);
	return constrainTemp16(rawTemp);
}

fixed7_9 stringToTempDiff(const char * numberString){
	fixed23_9 rawTempDiff = stringToFixedPoint(numberString);
	rawTempDiff = convertToInternalTemp(rawTempDiff, false);	
	return constrainTemp16(rawTempDiff);
}

int fixedToTenths(fixed23_9 temperature){
	temperature = convertFromInternalTemp(temperature, true);
	return (int) ((10 * temperature + 256) / 512); // return rounded result in tenth of degrees
}

fixed7_9 tenthsToFixed(int temperature){
	fixed23_9 fixedPointTemp = convertToInternalTemp(((fixed23_9) temperature * 512 + 5)/10, true);
	return constrainTemp16(fixedPointTemp);
}

fixed7_9 constrainTemp(fixed23_9 valLong, fixed7_9 lower, fixed7_9 upper){
	fixed7_9 val = constrainTemp16(valLong);
	
	if(val < lower){
		return lower;
	}
	
	if(val > upper){
		return upper;
	}
	return (fixed7_9)valLong;
}