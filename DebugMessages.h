/*
 * DebugMessages.h
 *
 * Created: 24-5-2013 17:50:48
 *  Author: Elco
 */ 


#ifndef DEBUGMESSAGES_H_
#define DEBUGMESSAGES_H_

/*	Overview of debug messages and ID's 
	This file is parsed by 

*/

#define MSG(errorID, errorString, ...) errorID

// Errors
enum errorMessages{
// OneWireTempSensor.cpp
	MSG(ERROR_SRAM_SENSOR, "Not enough SRAM for temp sensor %s", addressString),
	MSG(ERROR_SENSOR_NO_ADDRESS_ON_PIN, "Cannot find address for sensor on pin %d", pinNr),
	MSG(ERROR_OUT_OF_MEMORY_FOR_DEVICE, "*** OUT OF MEMORY for device f=%d", config.deviceFunction),

// DeviceManager.cpp
	MSG(ERROR_DEVICE_DEFINITION_UPDATE_SPEC_INVALID, "Device defifination update specification is invalid"),
	MSG(ERROR_INVALID_CHAMBER, "Invalid chamber id %d", config.chamber),
	MSG(ERROR_INVALID_BEER, "Invalid beer id %d", config.beer),
	MSG(ERROR_INVALID_DEVICE_FUNCTION, "Invalid device function id %d", config.deviceFunction)
}; // END enum errorMessages

// Info messages
enum infoMessages{
// OneWireTempSensor.cpp
	MSG(INFO_SENSOR_CONNECTED, "Temp sensor connected on pin %d", pinNr),
	MSG(INFO_SENSOR_FETCHING_INITIAL_TEMP, "Fetching initial temperature of sensor %s", addressString),

// DeviceManager.cpp
	MSG(INFO_UNINSTALL_TEMP_SENSOR, "uninstalling temperature sensor with function %d", config.deviceFunction),
	MSG(INFO_UNINSTALL_ACTUATOR, "uninstalling actuator with function %d", config.deviceFunction),
	MSG(INFO_UNINSTALL_SWITCH_SENSOR, "uninstalling switch sensor  with function %d", config.deviceFunction),

	MSG(INFO_INSTALL_TEMP_SENSOR, "installing temperature sensor  with function %d", config.deviceFunction),
	MSG(INFO_INSTALL_ACTUATOR, "installing actuator with function %d", config.deviceFunction),
	MSG(INFO_INSTALL_SWITCH_SENSOR, "installing switch sensor  with function %d", config.deviceFunction),

	MSG(INFO_INSTALL_DEVICE, "Installing device f=%d", config.deviceFunction),
	MSG(INFO_DEVICE_DEFINITION, "deviceDef %s:%s", key, val),
}; // END enum infoMessages



#endif /* DEBUGMESSAGES_H_ */