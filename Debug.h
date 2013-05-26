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

// select whether to inline or outline info log message functions. Difference in bytes in the comments
#define INFO_INLINE_EMPTY 1 // 2
#define INFO_INLINE_S 0 // 0
#define INFO_INLINE_D 0 // 0
#define INFO_INLINE_T 0 // 0
#define INFO_INLINE_DS 1 // 32
#define INFO_INLINE_DST 1 // 16
#define INFO_INLINE_SS 1 // 8

class Logger{
	public:
	Logger(){};
	~Logger(){};
	
	static void logMessageVaArg(char type, DEBUG_ID_TYPE errorID, const char * varTypes, ...);
};
extern Logger logger;


#if BREWPI_DEBUG_ERRORS
	#define logError(debugId)				logger.logMessageVaArg('E', debugId, "")
	#define logErrorInt(debugId, val)		logger.logMessageVaArg('E', debugId, "d", val)
	#define logErrorString(debugId, val)	logger.logMessageVaArg('E', debugId, "s", val)
	#define logErrorTemp(debugId, temp)		logger.logMessageVaArg('E', debugId, "t", val)
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

	#if INFO_INLINE_EMPTY
		inline void logInfo(uint8_t debugId){
			logger.logMessageVaArg('I', debugId, "");
		}
	#else
		void logInfo(uint8_t debugId);
	#endif
	
	#if INFO_INLINE_D
		inline void logInfoInt(uint8_t debugId, int val){
			logger.logMessageVaArg('I', debugId, "d", val);
		}
	#else
		void logInfoInt(uint8_t debugId, int val);
	#endif
	
	#if INFO_INLINE_S
		inline void logInfoString(uint8_t debugId, const char * val){
			logger.logMessageVaArg('I', debugId, "s", val);
		}
	#else
		void logInfoString(uint8_t debugId, const char * val);
	#endif
	
	#if INFO_INLINE_T
		inline void logInfoTemp(uint8_t debugId, fixed7_9 temp){
			logger.logMessageVaArg('I', debugId, "t", temp);
		}
	#else
		void logInfoTemp(uint8_t debugId, fixed7_9 temp);
	#endif
	
	#if INFO_INLINE_DS
		inline void logInfoIntString(uint8_t debugId, int val1, const char * val2){
			logger.logMessageVaArg('I', debugId, "ds", val1, val2);
		}
	#else
		void logInfoIntString(uint8_t debugId, int val1, const char * val2);
	#endif
	
	#if INFO_INLINE_SS
		inline void logInfoStringString(uint8_t debugId, const char * val1, const char * val2){
			logger.logMessageVaArg('I', debugId, "ss", val1, val2);
		}
	#else
		void logInfoStringString(uint8_t debugId, const char * val1, const char * val2);
	#endif

	#if INFO_INLINE_DST
		inline void logInfoIntStringTemp(uint8_t debugId, int val1, const char * val2, fixed7_9 val3){
			logger.logMessageVaArg('I', debugId, "dst", val1, val2, val3);
		}
	#else
		void logInfoIntStringTemp(uint8_t debugId, int val1, const char * val2, fixed7_9 val3);
	#endif

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

#endif /* DEBUG_H_ */