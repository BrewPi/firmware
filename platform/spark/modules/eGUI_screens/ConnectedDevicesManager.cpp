#include "ConnectedDevicesManager.h"
#include "UI.h"

void valueAsText(const ConnectedDevice* device, char* buf, size_t len) {
    if (device->dt==DEVICETYPE_TEMP_SENSOR) {        
        tempToString(buf, device->value.temp, 1, len);        
    }
    else if (device->dt==DEVICETYPE_SWITCH_ACTUATOR || device->dt==DEVICETYPE_SWITCH_SENSOR) {
        strncpy(buf, device->value.state ? "On" : "Off", len);
    }
    else {
        buf[0] = 0;
    }
    buf[len-1] = 0;
}

void connectionAsText(const ConnectedDevice* device, char* buf, size_t len) {
    if (device->dt==DEVICETYPE_NONE) {
        buf[0] = 0;
    }    
    else if (device->connection.type==DEVICE_CONNECTION_ONEWIRE && len>16) {
        printBytes(device->connection.address, 8, buf);        
    }
    else {
        itoa(device->connection.pin, buf);        
    }
    buf[len-1] = 0;
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}


void ConnectedDevicesManager::handleDevice(DeviceConfig* config, DeviceCallbackInfo* info) 
{    
    if (config->deviceHardware == DEVICE_HARDWARE_ONEWIRE_TEMP) {     
        int slot = existingSlot(config);
        if (slot >= 0) { // found the device still active
            temperature newTemp = devices[slot].pointer.tempSensor->read();
            if(newTemp == TEMP_SENSOR_DISCONNECTED){
                devices[slot].lastSeen+=2;                
            } 
            else {
                devices[slot].lastSeen = 0; // seen this one now
                // check if temperature has changed to notify that UI needs to be updated
                if(newTemp != devices[slot].value.temp){
                    devices[slot].value.temp = newTemp;
                    changed(this, slot, devices + slot, UPDATED);
                }
            }                                
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

                device.dh = config->deviceHardware;
                device.dt = device.dh == DEVICE_HARDWARE_ONEWIRE_TEMP ? DEVICETYPE_TEMP_SENSOR : DEVICETYPE_SWITCH_ACTUATOR;
                device.connection.type = deviceConnection(device.dh);
                memcpy(device.connection.address, config->hw.address, 8);
                device.value.temp = INVALID_TEMP; // flag invalid
                device.pointer.tempSensor = (BasicTempSensor*) DeviceManager::createDevice(*config, device.dt);
                if (!device.pointer.tempSensor || !device.pointer.tempSensor->init()) {
                    clearSlot(slot);
                    device.lastSeen = -1; // don't send REMOVED event since no added event has been sent
                } else
                    changed(this, slot, &device, ADDED); // new device added					
            }
            // just ignore the device - not enough free slots
        }
    }
}

void ConnectedDevicesManager::update() 
{
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
