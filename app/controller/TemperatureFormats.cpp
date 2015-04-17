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
#include "Platform.h"
#include "TempControl.h"
#include <string.h>
#include <stdarg.h>

// See header file for details about the temp format used.

// result can have maximum length of : sign + 3 digits integer part + point + 3 digits fraction part + '\0' = 9 bytes;
// only 1, 2 or 3 decimals allowed.
// returns pointer to the string
// long_temperature is used to prevent overflow
char * tempToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	if(rawValue == INVALID_TEMP){
		strcpy_P(s, PSTR("null")); 
		return s;
	}
	rawValue = convertFromInternalTemp(rawValue);
	return fixedPointToString(s, rawValue, numDecimals, maxLength);
}

char * tempDiffToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength){
	rawValue = convertFromInternalTempDiff(rawValue);
	return fixedPointToString(s, rawValue, numDecimals, maxLength);
}
	
char * fixedPointToString(char * s, temperature rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	return fixedPointToString(s, long_temperature(rawValue), numDecimals, maxLength);
}

// this gets rid of snprintf_P
void mysnprintf_P(char* buf, int len, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt );
	vsnprintf_P(buf, len, fmt, args);
	va_end (args);
}

char * fixedPointToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength){ 
	s[0] = ' ';
	if(rawValue < 0l){
		s[0] = '-';
		rawValue = -rawValue;
	}
	
	int intPart = longTempDiffToInt(rawValue); // rawValue is supposed to be without internal offset
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
	fracPart = ((rawValue & TEMP_FIXED_POINT_MASK) * scale + TEMP_FIXED_POINT_SCALE/2) >> TEMP_FIXED_POINT_BITS; // add 256 for rounding
	if(fracPart >= scale){
		intPart++;
		fracPart = 0;
	}
	mysnprintf_P(&s[1], maxLength-1, fmt,  intPart, fracPart);
	return s;
}

temperature stringToTemp(const char * numberString){
	long_temperature rawTemp = stringToFixedPointLong(numberString);
        if(rawTemp == INVALID_TEMP_LONG){
            return INVALID_TEMP;
        }
	rawTemp = convertToInternalTemp(rawTemp);
	return constrainTemp16(rawTemp);
}

temperature stringToTempDiff(const char * numberString){
	long_temperature rawTempDiff = stringToFixedPointLong(numberString);
        if(rawTempDiff == INVALID_TEMP_LONG){
            return INVALID_TEMP;
        }
	rawTempDiff = convertToInternalTempDiff(rawTempDiff);
	return constrainTemp16(rawTempDiff);
}


long_temperature stringToFixedPointLong(const char * numberString){
	// receive new temperature as null terminated string: "19.20"
	long_temperature intPart = 0;
	long_temperature fractPart = 0;
	
	char * startFract;
        char* end;
	
	// find the point in the string to split in the integer part and the fraction part
	startFract = strchr(numberString, '.'); // returns pointer to the point.
        intPart = strtol(numberString, &end, 10);
        if(startFract != 0){
		// decimal point was found
		startFract++; // add 1 to pointer to skip point
		fractPart = strtol(startFract, &end, 10);
                int8_t numDecimals = end - startFract;
		fractPart = fractPart << TEMP_FIXED_POINT_BITS; // make room for  bits for fraction part
		while(numDecimals > 0){
			fractPart = (fractPart + 5) / 10; // divide by 10 rounded
			numDecimals--;
		}
	}
        if((*end != '\0' && *end != '.' && *end != ' ') // parsing did not end at end of string, space or decimal point
                || numberString == end){ // no number found in string
            return INVALID_TEMP_LONG;
        }        
        
	return (intPart << TEMP_FIXED_POINT_BITS) + fractPart;
}

temperature stringToFixedPoint(const char * numberString){
    long_temperature temp = stringToFixedPointLong(numberString);
    if(temp == INVALID_TEMP_LONG){
        return INVALID_TEMP;
    }
    return constrainTemp16(temp);
}

// convertToInternalTemp receives the external temp format in fixed point and converts it to the internal format
// It scales the value for Fahrenheit and adds the offset needed for absolute temperatures. For temperature differences, use no offset.
long_temperature convertToInternalTempImpl(long_temperature rawTemp, bool addOffset){
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
long_temperature convertFromInternalTempImpl(long_temperature rawTemp, bool addOffset){
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

int fixedToTenths(long_temperature temp){
	temp = convertFromInternalTemp(temp);
	return (int) ((10 * temp + intToTempDiff(5)/10) / intToTempDiff(1)); // return rounded result in tenth of degrees
}

temperature tenthsToFixed(int temp){
	long_temperature fixedPointTemp = convertToInternalTemp(((long_temperature) temp * intToTempDiff(1) + 5) / 10);	
	return constrainTemp16(fixedPointTemp);
}

temperature constrainTemp(long_temperature valLong, temperature lower, temperature upper){
	temperature val = constrainTemp16(valLong);
	
	if(val < lower){
		return lower;
	}
	
	if(val > upper){
		return upper;
	}
	return temperature(valLong);
}


temperature constrainTemp16(long_temperature val)
{
	if(val<MIN_TEMP){
		return MIN_TEMP;
	}
	if(val>MAX_TEMP){
		return MAX_TEMP;
	}
	return val;	
}

temperature multiplyFactorTemperatureLong(temperature factor, long_temperature b)
{
	return constrainTemp16(((long_temperature) factor * (b-C_OFFSET))>>TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperatureDiffLong(temperature factor, long_temperature b)
{
	return constrainTemp16(((long_temperature) factor * b)>>TEMP_FIXED_POINT_BITS);
}


temperature multiplyFactorTemperature(temperature factor, temperature b)
{
	return constrainTemp16(((long_temperature) factor * ((long_temperature) b - C_OFFSET))>>TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperatureDiff(temperature factor, temperature b)
{
	return constrainTemp16(((long_temperature) factor * (long_temperature) b )>>TEMP_FIXED_POINT_BITS);
}
