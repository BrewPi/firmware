/**
 ******************************************************************************
 * @file    ConnectedDevicesView.h
 * @authors Matthew McGowan
 * @date    25 February 2015
 ******************************************************************************
  Copyright (c) 2015 Brewpi/Matthew McGowan/Elco Jacobs.  All rights reserved.

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


#pragma once

#include "connected_device_widget.h"
#include "ConnectedDevicesManager.h"

/**
 * Constructed with the number of slots to present and a listener.
 * Listens to events about hardware slot changes
 * The UI elements are created statically, contained within the controlled devices widget.
*/ 
 
 /*
 * Each device has a widget that displays:
 * - value - larger font, centered. (if a sensor, state if an actuator) state may affect background color
 * - active/inactive(if the device has gone away)
 * - location (onewire address, pin number/name)
 * - index?
 */

typedef const D4D_OBJECT* DeviceStatusViewInitType;

class DeviceStatusView
{
    const D4D_OBJECT* pObject;
    
public:
    void init(DeviceStatusViewInitType init) {
        pObject = init;
        setConnected(false);
    }
    
    void invalidate()
    {
        D4D_InvalidateObject(pObject, 1);
    }
    
    void setValueText(const char* text) {
        // convert value to a string
        // set the value label
        D4D_CDV_SetValueText(pObject, text);
    }
    
    void setConnectionText(const char* text) {
        D4D_CDV_SetConnectionText(pObject, text);
    }
    
    void setConnected(bool connected) {                
        D4D_EnableObject(pObject, connected);
        D4D_EnableObject(D4D_CDV_CONNECTION(pObject), connected);
        D4D_EnableObject(D4D_CDV_VALUE(pObject), connected);
    }
    
};

/**
 * Presentation model. Wraps the D4D stuff.
 */
class DeviceStatusPresenter
{
    DeviceStatusView view;
    
    const ConnectedDevice* device;
    static char buf[17];    // just used as temp storage
public:
    
    void setDevice(const ConnectedDevice* device) { this->device = device; }
    
    DeviceStatusView& getView() {
        return view;
    }
    
    void update(ConnectedDeviceChange change) {
        bool connected = (change!=REMOVED);
        view.setConnected(connected);
        
        if(change != ADDED){
            // skip printing value when sensor has just been connected
            // and value is still null
            buf[0] = 0;
            valueAsText(device, buf, sizeof(buf));
            view.setValueText(buf);
        }

        buf[0] = 0;
        connectionAsText(device, buf, sizeof(buf));
        view.setConnectionText(buf);        
        
        view.invalidate();
    }
    
};


#define MAX_PRESENTED_DEVICES   ((std::size_t)MAX_CONNECTED_DEVICES)

class ConnectedDevicesPresenter 
{    
    DeviceStatusPresenter status[MAX_PRESENTED_DEVICES];

public:
    
    ConnectedDevicesPresenter(ConnectedDevicesManager* mgr, DeviceStatusViewInitType* viewInit, std::size_t max_devices);

    void notifyChanged(const ConnectedDevicesManager* mgr, int slot, const ConnectedDevice* device, ConnectedDeviceChange change);
           
};

