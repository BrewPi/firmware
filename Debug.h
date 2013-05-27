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

class Logger{
	public:
	Logger(){};
	~Logger(){};
	
	static void logMessageVaArg(const char type, DEBUG_ID_TYPE errorID, const char * varTypes, ...);
};
extern Logger logger;


#if BREWPI_DEBUG_ERRORS
	inline void logError(uint8_t debugId){
		logger.logMessageVaArg('E', debugId, "");
	}
	inline void logErrorInt(uint8_t debugId, int val){
		logger.logMessageVaArg('E', debugId, "d", val);
	}
	inline void logErrorString(uint8_t debugId, const char * val){
		logger.logMessageVaArg('E', debugId, "s", val);
	}
	inline void logErrorTemp(uint8_t debugId, fixed7_9 temp){
		logger.logMessageVaArg('E', debugId, "t", temp);
	}
	inline void logErrorIntInt(uint8_t debugId, int val1, int val2){
		logger.logMessageVaArg('E', debugId, "dd", val1, val2);
	}
	inline void logErrorIntIntInt(uint8_t debugId, int val1, int val2, int val3){
		logger.logMessageVaArg('E', debugId, "ddd", val1, val2, val3);
	}
#else
	#define logError(debugId)
	#define logErrorInt(debugId, val)
	#define logErrorString(debugId, val)
	#define logErrorTemp(debugId, temp)
	#define logErrorIntInt(debugId, val1, val2)
	#define logErrorIntIntInt(debugId, val1, val2, val3)
#endif

#if BREWPI_DEBUG_WARNINGS
	inline void logWarning(uint8_t debugId){
		logger.logMessageVaArg('W', debugId, "");
	}
	inline void logWarningInt(uint8_t debugId, int val){
		logger.logMessageVaArg('W', debugId, "d", val);
	}
	inline void logWarningString(uint8_t debugId, const char * val){
		logger.logMessageVaArg('W', debugId, "s", val);
	}
	inline void logWarningTemp(uint8_t debugId, fixed7_9 temp){
		logger.logMessageVaArg('W', debugId, "t", temp);
	}
	inline void logWarningIntString(uint8_t debugId, int val1, const char *val2){
		logger.logMessageVaArg('W', debugId, "ds", val1, val2);
	}
#else
	#define logWarning(debugId)
	#define logWarningInt(debugId, val)
	#define logWarningString(debugId, val)
	#define logWarningTemp(debugId, temp)
	#define logWarningIntString(debugId, val1, val2)
#endif

#if BREWPI_DEBUG_INFO
		inline void logInfo(uint8_t debugId){
			logger.logMessageVaArg('I', debugId, "");
		}
		inline void logInfoInt(uint8_t debugId, int val){
			logger.logMessageVaArg('I', debugId, "d", val);
		}
		inline void logInfoString(uint8_t debugId, const char * val){
			logger.logMessageVaArg('I', debugId, "s", val);
		}
		inline void logInfoTemp(uint8_t debugId, fixed7_9 temp){
			logger.logMessageVaArg('I', debugId, "t", temp);
		}
		inline void logInfoIntString(uint8_t debugId, int val1, const char * val2){
			logger.logMessageVaArg('I', debugId, "ds", val1, val2);
		}
		inline void logInfoStringString(uint8_t debugId, const char * val1, const char * val2){
			logger.logMessageVaArg('I', debugId, "ss", val1, val2);
		}
		inline void logInfoIntStringTemp(uint8_t debugId, int val1, const char * val2, fixed7_9 val3){
			logger.logMessageVaArg('I', debugId, "dst", val1, val2, val3);
		}
#else
	#define logInfo(debugId)
	#define logInfoInt(debugId, val)
	#define logInfoString(debugId, val)
	#define logInfoTemp(debugId, temp)
	#define logInfoStringString(debugId, val1, val2)
	#define logInfoIntString(debugId, val1, val2)
	#define logInfoIntStringTemp(debugId, val1, val2, val3)
	
	
#endif

#if BREWPI_DEBUG_DEVELOPER
	#define logDeveloper(string, ...) piLink.debugMessage(PSTR(string), ##__VA_ARGS__)
#else
	#define logDeveloper(string, ...)
#endif

#if BREWPI_DEBUG>0
	#define DEBUG_ONLY(x) x
#else
	#define DEBUG_ONLY(x)
#endif


#endif /* DEBUG_H_ */