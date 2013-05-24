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

#define NUMARGS_UINT8(...)  (sizeof((uint8_t[]){__VA_ARGS__})/sizeof(uint8_t))
#define NUMARGS_STRING(...)  (sizeof((char*[]){__VA_ARGS__})/sizeof(char*))
#define NUMARGS_CONST_STRING(...)  (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*))

#if BREWPI_DEBUG_ERRORS
	#define logError(debugId)					logger.logMessage('E', debugId)
	#define logErrorUint8(debugId, ...)			logger.logMessageUint8('E', debugId, NUMARGS_UINT8(__VA_ARGS__), __VA_ARGS__)
	#define logErrorString(debugId, ...)		logger.logMessageString('E', debugId, NUMARGS_STRING(__VA_ARGS__), __VA_ARGS__)
	#define logErrorConstString(debugId, ...)	logger.logMessageConstString('E', debugId, NUMARGS_CONST_STRING(__VA_ARGS__), __VA_ARGS__)
	#define logError_temp(debugId, temp)		logger.logMessage_temp(debugId, temp)
#else
	#define logError(debugId)
	#define logErrorUint8(debugId, ...)
	#define logErrorString(debugId, ...)
	#define logErrorConstString(debugId, ...)
	#define logError_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_WARNINGS
	#define logWarning(debugId)					logger.logMessage('W', debugId)
	#define logWarningUint8(debugId, ...)		logger.logMessageUint8('W', debugId, NUMARGS_UINT8(__VA_ARGS__), __VA_ARGS__)
	#define logWarningString(debugId, ...)		logger.logMessageString('W', debugId, NUMARGS_STRING(__VA_ARGS__), __VA_ARGS__)
	#define logWarningConstString(debugId, ...) logger.logMessageConstString('W', debugId, NUMARGS_CONST_STRING(__VA_ARGS__), __VA_ARGS__)
	#define logWarning_temp(debugId, temp)		logger.logMessage_temp(debugId, temp)
#else
	#define logWarning(debugId)
	#define logWarningUint8(debugId, ...)
	#define logWarningString(debugId, ...)
	#define logWarningConstString(debugId, ...)
	#define logWarning_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_INFO
	#define logInfo(debugId)					logger.logMessage('I', debugId)
	#define logInfoUint8(debugId, ...)			logger.logMessageUint8('I', debugId, NUMARGS_UINT8(__VA_ARGS__), __VA_ARGS__)
	#define logInfoString(debugId, ...)			logger.logMessageString('I', debugId, NUMARGS_STRING(__VA_ARGS__), __VA_ARGS__)
	#define logInfoConstString(debugId, ...)	logger.logMessageConstString('I', debugId, NUMARGS_CONST_STRING(__VA_ARGS__), __VA_ARGS__)
	#define logInfo_temp(debugId, temp)			logger.logMessage_temp(debugId, temp)
#else
	#define logInfo(debugId)
	#define logInfoUint8(debugId, ...)
	#define logInfoString(debugId, ...)
	#define logInfoConstString(debugId, ...)
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
	
	template <class T>
	void logMessageVaArg(char type, DEBUG_ID_TYPE errorID, uint8_t n, ...);
	
	void logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 value);
};

static Logger logger;



#endif /* DEBUG_H_ */