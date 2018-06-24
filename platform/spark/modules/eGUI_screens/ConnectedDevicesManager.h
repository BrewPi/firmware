/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
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

#pragma once

#include "EepromManager.h"		// for clear()
#include "DeviceManager.h"
#include "TempSensor.h"
#include "ActuatorInterfaces.h"
#include "PiLink.h"
#include "Board.h"
#include "ActuatorPin.h"
#include "temperatureFormats.h"
#include <functional>
#include "OneWireAddress.h"
#include "RefTo.h"

#define MAX_TEMP_SENSORS


#define MAX_CONNECTED_DEVICES 6



/**
 * Describes a device presently connected or previously connected to the controller.
 */
struct ConnectedDevice
{
    int8_t lastSeen;
    DeviceType dt;
    DeviceHardware dh;

    struct Connection {
        DeviceConnection type;
        union {
            DeviceAddress address;  // type == DEVICECONNECTION_ONEWIRE
            int pin;                // type == DEVICECONNECTION_PIN
        };
    } connection;

    union Value {
        temp_t temp;       // dt==DEVICETYPE_TEMP_SENSOR
        bool state;             // dt==DEVICETYPE_SWITCH_SENSOR ||
                                // dt==DEVICETYPE_ACTUATOR
        Value(){} // needed because temp_t has non-trivial constructor
        Value(const Value& v){
            temp = v.temp; // copy biggest type
        }
    } value;

    Interface * pointer;
};

char * valueAsText(const ConnectedDevice* device, char* buf, size_t len);
void connectionAsText(const ConnectedDevice* device, char* buf, size_t len);
char* itoa(int i, char b[]);

enum ConnectedDeviceChange
{
    ADDED,
    UPDATED,
    REMOVED
};

class ConnectedDevicesManager;

typedef std::function<void(const ConnectedDevicesManager* mgr, int slot, const ConnectedDevice* device, ConnectedDeviceChange change)> NotifyDevicesChanged;

class ConnectedDevicesManager
{
    ConnectedDevice devices[MAX_CONNECTED_DEVICES];
    NotifyDevicesChanged changed;

    ActuatorDigital* actuators[MAX_ACTUATOR_COUNT];

    /**
     * Find a slot that matches the given device config.
     * The slot is matching if the address matches.
     * Furthermore, if an active device is required, then the slot must have a configured device and
     * must have been seen recently.
     */
    int existingSlot(DeviceConfig* config, bool active = true) {
        int slot = -1;
        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            ConnectedDevice& device = devices[i];

            if (config->deviceHardware==device.dh) {        // same hardware type
                if (device.connection.type==DEVICE_CONNECTION_ONEWIRE &&
                    !memcmp(device.connection.address, config->hw.address, 8) &&
                    (!active || (device.pointer && device.lastSeen >= 0 && device.lastSeen <= 2)))
                {
                slot = i;
                break;
                }
            }
        }
        return slot;
    }

    /**
     * Finds a free slot that can be cleared and overwritten.
     */
    int freeSlot(DeviceConfig* config) {
        int slot = -1;
        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            ConnectedDevice& device = devices[i];
            if (!device.pointer || device.lastSeen > 2) {
                slot = i;
                break;
            }
        }
        return slot;
    }

    void clearSlot(int slot) {
        ConnectedDevice& connectedDevice = devices[slot];
        delete connectedDevice.pointer;
        if (connectedDevice.pointer)
            connectedDevice.lastSeen = 0; // flag to send the REMOVED event
        connectedDevice.pointer = nullptr;
    }

    static void deviceCallback(DeviceConfig* config, DeviceCallbackInfo* info) {
        ConnectedDevicesManager* cdm = (ConnectedDevicesManager*) info->data;
        cdm->handleDevice(config, info);
    }

    void handleDevice(DeviceConfig* config, DeviceCallbackInfo* info);

    void sendRemoveEvent(int i) {
        if (!devices[i].pointer && devices[i].lastSeen != -1) {
            devices[i].lastSeen = -1;
            changed(this, i, devices + i, REMOVED);
        }
    }

public:

    ConnectedDevicesManager() {
        clear((uint8_t*) & devices, sizeof (devices));
        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            devices[i].dt = DEVICETYPE_NONE;
        }

        // todo - pull the definitions of the static devices from the device manager.
        actuators[0] = new ActuatorPin(PIN_ACTUATOR0, BREWPI_INVERT_ACTUATORS);
        actuators[1] = new ActuatorPin(PIN_ACTUATOR1, BREWPI_INVERT_ACTUATORS);
        actuators[2] = new ActuatorPin(PIN_ACTUATOR2, BREWPI_INVERT_ACTUATORS);
        actuators[3] = new ActuatorPin(PIN_ACTUATOR3, BREWPI_INVERT_ACTUATORS);
#if(MAX_ACTUATOR_COUNT > 4)
        actuators[4] = new ActuatorPin(PIN_ACTUATOR4, BREWPI_INVERT_ACTUATORS);
#endif
    }

    ~ConnectedDevicesManager() {
        for (int i=0; i<MAX_ACTUATOR_COUNT; i++) {
            delete actuators[i];
        }

        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            clearSlot(i);
        }
    }

    void setListener(NotifyDevicesChanged listener)
    {
        changed = listener;
    }

    void update();

    ActuatorDigital* actuator(size_t index) {
        return actuators[index];
    }

    const ConnectedDevice* device(size_t index) {
        return &devices[index];
    }
};
