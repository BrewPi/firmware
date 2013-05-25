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
	MSG(ERROR_INVALID_DEVICE_FUNCTION, "Invalid device function id %d", config.deviceFunction),
	MSG(ERROR_INVALID_DEVICE_CONFIG_OWNER, "Invalid config for device owner type %d beer=%d chamber=%d", owner, config.beer, config.chamber),	
	MSG(ERROR_CANNOT_ASSIGN_TO_HARDWARE, "Cannot assign device type %d to hardware %d", dt, config.deviceHardware),
	MSG(ERROR_NOT_ONEWIRE_BUS, "Device is onewire but pin %d is not configured as a onewire bus", pinNr),

// PiLink.cpp
	MSG(ERROR_EXPECTED_BRACKET, "Expected { got %c", character),
	
}; // END enum errorMessages

enum warningMessages{
// PiLink.cpp
	MSG(WARNING_COULD_NOT_PROCESS_SETTING, "Could not process setting"),
	MSG(WARNING_INVALID_COMMAND, "Invalid command received by Arduino: %c", character),

// OneWireTempSensor.cpp	
	MSG(WARNING_TEMP_SENSOR_DISCONNECTED, "Temperature sensor disconnected pin %d, address %s", pinNr, addressString),

// SettingsManager.cpp	
	MSG(WARNING_START_IN_SAFE_MODE, "EEPROM Settings not available. Starting in safe mode.")
};

// Info messages
enum infoMessages{
// OneWireTempSensor.cpp
	MSG(INFO_TEMP_SENSOR_CONNECTED, "Temp sensor connected on pin %d, address %s", pinNr, addressString),
	MSG(INFO_TEMP_SENSOR_DISCONNECTED, "Temp sensor disconnected on pin %d, address %s", pinNr, addressString),
	MSG(INFO_TEMP_SENSOR_INITIALIZED, "Sensor initialized: pin %d %s %s", pinNr, addressString, temperature),

// DeviceManager.cpp
	MSG(INFO_UNINSTALL_TEMP_SENSOR, "uninstalling temperature sensor with function %d", config.deviceFunction),
	MSG(INFO_UNINSTALL_ACTUATOR, "uninstalling actuator with function %d", config.deviceFunction),
	MSG(INFO_UNINSTALL_SWITCH_SENSOR, "uninstalling switch sensor with function %d", config.deviceFunction),
	MSG(INFO_INSTALL_TEMP_SENSOR, "installing temperature sensor with function %d", config.deviceFunction),
	MSG(INFO_INSTALL_ACTUATOR, "installing actuator with function %d", config.deviceFunction),
	MSG(INFO_INSTALL_SWITCH_SENSOR, "installing switch sensor with function %d", config.deviceFunction),
	MSG(INFO_INSTALL_DEVICE, "Installing device f=%d", config.deviceFunction),
	MSG(INFO_MATCHING_DEVICE, "Matching device at slot %d", out.slot),
	MSG(INFO_SETTING_ACTIVATOR_STATE, "Setting activator state %d", state),
			
// PiLink.cpp
	MSG(INFO_RECEIVED_SETTING, "Received new setting: %s = %s", key, val),
	MSG(INFO_DEFAULT_CONSTANTS_LOADED, "Default constants loaded."),
	MSG(INFO_DEFAULT_SETTINGS_LOADED, "Default settings loaded."),
	MSG(INFO_EEPROM_INITIALIZED, "EEPROM initialized"),
	MSG(INFO_EEPROM_ZAPPED, "EEPROM zapped!"),
	
}; // END enum infoMessages



#endif /* DEBUGMESSAGES_H_ */