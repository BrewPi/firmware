/*
 * Debug.h
 *
 * Created: 22-5-2013 17:52:26
 *  Author: Elco
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdarg.h>
#include "TemperatureFormats.h"
#include "DebugMessages.h"

// BREWPI_DEBUG is set in ConfigDefault.h

#if (BREWPI_DEBUG > 0)
#define DEBUG_MSG_1(...) piLink.debugMessage(__VA_ARGS__);
#else
#define DEBUG_MSG_1(...)
#endif

#if (BREWPI_DEBUG > 1)
#define DEBUG_MSG_2(...) piLink.debugMessage(__VA_ARGS__);
#else
#define DEBUG_MSG_2(...)
#endif
#if (BREWPI_DEBUG > 2)
#define DEBUG_MSG_3(...) piLink.debugMessage(__VA_ARGS__);
#else
#define DEBUG_MSG_3(...)
#endif

// define error id variable type to make it easy to bump to uint16 when needed
#define DEBUG_ID_TYPE uint8_t

// move these to config file later
#define BREWPI_DEBUG_ERRORS 1
#define BREWPI_DEBUG_WARNINGS 1
#define BREWPI_DEBUG_INFO 1
#define BREWPI_DEBUG_DEVELOPER 1

#if BREWPI_DEBUG_ERRORS
	#define logError(debugId)					logger.logMessageVaArg('E', debugId, "")
	#define logErrorInt(debugId, val)			logger.logMessageVaArg('E', debugId, "d", val)
	#define logErrorString(debugId, val)		logger.logMessageVaArg('E', debugId, "s", val)
	#define logErrorConstString(debugId, val)	logger.logMessageVaArg('E', debugId, "S", val)
	#define logError_temp(debugId, temp)		logger.logMessage_temp(debugId, temp)
#else
	#define logError(debugId)
	#define logErrorInt(debugId, ...)
	#define logErrorString(debugId, ...)
	#define logErrorConstString(debugId, ...)
	#define logError_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_WARNINGS
	#define logWarning(debugId)					logger.logMessageVaArg('I', debugId, "")
	#define logWarningInt(debugId, ...)			logger.logMessageVaArg('I', debugId, "d", val)
	#define logWarningString(debugId, ...)		logger.logMessageVaArg('I', debugId, "s", val)
	#define logWarningConstString(debugId, ...) logger.logMessageVaArg('I', debugId, "S", val)
	#define logWarning_temp(debugId, temp)		logger.logMessage_temp(debugId, temp)
#else
	#define logWarning(debugId)
	#define logWarningInt(debugId, ...)
	#define logWarningString(debugId, ...)
	#define logWarningConstString(debugId, ...)
	#define logWarning_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_INFO
	#define logInfo(debugId)							logger.logMessageVaArg('I', debugId, "")
	#define logInfoInt(debugId, val)					logger.logMessageVaArg('I', debugId, "d", val)
	#define logInfoString(debugId, val)					logger.logMessageVaArg('I', debugId, "s", val)
	#define logInfoConstString(debugId, val)			logger.logMessageVaArg('I', debugId, "S", val)
	#define logInfoConstString2(debugId, val1, val2)	logger.logMessageVaArg('I', debugId, "SS", val1, val2)
	#define logInfo_temp(debugId, temp)					logger.logMessage_temp(debugId, temp)
#else
	#define logInfo(debugId)
	#define logInfoInt(debugId, ...)
	#define logInfoString(debugId, ...)
	#define logInfoConstString(debugId, ...)
	#define logInfoConstString2(debugId, val1, val2)
	#define logInfo_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_DEVELOPER
	#define logDeveloper(...) piLink.debugMessage(__VA_ARGS__)
#else
	#define logDeveloper(debugId, ...)
#endif

class Logger{
	public:
	
	Logger(){};
	~Logger(){};
		
	void logMessage(char type, DEBUG_ID_TYPE errorID);
	void logMessageUint8(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...);
	void logMessageString(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...);
	void logMessageConstString(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...);
	void logMessageVaArg(char type, DEBUG_ID_TYPE errorID, const char * varTypes, ...);
		
	void logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 value);
};

static Logger logger;



#endif /* DEBUG_H_ */