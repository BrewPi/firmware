/*
 * Debug.cpp
 *
 * Created: 22-5-2013 17:53:42
 *  Author: Elco
 */ 

#include "Brewpi.h"
#include "Debug.h"
#include "PiLink.h"
#include "TemperatureFormats.h"
#include "JsonKeys.h"

static const char PROGMEM LOG_STRING_FORMAT[] = "\"%s\"";

void Logger::logMessageVaArg(char type, DEBUG_ID_TYPE errorID, const char * varTypes, ...){
	va_list args;
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logType, type);
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.print_P(PSTR(",\"V\":["));
	va_start (args, varTypes);
	uint8_t index = 0;
	while(varTypes[index]){
		switch(varTypes[index]){	
			case 'd': // integer, signed or unsigned
				piLink.print_P(PSTR("%d"), va_arg(args, int));
				break;
			case 's': // string
				piLink.print_P(LOG_STRING_FORMAT, va_arg(args, char*));
				break;
			case 't': // temperature in fixed_7_9 format
				char buf[12];
				piLink.print_P(LOG_STRING_FORMAT, tempToString(buf, va_arg(args,int), 1, 12));
			break;			
		}
		if(varTypes[++index]){
			piLink.print(',');
		}
	}
	va_end (args);
	piLink.print(']');
	piLink.sendJsonClose();
}

Logger logger;