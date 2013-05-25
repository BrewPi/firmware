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
#define BREWPI_DEBUG_DEVELOPER 0

#if BREWPI_DEBUG_ERRORS
	#define logError(debugId)				logger.logMessageVaArg('E', debugId, "")
	#define logErrorInt(debugId, val)		logger.logMessageVaArg('E', debugId, "d", val)
	#define logErrorString(debugId, val)	logger.logMessageVaArg('E', debugId, "s", val)
	#define logErrorTemp(debugId, temp)		logger.logMessage_temp('E', debugId, "t", val)
	#define logErrorIntInt(debugId, val1, val2)				logger.logMessageVaArg('E', debugId, "dd", val1, val2)
	#define logErrorIntIntInt(debugId, val1, val2, val3)	logger.logMessageVaArg('E', debugId, "ddd", val1, val2, val3)
#else
	#define logError(debugId)
	#define logErrorInt(debugId, val)
	#define logErrorString(debugId, val)
	#define logErrorTemp(debugId, temp)
#endif

#if BREWPI_DEBUG_WARNINGS
	#define logWarning(debugId)					logger.logMessageVaArg('W', debugId, "")
	#define logWarningInt(debugId, val)			logger.logMessageVaArg('W', debugId, "d", val)
	#define logWarningString(debugId, val)		logger.logMessageVaArg('W', debugId, "s", val)
	#define logWarningTemp(debugId, temp)		logger.logMessageVaArg('W', debugId, "t", val)
	#define logWarningIntString(debugId, val1, val2)	logger.logMessageVaArg('W', debugId, "ds", val1, val2)
#else
	#define logWarning(debugId)
	#define logWarningInt(debugId, val)
	#define logWarningString(debugId, val)
	#define logWarningTemp(debugId, temp)
#endif

#if BREWPI_DEBUG_INFO
	#define logInfo(debugId)							logger.logMessageVaArg('I', debugId, "")
	#define logInfoInt(debugId, val)					logger.logMessageVaArg('I', debugId, "d", val)
	#define logInfoString(debugId, val)					logger.logMessageVaArg('I', debugId, "s", val)
	#define logInfoTemp(debugId, temp)					logger.logMessageVaArg('I', debugId, "t", val)
	#define logInfoIntString(debugId, val1, val2)		logger.logMessageVaArg('I', debugId, "ds", val1, val2)
	#define logInfoStringString(debugId, val1, val2)	logger.logMessageVaArg('I', debugId, "ss", val1, val2)
	#define logInfoIntStringTemp(debugId, val1, val2, val3) logger.logMessageVaArg('I', debugId, "dst", val1, val2, val3)
#else
	#define logInfo(debugId)
	#define logInfoInt(debugId, val)
	#define logInfoStringString(debugId, val)
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
		
	//void logMessage(char type, DEBUG_ID_TYPE errorID);
	//void logMessageUint8(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...);
	//void logMessageString(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...);
	void logMessageVaArg(char type, DEBUG_ID_TYPE errorID, const char * varTypes, ...);
		
	void logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 value);
};

static Logger logger;



#endif /* DEBUG_H_ */