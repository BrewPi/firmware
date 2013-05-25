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

void Logger::logMessage(char type, DEBUG_ID_TYPE errorID){
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logType, type);
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.sendJsonClose();
}

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
			case 'd':
				piLink.print_P(PSTR("%d"), va_arg(args, int));
				break;
			case 's':
				piLink.print_P(PSTR("\"%s\""), va_arg(args, char*));
				break;
			case 'S':
				piLink.print_P(PSTR("\"%s\""), va_arg(args, const char *));
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

void Logger::logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 val){
	
}

