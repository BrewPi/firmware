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

void Logger::logMessageUint8(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...){
	va_list args;
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logType, type);
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.print_P(PSTR(",\"V\":["));
	va_start(args, n);
	for (uint8_t i=0;i<n;i++)
	{
		piLink.firstPair = true;
		piLink.sendJsonPair(JSONKEY_logString, (uint16_t) va_arg(args, unsigned int));
		piLink.print('}');
		if(i+1<n){
			piLink.print(',');
		}
	}
	va_end(args);
	piLink.print(']');
	piLink.sendJsonClose();
}

void Logger::logMessageString(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...){
	va_list args;
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logType, type);
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.print_P(PSTR(",\"V\":["));
	va_start(args, n);
	for (uint8_t i=0;i<n;i++)
	{
		piLink.firstPair = true;
		piLink.sendJsonPair(JSONKEY_logString, va_arg(args, char*));
		piLink.print('}');
		if(i+1<n){
			piLink.print(',');
		}
	}
	va_end(args);
	piLink.print(']');
	piLink.sendJsonClose();
}

void Logger::logMessageConstString(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...){
	va_list args;
	piLink.printResponse('D');
	piLink.sendJsonPair(JSONKEY_logType, type);
	piLink.sendJsonPair(JSONKEY_logID, errorID);
	piLink.print_P(PSTR(",\"V\":["));
	va_start(args, n);
	for (uint8_t i=0;i<n;i++)
	{
		piLink.firstPair = true;
		piLink.sendJsonPair(JSONKEY_logString, va_arg(args, const char *));
		piLink.print('}');
		if(i+1<n){
			piLink.print(',');
		}
	}
	va_end(args);
	piLink.print(']');
	piLink.sendJsonClose();
}

void Logger::logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 val){
	
}

