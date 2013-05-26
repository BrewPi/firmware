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
				piLink.print_P(PSTR("\"%s\""), va_arg(args, char*));
				break;
			case 't': // temperature in fixed_7_9 format
				char buf[12];
				piLink.print_P(PSTR("\"%s\""), tempToString(buf, va_arg(args,int), 1, 12));
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

#if BREWPI_DEBUG_INFO

#if !INFO_INLINE_EMPTY
void logInfo(uint8_t debugId){
	logger.logMessageVaArg('I', debugId, "");
}
#endif

#if !INFO_INLINE_D
void logInfoInt(uint8_t debugId, int val){
	logger.logMessageVaArg('I', debugId, "d", val);
}
#endif

#if !INFO_INLINE_S
void logInfoString(uint8_t debugId, const char * val){
	logger.logMessageVaArg('I', debugId, "s", val);	
}
#endif

#if !INFO_INLINE_SS
void logInfoStringString(uint8_t debugId, const char * val1, const char * val2){
	logger.logMessageVaArg('I', debugId, "ss", val1, val2);
}
#endif

#if !INFO_INLINE_DS
void logInfoIntString(uint8_t debugId, int val1, const char * val2){
	logger.logMessageVaArg('I', debugId, "ds", val1, val2);
}
#endif

#if !INFO_INLINE_DST
void logInfoIntStringTemp(uint8_t debugId, int val1, const char * val2, fixed7_9 val3){
	logger.logMessageVaArg('I', debugId, "dst", val1, val2, val3);
}
#endif

#if !INFO_INLINE_T
void logInfoTemp(uint8_t debugId, fixed7_9 temp){
	logger.logMessageVaArg('I', debugId, "t", temp);
}
#endif

#endif //BREWPI_DEBUG_INFO

Logger logger;