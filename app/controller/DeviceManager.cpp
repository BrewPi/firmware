/*
 * Copyright 2013 Matthew McGowan
 * Copyright 2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Brewpi.h"
#include "Board.h"
#include "BrewpiStrings.h"
#include "DeviceManager.h"
#include "TempControl.h"
#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "TempSensorDisconnected.h"
#include "TempSensorExternal.h"
#include "PiLink.h"
#include "EepromFormat.h"
#include "EepromManager.h"
#include "defaultDevices.h"
#include "OneWireAddress.h"

#ifdef WIRING

#include "OneWireTempSensor.h"
#if BREWPI_DS2408
#include "DS2408.h"
#include "ValveController.h"
#endif
#if BREWPI_DS2413
#include "DS2413.h"
#include "ActuatorOneWire.h"
#endif
#include "OneWire.h"
#include "DallasTemperature.h"
#include "ActuatorPin.h"

#endif

class OneWire;

bool DeviceManager::firstDeviceOutput;
device_slot_t findHardwareDevice(DeviceConfig & find);
device_slot_t findDeviceFunction(DeviceConfig & find);

Interface * DeviceManager::devices[NUM_DEVICE_SLOTS];

bool isAssignable(DeviceFunction function,
                  DeviceHardware hardware)
{
    auto type = deviceType(function, hardware);
    return ((hardware == DEVICE_HARDWARE_PIN)
            && (type == DEVICETYPE_SWITCH_ACTUATOR || type == DEVICETYPE_SWITCH_SENSOR))

#if BREWPI_DS2413
            || ((hardware == DEVICE_HARDWARE_ONEWIRE_2413)
                && ((type == DEVICETYPE_SWITCH_ACTUATOR )
                    || (DS2413_SUPPORT_SENSE && (type == DEVICETYPE_SWITCH_SENSOR))))
#endif

#if BREWPI_DS2408
            || ((hardware == DEVICE_HARDWARE_ONEWIRE_2408)
                && (type == DEVICETYPE_SWITCH_ACTUATOR ))
#endif

            || ((hardware == DEVICE_HARDWARE_ONEWIRE_TEMP)
                && (type == DEVICETYPE_TEMP_SENSOR))
            || ((hardware == DEVICE_HARDWARE_NONE) && (type == DEVICETYPE_NONE));
}

bool isOneWire(DeviceHardware hardware)
{
    return
#if BREWPI_DS2413
    (hardware == DEVICE_HARDWARE_ONEWIRE_2413) ||
#endif
#if BREWPI_DS2408
    (hardware == DEVICE_HARDWARE_ONEWIRE_2408) ||
#endif

    (hardware == DEVICE_HARDWARE_ONEWIRE_TEMP);
}

bool isDigitalPin(DeviceHardware hardware)
{
    return hardware == DEVICE_HARDWARE_PIN;
}

DeviceConnection deviceConnection(DeviceHardware hardware)
{
    return isOneWire(hardware) ? DEVICE_CONNECTION_ONEWIRE : DEVICE_CONNECTION_PIN;
}

/*
 * Sets devices to their unconfigured states. Each device is initialized to a static no-op instance.
 * This method is idempotent, and is called each time the eeprom is reset.
 */
void DeviceManager::setupUnconfiguredDevices(bool eraseEeprom)
{
    // right now, uninstall doesn't care about chamber/beer distinction.
    // but this will need to match beer/function when multiferment is available

    for (device_slot_t i = 0; i < NUM_DEVICE_SLOTS; i++){
        uninstallDevice(i, eraseEeprom);
    }
}

/*
 * Creates a new device for the given config.
 */
Interface * DeviceManager::createDevice(DeviceConfig & config,
                                   DeviceType     dt)
{
    switch (config.deviceHardware){
        case DEVICE_HARDWARE_NONE :
            break;

        case DEVICE_HARDWARE_PIN :
            // No sense support for now
            return new ActuatorPin(config.hw.pinNr, config.hw.invert);
        case DEVICE_HARDWARE_ONEWIRE_TEMP :
        {
            auto sensor = new OneWireTempSensor(oneWireBus(config.hw.pinNr), config.hw.address, config.hw.settings.sensor.calibration);
            return sensor;
        }
        break;
#if BREWPI_DS2413
        case DEVICE_HARDWARE_ONEWIRE_2413 :
        {
            std::shared_ptr<DS2413> hwDevice(nullptr);
            Interface * sharedHardware = findSharedHardware(config);

            if(sharedHardware){
                auto otherActuator = asInterface<ActuatorOneWire>(sharedHardware);
                if(otherActuator != nullptr){
                    hwDevice = otherActuator->getHardwareDevice();
                }
            }
            if(hwDevice == nullptr){
                hwDevice = std::make_shared<DS2413>(oneWireBus(config.hw.pinNr), config.hw.address);
            }

            return new ActuatorOneWire(hwDevice, config.hw.settings.actuator.pio, config.hw.invert);
        }
#endif

#if BREWPI_DS2408
        case DEVICE_HARDWARE_ONEWIRE_2408 :
        {
            std::shared_ptr<DS2408> hwDevice(nullptr);
            Interface * sharedHardware = findSharedHardware(config);

            if(sharedHardware){
                auto otherValve = asInterface<ValveController>(sharedHardware);
                if(otherValve != nullptr){
                    hwDevice = otherValve->getHardwareDevice();
                }
            }
            if(hwDevice == nullptr){
                hwDevice = std::make_shared<DS2408>(oneWireBus(config.hw.pinNr), config.hw.address);
            }

            return new ValveController(hwDevice, config.hw.settings.actuator.pio);
        }
#endif
    }
    return nullptr;
}

class DeviceListLookup {
public:
    DeviceListLookup(uint8_t _slot) : slot(_slot){}
    ~DeviceListLookup() = default;

    Interface * operator()(){
        return DeviceManager::fetch(slot);
    }
private:
    uint8_t slot;
};

/*
 * Creates and installs a device
 */
bool DeviceManager::createAndInstallDevice(DeviceConfig config, device_slot_t slot){

    // deduct device type from function and hardware for now
    DeviceType dt  = deviceType(config.deviceFunction, config.deviceHardware);
    Interface * device = nullptr;
    device = DeviceManager::createDevice(config, dt);
    if (device == nullptr && config.deviceFunction != DEVICE_NONE){
        logErrorInt(ERROR_OUT_OF_MEMORY_FOR_DEVICE, config.deviceFunction);
        return false;
    }
    return installDevice(device, config, slot, true);
}

bool DeviceManager::installDevice(Interface * device, DeviceConfig config, device_slot_t slot, bool storeEeprom)
{
    if(!isDefinedSlot(slot)){
        return false;
    }
    devices[slot] = device;
    // create lookup object for passing to delegates
    // if we are uninstalling a device (device = nullptr), set the lookup of the old function to nullptr
    std::function<Interface* ()> lookup = nullptr;
    if(device != nullptr){
        lookup = DeviceListLookup(slot);
    }

    switch (config.deviceFunction){
        case DEVICE_NONE :
            break;
        case DEVICE_CHAMBER_HEAT:
            control.heater1Toggle.setLookup(lookup);
            break;
        case DEVICE_CHAMBER_COOL :
            control.coolerToggle.setLookup(lookup);
            break;
        case DEVICE_CHAMBER_TEMP :
            control.fridgeSensor.setLookup(lookup);
            break;
        case DEVICE_LOG1_TEMP :
            control.log1Sensor.setLookup(lookup);
            break;
        case DEVICE_LOG2_TEMP :
            control.log2Sensor.setLookup(lookup);
            break;
        case DEVICE_LOG3_TEMP :
            control.log3Sensor.setLookup(lookup);
            break;

        case DEVICE_BEER_FIRST:
            control.beer1Sensor.setLookup(lookup);
            break;

        case DEVICE_BEER_TEMP2:
            control.beer2Sensor.setLookup(lookup);
            break;
        case DEVICE_CHAMBER_MANUAL_ACTUATOR :
        	break; // not installed for now, only exists in device list
    }
    if(device == nullptr){
        config.deviceFunction = DEVICE_NONE; // if the device ptr is cleared, also clear EEPROM slot
    }
    if(storeEeprom){
        eepromManager.storeDevice(config, slot);
    }
    return true;
}

/*
 * Removes an installed device.
 * /param config The device to remove. The fields that are used are
 *              chamber, beer, hardware and function.
 */
void DeviceManager::uninstallDevice(device_slot_t slot, bool eraseEeprom)
{
    if(isDefinedSlot(slot)){
        Interface * device = devices[slot];
        DeviceConfig config;
        config.deviceFunction = DeviceFunction::DEVICE_NONE;
        config.deviceHardware = DeviceHardware::DEVICE_HARDWARE_NONE;
        installDevice(nullptr, config, slot, eraseEeprom);
        delete device;
        devices[slot] = nullptr;
    }
}

struct DeviceDefinition
{
    int8_t        id;
    int8_t        chamber;
    int8_t        beer;
    int8_t        deviceFunction;
    int8_t        deviceHardware;
    int8_t        pinNr;
    int8_t        invert;
    int8_t        pio;
    int8_t        deactivate;
    temp_t        calibrationAdjust;
    DeviceAddress address;

    /*
     * Lists the first letter of the key name for each attribute.
     */
    static const char ORDER[12];
};

// the special cases are placed at the end. All others should map directly to an int8_t via atoi().
const char DeviceDefinition::ORDER[12] = "icbfhpxndja";
const char DEVICE_ATTRIB_INDEX = 'i';
const char DEVICE_ATTRIB_CHAMBER = 'c';
const char DEVICE_ATTRIB_BEER = 'b';
const char DEVICE_ATTRIB_FUNCTION = 'f';
const char DEVICE_ATTRIB_HARDWARE = 'h';
const char DEVICE_ATTRIB_PIN = 'p';
const char DEVICE_ATTRIB_INVERT = 'x';
const char DEVICE_ATTRIB_DEACTIVATED = 'd';
const char DEVICE_ATTRIB_ADDRESS = 'a';

#if BREWPI_DS2413 || BREWPI_DS2408
const char DEVICE_ATTRIB_PIO = 'n';
#endif

const char DEVICE_ATTRIB_CALIBRATEADJUST = 'j'; // value to add to temp sensors to bring to correct temperature
//const char DEVICE_ATTRIB_VALUE           = 'v';    // print current values
//const char DEVICE_ATTRIB_WRITE           = 'w';    // write value to device
const char DEVICE_ATTRIB_TYPE = 't';

void handleDeviceDefinition(const char * key, const char * val, void * pv) {
    DeviceDefinition * def = (DeviceDefinition *) pv;

    // logDebug("deviceDef %s:%s", key, val);
    // the characters are listed in the same order as the DeviceDefinition struct.
    int8_t idx = indexOf(DeviceDefinition::ORDER, key[0]);

    if (key[0] == DEVICE_ATTRIB_ADDRESS) {
        parseBytes(def->address, val, 8);
    } else if (key[0] == DEVICE_ATTRIB_CALIBRATEADJUST) {
        temp_t parsedVal = temp_t::raw(0);
        const int32_t minval = temp_t(-5.0).getRaw();
        const int32_t maxval = temp_t(5.0).getRaw();
        if (parsedVal.fromTempString(val, tempControl.cc.tempFormat, false, minval, maxval)) {
            def->calibrationAdjust = parsedVal;
        }
    } else if (idx >= 0) {
        ((uint8_t *) def)[idx] = (uint8_t) atol(val);
    }
}

bool inRangeUInt8(uint8_t val, uint8_t min, uint8_t max) {
    return (min <= val) && (val <= max);
}

bool inRangeInt8(int8_t val, int8_t min, int8_t max) {
    return (min <= val) && (val <= max);
}

void assignIfSet(int8_t value, uint8_t * target) {
    if (value >= 0) {
        *target = (uint8_t) value;
    }
}

bool isUniqueFunction(int8_t f) {
    return (f != DEVICE_CHAMBER_MANUAL_ACTUATOR && f != DEVICE_NONE);
}

/*
 * Updates the device definition. Only changes that result in a valid device, with no conflicts with other devices
 * are allowed.
 */
void DeviceManager::parseDeviceDefinition(Stream & p) {
    static DeviceDefinition dev;

    fill((int8_t *) &dev, sizeof(dev)); // fills with all -1
    dev.calibrationAdjust == temp_t::invalid();
    piLink.parseJson(&handleDeviceDefinition, &dev);

    if (!inRangeInt8(dev.id, 0, NUM_DEVICE_SLOTS))    // no device id given, or it's out of range, can't do anything else.
    {
        return;
    }

    // save the original device so we can revert
    DeviceConfig target;
    DeviceConfig original;

    // todo - should ideally check if the eeprom is correctly initialized.
    eepromManager.fetchDevice(original, dev.id);
    memcpy(&target, &original, sizeof(target));
    assignIfSet(dev.chamber, &target.chamber);
    assignIfSet(dev.beer, &target.beer);
    assignIfSet(dev.deviceFunction, (uint8_t *) &target.deviceFunction);
    assignIfSet(dev.deviceHardware, (uint8_t *) &target.deviceHardware);

    assignIfSet(dev.pinNr, &target.hw.pinNr);
    assignIfSet(dev.invert, (uint8_t *) &target.hw.invert);

    assignIfSet(dev.pio, &target.hw.settings.actuator.pio);

    if(dev.deviceHardware == DEVICE_HARDWARE_ONEWIRE_TEMP){
		if(temp_t::invalid() != dev.calibrationAdjust){
			target.hw.settings.sensor.calibration = dev.calibrationAdjust;
		}
		else{
			target.hw.settings.sensor.calibration = temp_t::raw(0);
		}
    }
    if (dev.address[0] != 0xFF) // first byte is family identifier. I don't have a complete list, but so far 0xFF is not used.
            {
        memcpy(target.hw.address, dev.address, 8);
    }

    assignIfSet(dev.deactivate, (uint8_t *) &target.hw.deactivate);

    // setting function to none clears all other fields.
    if (target.deviceFunction == DEVICE_NONE) {
        clear((uint8_t *) &target, sizeof(target));
    }

    bool valid = isDeviceValid(target, original, dev.id);
    DeviceConfig * print = &original;

    if (valid) {
        print = &target;

        // remove the device from another slot if that slot has the same hardware
        device_slot_t oldHardwareSlot = findHardwareDevice(target);
        uninstallDevice(oldHardwareSlot, true);


        if(isUniqueFunction(target.deviceFunction)){ // check if function can only be installed once
            // remove the device from another slot if that slot has the same function
            device_slot_t oldFunctionSlot = findDeviceFunction(target);
            uninstallDevice(oldFunctionSlot, true);
        }

        // remove the existing device from the target slot, no need to erase EEPROM because we'll overwrite it
        uninstallDevice(dev.id, false);

        createAndInstallDevice(target, dev.id);
    } else{
        logError(ERROR_DEVICE_DEFINITION_UPDATE_SPEC_INVALID);
    }

    piLink.printResponse('U');
    deviceManager.beginDeviceOutput();
    deviceManager.printDevice(dev.id, *print, NULL, p);
    piLink.printNewLine();
}

/*
 * Determines if a given device definition is valid.
 * chamber/beer must be within bounds
 * device function must match the chamber/beer spec, and must not already be defined for the same chamber/beer combination
 * device hardware type must be applicable with the device function
 * pinNr must be unique for digital pin devices?
 * pinNr must be a valid OneWire bus for one wire devices.
 * for onewire temp devices, address must be unique.
 * for onewire ds2413 devices, address+pio must be unique.
 */
bool DeviceManager::isDeviceValid(DeviceConfig & config,
        DeviceConfig & original, uint8_t deviceIndex) {
#if 1

    /*
     *  Implemented checks to ensure the system will not crash when supplied with invalid data.
     *  More refined checks that may cause confusing results are not yet implemented. See todo below.
     */

    if (!inRangeUInt8(config.deviceFunction, 0, DEVICE_FUNCTION_MAX - 1)){
        logErrorInt(ERROR_INVALID_DEVICE_FUNCTION, config.deviceFunction);

        return false;
    }

    if(!isDefinedSlot(deviceIndex)){
        return false;
    }

    // todo - find device at another index with the same chamber/beer/function spec.
    // with duplicate function defined for the same beer, that they will both try to create/delete the device in the target location.
    // The highest id will win.
    DeviceType dt = deviceType(config.deviceFunction, config.deviceHardware);

    if (!isAssignable(config.deviceFunction, config.deviceHardware)){
        logErrorIntInt(ERROR_CANNOT_ASSIGN_TO_HARDWARE, dt, config.deviceHardware);

        return false;
    }

    // todo - check pinNr uniqueness for direct digital I/O devices?

    /* pinNr for a onewire device must be a valid bus. While this won't cause a crash, it's a good idea to validate this. */
    if (isOneWire(config.deviceHardware)) {
        if (!oneWireBus(config.hw.pinNr)) {
            logErrorInt(ERROR_NOT_ONEWIRE_BUS, config.hw.pinNr);

            return false;
        }
    } else {    // regular pin device
        // todo - could verify that the pin nr corresponds to enumActuatorPins/enumSensorPins
    }
#endif

    // todo - for onewire temp, ensure address is unique
    // todo - for onewire 2413 check address+pio nr is unique
    return true;
}

void printAttrib(Print & p, char c, int8_t val,
bool first = false) {
    if (!first) {
        p.print(',');
    }

    char tempString[32];    // resulting string limited to 128 chars

    sprintf(tempString, "\"%c\":%d", c, val);
    p.print(tempString);
}

bool hasInvert(DeviceHardware hw)
{
#if BREWPI_DS2413
    return (hw == DEVICE_HARDWARE_PIN) || (hw == DEVICE_HARDWARE_ONEWIRE_2413);
#else
    return hw == DEVICE_HARDWARE_PIN;
#endif

}

bool hasOnewire(DeviceHardware hw)
{
    return hw == DEVICE_HARDWARE_ONEWIRE_TEMP
#if BREWPI_DS2413
            || (hw == DEVICE_HARDWARE_ONEWIRE_2413)
#endif
#if BREWPI_DS2408
    || (hw == DEVICE_HARDWARE_ONEWIRE_2408)
#endif
    ;
}

void DeviceManager::printDevice(device_slot_t  slot,
                                DeviceConfig & config,
                                const char *   value,
                                Print &        p)
{
    char       buf[17];
    DeviceType dt = deviceType(config.deviceFunction, config.deviceHardware);

    if (!firstDeviceOutput) {
        // p.print('\n');
        p.print(',');
    }

    firstDeviceOutput = false;

    p.print('{');
    printAttrib(p, DEVICE_ATTRIB_INDEX, slot, true);
    printAttrib(p, DEVICE_ATTRIB_TYPE, dt);
    printAttrib(p, DEVICE_ATTRIB_CHAMBER, config.chamber);
    printAttrib(p, DEVICE_ATTRIB_BEER, config.beer);
    printAttrib(p, DEVICE_ATTRIB_FUNCTION, config.deviceFunction);
    printAttrib(p, DEVICE_ATTRIB_HARDWARE, config.deviceHardware);
    printAttrib(p, DEVICE_ATTRIB_DEACTIVATED, config.hw.deactivate);
    printAttrib(p, DEVICE_ATTRIB_PIN, config.hw.pinNr);

    if (value && *value) {
        p.print(",\"v\":");
        p.print(value);
    }

    if (hasInvert(config.deviceHardware)) {
        printAttrib(p, DEVICE_ATTRIB_INVERT, config.hw.invert);
    }

    if (hasOnewire(config.deviceHardware)) {
        p.print(",\"a\":\"");
        printBytes(config.hw.address, 8, buf);
        p.print(buf);
        p.print('"');
    }

#if BREWPI_DS2413 || BREWPI_DS2408
    if (false // to be able to use || below
#if BREWPI_DS2413
            || config.deviceHardware == DEVICE_HARDWARE_ONEWIRE_2413
#endif
#if BREWPI_DS2408
                    || config.deviceHardware == DEVICE_HARDWARE_ONEWIRE_2408
#endif
                    ) {
        printAttrib(p, DEVICE_ATTRIB_PIO, config.hw.settings.actuator.pio);
    }
#endif

    if (config.deviceHardware == DEVICE_HARDWARE_ONEWIRE_TEMP) {
        config.hw.settings.sensor.calibration.toTempString(buf, 3, 8,
                tempControl.cc.tempFormat, false);
        p.print(",\"j\":");
        p.print(buf);
    }

    p.print('}');
}

bool DeviceManager::allDevices(DeviceConfig & config, uint8_t deviceIndex) {
    return eepromManager.fetchDevice(config, deviceIndex);
}

void DeviceManager::OutputEnumeratedDevices(DeviceConfig * config,
        DeviceCallbackInfo * info) {
    printDevice(info->slot, *config, info->value, *(Print *) info->data);
}

bool DeviceManager::enumDevice(DeviceDisplay & dd, DeviceConfig & dc,
        uint8_t idx) {
    if (dd.id == -1) {
        return (dd.empty || dc.deviceFunction); // if enumerating all devices, honor the unused request param
    } else {
        return (dd.id == idx);   // enumerate only the specific device requested
    }
}

void handleHardwareSpec(const char * key, const char * val, void * pv) {
    EnumerateHardware * h = (EnumerateHardware *) pv;

    // logDebug("hardwareSpec %s:%s", key, val);
    int8_t idx = indexOf("hpvuf", key[0]);

    if (idx >= 0) {
        *((int8_t *) h + idx) = atol(val);
    }
}

bool matchAddress(uint8_t * detected,
                  uint8_t * configured,
                  uint8_t   count)
{
    if (!configured[0]){
        return true;
    }

    while (count-- > 0) {
        if (detected[count] != configured[count]) {
            return false;
        }
    }

    return true;
}

/*
 * Find a device based on it's location.
 * A device's location is:
 *   pinNr  for simple digital pin devices
 *   pinNr+address for one-wire devices
 *   pinNr+address+pio for 2413 and 2408
 */
device_slot_t findHardwareDevice(DeviceConfig & find) {
    DeviceConfig config;

    for (device_slot_t slot = 0; deviceManager.allDevices(config, slot);
            slot++) {
        if (find.deviceHardware == config.deviceHardware) {
            bool match = true;

            switch (find.deviceHardware) {
#if BREWPI_DS2413
                case DEVICE_HARDWARE_ONEWIRE_2413:
#endif
#if BREWPI_DS2408
                case DEVICE_HARDWARE_ONEWIRE_2408:
#endif
#if BREWPI_DS2413 || BREWPI_DS2408
                    match &= find.hw.settings.actuator.pio
                            == config.hw.settings.actuator.pio;

                    // fall through
#endif

                case DEVICE_HARDWARE_ONEWIRE_TEMP:
                    match &= matchAddress(find.hw.address, config.hw.address,
                            8);

                    // fall through
                case DEVICE_HARDWARE_PIN:
                    match &= find.hw.pinNr == config.hw.pinNr;
                    break;
                case DEVICE_HARDWARE_NONE:
                    match = false; // don't return a match for no type
                    break;
                default: // this should not happen - if it does the device will be returned as matching.
                    break;
            }

            if (match) {
                return slot;
            }
        }
    }

    return INVALID_SLOT;
}

/*
 * Find a device based on the function.
 */
device_slot_t findDeviceFunction(DeviceConfig & find) {
    DeviceConfig config;

    for (device_slot_t slot = 0; deviceManager.allDevices(config, slot);
            slot++) {
        if (find.deviceFunction == config.deviceFunction) {
            bool match = true;
            if (match) {
                return slot;
            }
        }
    }
    return INVALID_SLOT;
}

void DeviceManager::readTempSensorValue(DeviceConfig::Hardware hw,
                                               char * out)
{
#if !BREWPI_SIMULATE
    OneWire * bus = oneWireBus(hw.pinNr);
    OneWireTempSensor sensor(bus, hw.address, short(0)); // NB: this value is uncalibrated, since we don't have the calibration offset until the device is configured
    temp_t temp = temp_t::invalid();

    if (sensor.init()) {
        sensor.update();
        temp = sensor.read();
    }
    temp.toTempString(out, 3, 9, tempControl.cc.tempFormat, true);
#else
    strcpy(out, "0.00");
#endif
}

#if BREWPI_DS2408
void DeviceManager::readNotInstalledValve(DeviceConfig::Hardware hw,
                                     char * out)
{
    auto hwDevice = std::make_shared<DS2408>(oneWireBus(hw.pinNr), hw.address);
    ValveController valve(hwDevice, hw.settings.actuator.pio);
    uint8_t valveState = valve.read(true);
    sprintf(out, STR_FMT_U, (unsigned int) valveState);
}

void DeviceManager::writeNotInstalledValve(DeviceConfig::Hardware hw, uint8_t value)
{
    auto hwDevice = std::make_shared<DS2408>(oneWireBus(hw.pinNr), hw.address);
    ValveController valve(hwDevice, hw.settings.actuator.pio);
    valve.write(value);
}
#endif

void DeviceManager::writeNotInstalledPin(DeviceConfig::Hardware hw, uint8_t value)
{
    bool active = value != 0;
    pinMode(hw.pinNr, OUTPUT);
    digitalWrite(hw.pinNr, (active ^ hw.invert) ? HIGH : LOW);
}

void DeviceManager::readNotInstalledPin(DeviceConfig::Hardware hw, char * out){
    ActuatorDigital::State state = digitalRead(hw.pinNr) ^ hw.invert ? ActuatorDigital::State::Active : ActuatorDigital::State::Inactive;
    sprintf(out, STR_FMT_U, state);
}

#if BREWPI_DS2413
void DeviceManager::writeNotInstalledOneWirePin(DeviceConfig::Hardware hw, uint8_t value)
{
    auto hwDevice = std::make_shared<DS2413>(oneWireBus(hw.pinNr), hw.address);
    ActuatorOneWire pin(hwDevice, hw.settings.actuator.pio, hw.invert);
    pin.setState(value != 0 ? ActuatorDigital::State::Active : ActuatorDigital::State::Inactive);
}

void DeviceManager::readNotInstalledOneWirePin(DeviceConfig::Hardware hw, char * out){
    auto hwDevice = std::make_shared<DS2413>(oneWireBus(hw.pinNr), hw.address);
    ActuatorOneWire pin(hwDevice, hw.settings.actuator.pio, hw.invert);
    ActuatorDigital::State state = pin.getState();
    sprintf(out, STR_FMT_U, state);
}
#endif

void DeviceManager::handleEnumeratedDevice(DeviceConfig & config,
        EnumerateHardware &                               h,
        EnumDevicesCallback                               callback,
        DeviceCallbackInfo *                              info)
{
    if (h.function &&!isAssignable(DeviceFunction(h.function), config.deviceHardware)){
        return;    // device not applicable for required function
    }

    // logDebug("Handling device");
    device_slot_t slot = findHardwareDevice(config);

    info->slot = slot;
    info->value[0] = 0;

    DEBUG_ONLY(logInfoInt(INFO_MATCHING_DEVICE, slot));

    if (isDefinedSlot(slot)) {
        if (h.unused)  // only list unused devices, and this one is already used
        {
            return;
        }

        // display the actual matched value
        deviceManager.allDevices(config, slot);
    }

    if (h.values) {
        // logDebug("Fetching device value");
        switch (config.deviceHardware) {
            case DEVICE_HARDWARE_ONEWIRE_TEMP:
                readTempSensorValue(config.hw, info->value);
                break;
#if BREWPI_DS2408
            case DEVICE_HARDWARE_ONEWIRE_2408:
                readNotInstalledValve(config.hw, info->value);
                break;
#endif
                // unassigned pins could be input or output so we can't determine any other details from here.
                // values can be read once the pin has been assigned a function
            default:
                break;
        }
    }

    // logDebug("Passing device to callback");
    callback(&config, info);
}

bool isDefinedSlot(device_slot_t s)
{
    return INVALID_SLOT < s && s < NUM_DEVICE_SLOTS;
}


/*
 * Enumerate the static devices that are permanently installed.
 * @param h
 * @param callback
 * @param output
 */
void DeviceManager::enumeratePinDevices(EnumerateHardware & h,
        EnumDevicesCallback callback, DeviceCallbackInfo * info) {
    DeviceConfig config;

    clear((uint8_t *) &config, sizeof(config));

    config.deviceHardware = DEVICE_HARDWARE_PIN;
    config.chamber = 1;    // chamber 1 is default

    int8_t pin;

#if BREWPI_ACTUATOR_PINS
    for (uint8_t count = 0;
            (pin = deviceManager.enumerateActuatorPins(count)) >= 0; count++) {
        if ((h.pin != -1) && (h.pin != pin)) {
            continue;
        }

        config.hw.pinNr = pin;

        config.hw.invert = false;

        handleEnumeratedDevice(config, h, callback, info);
    }
#endif

#if BREWPI_SENSOR_PINS
    for (uint8_t count = 0;
            (pin = deviceManager.enumerateSensorPins(count)) >= 0; count++) {
        if ((h.pin != -1) && (h.pin != pin)) {
            continue;
        }

        config.hw.pinNr = pin;

        handleEnumeratedDevice(config, h, callback, info);
    }
#endif

}

void DeviceManager::enumerateOneWireDevices(EnumerateHardware & h,
        EnumDevicesCallback callback, DeviceCallbackInfo * info) {
#if !BREWPI_SIMULATE
    int8_t pin;

    for (uint8_t count = 0; (pin = deviceManager.enumOneWirePins(count)) >= 0;
            count++) {
        DeviceConfig config;

        clear((uint8_t *) &config, sizeof(config));

        if ((h.pin != -1) && (h.pin != pin))
            continue;

        config.hw.pinNr = pin;
        config.chamber = 1;    // chamber 1 is default

        // logDebug("Enumerating one-wire devices on pin %d", pin);
        OneWire * wire = oneWireBus(pin);

        if (wire != nullptr){
            wire -> reset_search();

            while (wire->search(config.hw.address)) {
                // hardware device type from OneWire family ID
                switch (config.hw.address[0]) {
#if BREWPI_DS2413
                    case DS2413_FAMILY_ID:
                        config.deviceHardware = DEVICE_HARDWARE_ONEWIRE_2413;
                        break;
#endif
#if BREWPI_DS2408
                        case DS2408_FAMILY_ID :
                        config.deviceHardware = DEVICE_HARDWARE_ONEWIRE_2408;
                        break;
#endif

                    case DS18B20MODEL:
                        config.deviceHardware = DEVICE_HARDWARE_ONEWIRE_TEMP;
                        break;

                    default:
                        config.deviceHardware = DEVICE_HARDWARE_NONE;
                }

                switch (config.deviceHardware) {
#if BREWPI_DS2413 || BREWPI_DS2408
#if BREWPI_DS2413
                    case DEVICE_HARDWARE_ONEWIRE_2413:
#endif
#if BREWPI_DS2408
                        case DEVICE_HARDWARE_ONEWIRE_2408 : // 2408 will show as 2 valves
#endif
                        // enumerate each pin separately
                        for (uint8_t i = 0; i < 2; i++) {
                            config.hw.settings.actuator.pio = i;

                            handleEnumeratedDevice(config, h, callback, info);
                        }
                        break;
#endif

                    case DEVICE_HARDWARE_ONEWIRE_TEMP:

#if !ONEWIRE_PARASITE_SUPPORT
                    {    // check that device is not parasite powered
                        DallasTemperature sensor(wire);

                        // initialize sensor without reset detection (faster)
                        if (!sensor.isParasitePowered(config.hw.address)) {
                            handleEnumeratedDevice(config, h, callback, info);
                        }
                    }

#else
                        handleEnumeratedDevice(config, h, callback, info);
#endif

                        break;

                    default:
                        handleEnumeratedDevice(config, h, callback, info);
                }
            }
        }
    }
#endif

}

void DeviceManager::enumerateHardwareToStream(Stream & p) {
    EnumerateHardware spec;

    // set up defaults
    spec.unused = 0;     // list all devices
    spec.values = 0;     // don't list values
    spec.pin = -1;    // any pin
    spec.hardware = -1;    // any hardware
    spec.function = 0;     // no function restriction

    piLink.parseJson(handleHardwareSpec, &spec);

    DeviceCallbackInfo info;

    info.data = &p;

    // logDebug("Enumerating Hardware");
    firstDeviceOutput = true;

    enumerateHardware(spec, OutputEnumeratedDevices, &info);

    // logDebug("Enumerating Hardware Complete");
}

void DeviceManager::enumerateHardware(EnumerateHardware & spec,
        EnumDevicesCallback callback, DeviceCallbackInfo * info) {
    if ((spec.hardware == -1) || isOneWire(DeviceHardware(spec.hardware))) {
        enumerateOneWireDevices(spec, callback, info);
    }

    if ((spec.hardware == -1) || isDigitalPin(DeviceHardware(spec.hardware))) {
        enumeratePinDevices(spec, callback, info);
    }
}

void HandleDeviceDisplay(const char * key, const char * val, void * pv) {
    DeviceDisplay & dd = *(DeviceDisplay *) pv;

    // logDeveloper("DeviceDisplay %s:%s"), key, val);
    int8_t idx = indexOf("irwe", key[0]);

    if (idx >= 0) {
        *((int8_t *) &dd + idx) = atol(val);
    }
}

void DeviceManager::UpdateDeviceState(DeviceDisplay & dd, DeviceConfig &  dc, char * val, device_slot_t idx)
{
    DeviceType dt = deviceType(dc.deviceFunction, dc. deviceHardware);

    if (dt == DEVICETYPE_NONE) {
        return;
    }
    if(!isDefinedSlot(idx)){
        return;
    }
    if (dd.write >= 0) {
        if (dc.deviceFunction == DEVICE_CHAMBER_MANUAL_ACTUATOR){
            DEBUG_ONLY(logInfoInt(INFO_SETTING_ACTIVATOR_STATE, dd.write != 0));
            auto act = asInterface<ActuatorDigital>(devices[idx]);
            if(act != nullptr){
                act->setState(dd.write != 0 ? ActuatorDigital::State::Active : ActuatorDigital::State::Inactive);
            }
        }
    } else if (dd.value == 1){    // read values
        // todo add switch sensors
        if (dt == DEVICETYPE_TEMP_SENSOR){
            auto s = asInterface<TempSensor>(devices[idx]);
            if(s != nullptr){
                s->update();
                temp_t temp = s->read();
                temp.toTempString(val, 3, 9, tempControl.cc.tempFormat, true);
            }
        } else if (dt == DEVICETYPE_SWITCH_ACTUATOR){
            auto act = asInterface<ActuatorDigital>(devices[idx]);
            if(act != nullptr){
                sprintf(val, STR_FMT_U, act->getState());
            }
        } else if (dc.deviceFunction == DEVICE_CHAMBER_MANUAL_ACTUATOR){
#if BREWPI_DS2408
            if(dc.deviceHardware == DEVICE_HARDWARE_ONEWIRE_2408){
                readNotInstalledValve(dc.hw, val);
            }
#endif
            if (dc.deviceHardware == DEVICE_HARDWARE_PIN) {
                readNotInstalledPin(dc.hw, val);
            }
#if BREWPI_DS2413
            if (dc.deviceHardware == DEVICE_HARDWARE_ONEWIRE_2413) {
                readNotInstalledOneWirePin(dc.hw, val);
            }
#endif
        }
    }
}

void DeviceManager::listDevices(Stream & p) {
    DeviceConfig dc;
    DeviceDisplay dd;

    fill((int8_t *) &dd, sizeof(dd));

    dd.empty = 0;

    piLink.parseJson(HandleDeviceDisplay, (void *) &dd);

    deviceManager.beginDeviceOutput();

    for (device_slot_t idx = 0; deviceManager.allDevices(dc, idx); idx++) {
        if (deviceManager.enumDevice(dd, dc, idx)) {
            char val[10];

            val[0] = 0;

            UpdateDeviceState(dd, dc, val, idx);
            deviceManager.printDevice(idx, dc, val, p);
        }
    }
}

/*
 * Determines the class of device for the given DeviceID.
 */
DeviceType deviceType(DeviceFunction func, DeviceHardware hw)
{
    switch (func){
        case DEVICE_CHAMBER_DOOR :
            return DEVICETYPE_SWITCH_SENSOR;

        case DEVICE_CHAMBER_LIGHT :
        case DEVICE_CHAMBER_FAN :
        case DEVICE_CHAMBER_COOL :
        case DEVICE_CHAMBER_HEAT :
        case DEVICE_BEER_HEAT :
        case DEVICE_BEER_COOL :
            return DEVICETYPE_SWITCH_ACTUATOR;
        case DEVICE_CHAMBER_TEMP :
        case DEVICE_LOG1_TEMP :
        case DEVICE_LOG2_TEMP :
        case DEVICE_LOG3_TEMP :
        case DEVICE_BEER_TEMP :
        case DEVICE_BEER_TEMP2 :
            return DEVICETYPE_TEMP_SENSOR;
        case DEVICE_CHAMBER_MANUAL_ACTUATOR:
            return DEVICETYPE_SWITCH_ACTUATOR;
        default :
            return DEVICETYPE_NONE;
    }
}


/**
 * Find if a OneWire hardware chip is already used in another actuator, but with a different channel
 * @param find Device configuration for the newly installed hardware
 * @return pointer to low level hardware. nullptr if not found
 */
Interface * DeviceManager::findSharedHardware(DeviceConfig & find) {
    DeviceConfig config;
    Interface * hw = nullptr;

    for (device_slot_t slot = 0; deviceManager.allDevices(config, slot);
            slot++) {
        if (find.deviceHardware == config.deviceHardware) {
            bool match = true;
            switch (find.deviceHardware) {
#if BREWPI_DS2413
                case DEVICE_HARDWARE_ONEWIRE_2413:
#endif
#if BREWPI_DS2408
                case DEVICE_HARDWARE_ONEWIRE_2408:
#endif
#if BREWPI_DS2413 || BREWPI_DS2408
                    // Same address
                    match &= matchAddress(find.hw.address, config.hw.address,
                            8);

                    // Different pio
                    match &= find.hw.settings.actuator.pio
                            != config.hw.settings.actuator.pio;

                    if (match){
                        // device in slot has same hardware
                        hw = devices[slot];
                    }
                    break;
#endif
                default:
                    break;
            }
        }
    }

    return hw;
}

DeviceManager deviceManager;

