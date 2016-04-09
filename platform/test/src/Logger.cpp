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

#include "temperatureFormats.h"
//#include "Platform.h"
//#include "BrewpiStrings.h"
#include "Logger.h"
#include "runner.h"

void Logger::logMessageVaArg(char type, LOG_ID_TYPE errorID, const char * varTypes, ...){
	va_list args;
	*output << "LOG MESSAGE: {";
	*output << type << ": ";
	*output << int(errorID) << ", V: [";
	va_start (args, varTypes);
	uint8_t index = 0;
	char buf[12];
	while(varTypes[index]){
		switch(varTypes[index]){	
			case 'd': // integer, signed or unsigned
				*output << va_arg(args, int);
				break;
			case 's': // string
				*output << va_arg(args, char*);
				break;
			case 't': // temperature in fixed point format
				*output << ((temp_t *) va_arg(args,void*))->toString(buf, 3, 12);
			break;			
		}
		if(varTypes[++index]){
			*output << ',';
		}
	}
	va_end (args);
	*output << (']');
	*output << "}\n";
}

Logger logger;
