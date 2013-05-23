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
	#define logErrorConstString(debugId, ...)	logger.logMessageConstString('E', debugId, NUMARGS_CONSTSTRING(__VA_ARGS__), __VA_ARGS__)
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
	#define logWarningConstString(debugId, ...) logger.logMessageConstString('W', debugId, NUMARGS_CONSTSTRING(__VA_ARGS__), __VA_ARGS__)
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
	#define logInfoConstString(debugId, ...)	logger.logMessageConstString('I', debugId, NUMARGS_CONSTSTRING(__VA_ARGS__), __VA_ARGS__)
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
	
	void logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 value);
};

static Logger logger;

/* Overview of error messages and ID's 
	ERRORS are ID < 99
	WARNINGS are 200 < ID < 255
	INFO MESSAGES are 100 < ID < 200
*/

// Errors

// OneWireTempSensor.cpp
#define ERROR_SRAM_SENSOR 0
// ("Not enough SRAM for temp sensor %s"), addressString
#define ERROR_SENSOR_NO_ADDRESS_ON_PIN 1
// ("Cannot find address for sensor on pin %d"), pinNr
#define ERROR_OUT_OF_MEMORY_FOR_DEVICE 2
// ("*** OUT OF MEMORY for device f=%d"), config.deviceFunction

// DeviceManager.cpp
#define ERROR_DEVICE_DEFINITION_UPDATE_SPEC_INVALID 3
// ("Device defifination update specification is invalid"
#define ERROR_INVALID_CHAMBER 4
// ("Invalid chamber id %d"), config.chamber
#define ERROR_INVALID_BEER 5
// ("Invalid beer id %d"), config.beer
#define ERROR_INVALID_DEVICE_FUNCTION 6
// ("Invalid device function id %d"), config.deviceFunction

// Info messages

// OneWireTempSensor.cpp
#define INFO_SENSOR_CONNECTED 100
// ("Temp sensor connected on pin %d, pinNr
#define INFO_SENSOR_FETCHING_INITIAL_TEMP 101
// ("Fetching initial temperature of sensor %s"), addressString


// DeviceManager.cpp
#define INFO_UNINSTALL_TEMP_SENSOR 102
// ("uninstalling temperature sensor  with function %d"), config.deviceFunction
#define INFO_UNINSTALL_ACTUATOR 103
// ("uninstalling actuator with function %d"), config.deviceFunction
#define INFO_UNINSTALL_SWITCH_SENSOR 104
// ("uninstalling switch sensor  with function %d"), config.deviceFunction

#define INFO_INSTALL_TEMP_SENSOR 105
// ("installing temperature sensor  with function %d"), config.deviceFunction
#define INFO_INSTALL_ACTUATOR 106
// ("installing actuator with function %d"), config.deviceFunction
#define INFO_INSTALL_SWITCH_SENSOR 107
// ("installing switch sensor  with function %d"), config.deviceFunction

#define INFO_INSTALL_DEVICE 108
// ("Installing device f=%d"), config.deviceFunction
#define INFO_DEVICE_DEFINITION 109
// ("deviceDef %s:%s"), key, val


#endif /* DEBUG_H_ */