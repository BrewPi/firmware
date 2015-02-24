#pragma once

/**
 ******************************************************************************
 * @file    ConnectedDevicesManager.cpp
 * @authors Matthew McGowan
 * @date    22 February 2015
 ******************************************************************************
  Copyright (c) 2015 Brewpi/Matthew McGowan All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */
#include "EepromManager.h"		// for clear()
#include "OneWireDevices.h"
#include "DeviceManager.h"
#include "TempSensorBasic.h"
#include "Actuator.h"
#include "Sensor.h"
#include "PiLink.h"

#define MAX_TEMP_SENSORS

/**
 * Periodically scan the device manager.
 * Don't use DM's value function since it waits 750ms per device.
 *  Looks specifically for temperature sensors. 
 */

#define MAX_CONNECTED_DEVICES 4

struct ConnectedDevice {
    DeviceAddress address;
    int8_t lastSeen;
    DeviceType dt;

    union {
        temperature temp;
        bool state;
    } value;

    union {
        void* any;
        BasicTempSensor* tempSensor;
        Actuator* actuator;
        SwitchSensor* sensor;
    } pointer;
};

enum ConnectedDeviceChange {
    ADDED,
    UPDATED,
    REMOVED
};

class ConnectedDevicesManager;
typedef void (*NotifyDevicesChanged)(ConnectedDevicesManager* mgr, int slot, ConnectedDevice* device, ConnectedDeviceChange change);

class ConnectedDevicesManager {
    ConnectedDevice devices[MAX_CONNECTED_DEVICES];
    NotifyDevicesChanged changed;

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
            if (!memcmp(device.address, config->hw.address, 8)
                    && (!active || (device.pointer.any && device.lastSeen >= 0 && device.lastSeen <= 2))) {
                slot = i;
                break;
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
            if (!device.pointer.any || device.lastSeen > 2) {
                slot = i;
                break;
            }
        }
        return slot;
    }

    void clearSlot(int slot) {
        ConnectedDevice& connectedDevice = devices[slot];
        DeviceManager::disposeDevice(connectedDevice.dt, connectedDevice.pointer.any);
        if (connectedDevice.pointer.any)
            connectedDevice.lastSeen = 0; // flag to send the REMOVED event
        connectedDevice.pointer.any = NULL;
    }

    static void deviceCallback(DeviceConfig* config, DeviceCallbackInfo* info) {
        ConnectedDevicesManager* cdm = (ConnectedDevicesManager*) info->data;
        cdm->handleDevice(config, info);
    }

    void handleDevice(DeviceConfig* config, DeviceCallbackInfo* info) {
        if (config->deviceHardware == DEVICE_HARDWARE_ONEWIRE_TEMP) {
            char addressString[17];
            printBytes(config->hw.address, 8, addressString);

            int slot = existingSlot(config);
            if (slot >= 0) { // found the device still active
                devices[slot].lastSeen = 0; // seen this one now
                devices[slot].value.temp = devices[slot].pointer.tempSensor->read();
                if(devices[slot].value.temp == TEMP_SENSOR_DISCONNECTED){
                    devices[slot].pointer.tempSensor->init();                    
                }
                changed(this, slot, devices + slot, UPDATED);
            } else {
                // attempt to reuse previous location
                slot = existingSlot(config, false);
                if (slot < 0)
                    slot = freeSlot(config);
                if (slot >= 0) {
                    clearSlot(slot);
                    sendRemoveEvent(slot);
                    ConnectedDevice& device = devices[slot];
                    device.lastSeen = 0;
                    device.dt = DEVICETYPE_TEMP_SENSOR;
                    memcpy(device.address, config->hw.address, 8);
                    device.value.temp = MIN_TEMP; // flag invalid
                    device.pointer.tempSensor = (BasicTempSensor*) DeviceManager::createDevice(*config, device.dt);
                    if (!device.pointer.tempSensor || !device.pointer.tempSensor->init()) {
                        clearSlot(slot);
                        device.lastSeen = -1; // don't send REMOVED event since no added event has been sent
                    } else
                        changed(this, slot, devices + slot, ADDED); // new device added					
                }
                // just ignore the device - not enough free slots
            }
        }
    }

    void sendRemoveEvent(int i) {
        if (!devices[i].pointer.any && devices[i].lastSeen != -1) {
            devices[i].lastSeen = -1;
            changed(this, i, devices + i, REMOVED);
        }
    }

public:

    ConnectedDevicesManager(NotifyDevicesChanged listener) {
        clear((uint8_t*) & devices, sizeof (devices));
        changed = listener;
    }

    ~ConnectedDevicesManager() {
        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            clearSlot(i);
        }
    }

    void update() {
        DeviceCallbackInfo info;
        info.data = this;
        EnumerateHardware spec;
        memset(&spec, 0, sizeof (spec));
        spec.pin = -1; // any pin
        spec.hardware = -1; // any hardware

        // increment the last seen for all devices        
        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            if (devices[i].pointer.any)
                devices[i].lastSeen++;
        }
        DeviceManager::enumerateHardware(spec, deviceCallback, &info);

        // flag disconnected slots as disconnected (once only)
        // This also ensures that the first update posts the states of disconnected slots.
        for (int i = 0; i < MAX_CONNECTED_DEVICES; i++) {
            if (devices[i].lastSeen > 2) {
                clearSlot(i);
            }
            sendRemoveEvent(i);
        }

    }

};
