/**
 ******************************************************************************
 * @file    ConnectedDevicesView.cpp
 * @authors Matthew McGowan
 * @date    26 February 2015
 ******************************************************************************
  Copyright (c) 2015 Brewpi/Matthew McGowan.  All rights reserved.

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

#include <functional>
#include <algorithm>

#include "ConnectedDevicesView.h"
#include "ConnectedDevicesManager.h"
#include "device_test_screen.h"

using namespace std::placeholders;

char DeviceStatusPresenter::buf[17];    // just used as temp storage

ConnectedDevicesPresenter::ConnectedDevicesPresenter(ConnectedDevicesManager* mgr, 
    DeviceStatusViewInitType* viewInit, std::size_t max_devices)
{
    max_devices = std::min(max_devices, MAX_PRESENTED_DEVICES);
    for (unsigned i=0; i<max_devices; i++) {
        status[i].getView().init(viewInit[i]);
        status[i].setDevice(mgr->device(i));
    }                
    NotifyDevicesChanged listener = std::bind(&ConnectedDevicesPresenter::notifyChanged, this, _1, _2, _3, _4);
    mgr->setListener(listener);
}


void ConnectedDevicesPresenter::notifyChanged(const ConnectedDevicesManager* mgr, int slot, const ConnectedDevice* device, ConnectedDeviceChange change) {
    DeviceStatusPresenter pres = status[slot];
    pres.update(change);
}


const D4D_OBJECT* views[] = { &scrDeviceTest_devices00, &scrDeviceTest_devices10, &scrDeviceTest_devices20, &scrDeviceTest_devices01, &scrDeviceTest_devices11, &scrDeviceTest_devices21 };
ConnectedDevicesManager mgr;
ConnectedDevicesPresenter presenter(&mgr, views, 6);

extern "C" void ActuatorClicked(D4D_OBJECT* pThis)
{
    int idx = -1;
    if (pThis==&scrDeviceTest_actuator1)
        idx = 0;
    else if (pThis==&scrDeviceTest_actuator2)
        idx = 1;
    if (pThis==&scrDeviceTest_actuator3)
        idx = 2;
    
    if (idx>=0) {
        Actuator* actuator = mgr.actuator(idx);
        bool active = !actuator->isActive();
        actuator->setActive(active);
        SetActuatorButtonState(pThis, active);
    }
}
