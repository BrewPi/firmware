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

#if BREWPI_DEBUG_ERRORS
	#define logError(debugId, ...) logger.logMessage(debugId, __VA_ARGS__)
	#define logError_temp(debugId, temp) logger.logMessage_temp(debugId, temp)
#else
	#define logError(debugId, ...)
	#define logError_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_WARNINGS
	#define logWarning(debugId, ...) logger.logMessage(debugId, __VA_ARGS__)
	#define logWarning_temp(debugId, temp) logger.logMessage_temp(debugId, temp)
#else
	#define logWarning(debugId, ...)
	#define logWarning_temp(debugId, temp)
#endif

#if BREWPI_DEBUG_INFO
	#define logInfo(debugId, ...) logger.logMessage(debugId, __VA_ARGS__)
	#define logInfo_temp(debugId, temp) logger.logMessage_temp(debugId, temp)
#else
	#define logInfo(...)
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
		
	void logMessage(DEBUG_ID_TYPE errorID);
	void logMessage(DEBUG_ID_TYPE errorID, uint8_t value);
	void logMessage(DEBUG_ID_TYPE errorID, char * infoString);
	void logMessage_temp(DEBUG_ID_TYPE errorID, fixed7_9 value);
};

static Logger logger;

/* Overview of error messages and ID's 
	ERRORS are ID < 80
	WARNINGS are 81 < ID <160
	INFO MESSAGES are 161 < ID < 255
*/

// OneWireTempSensor.cpp
#define ERROR_SRAM_SENSOR 0
// ("Not enough SRAM for temp sensor %s"), addressString
#define ERROR_SENSOR_NO_ADDRESS_ON_PIN 1
// ("Cannot find address for sensor on pin %d"), pinNr

#define INFO_SENSOR_CONNECTED 161
// ("Temp sensor connected on pin %d, pinNr
#define INFO_SENSOR_FETCHING_INITIAL_TEMP 162
// ("Fetching initial temperature of sensor %s"), addressString


// DeviceManager.cpp
#define INFO_UNINSTALL_TEMP_SENSOR 163
// ("uninstalling temperature sensor  with function %d"), config.deviceFunction
#define INFO_UNINSTALL_ACTUATOR 164
// ("uninstalling actuator with function %d"), config.deviceFunction
#define INFO_UNINSTALL_SWITCH_SENSOR 165
// ("uninstalling switch sensor  with function %d"), config.deviceFunction

#define INFO_INSTALL_TEMP_SENSOR 166
// ("installing temperature sensor  with function %d"), config.deviceFunction
#define INFO_INSTALL_ACTUATOR 167
// ("installing actuator with function %d"), config.deviceFunction
#define INFO_INSTALL_SWITCH_SENSOR 168
// ("installing switch sensor  with function %d"), config.deviceFunction

#define ERROR_OUT_OF_MEMORY_FOR_DEVICE 2
// ("*** OUT OF MEMORY for device f=%d"), config.deviceFunction



#endif /* DEBUG_H_ */