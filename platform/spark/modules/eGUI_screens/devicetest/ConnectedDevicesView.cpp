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
    max_devices = min(max_devices, MAX_PRESENTED_DEVICES);
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

const D4D_OBJECT* actuator_views[] = { &scrDeviceTest_actuator0, &scrDeviceTest_actuator1, &scrDeviceTest_actuator2, &scrDeviceTest_actuator3 };

Actuator* actuatorForView(const D4D_OBJECT* pThis)
{
    Actuator* actuator = NULL;
    for (unsigned i=0; i<arraySize(actuator_views); i++) {
        if (actuator_views[i]==pThis) {
            actuator = mgr.actuator(i);
        }
    }
    return actuator;
}

extern "C" void ActuatorClicked(D4D_OBJECT* pThis)
{
    int idx = -1;
    if (pThis==&scrDeviceTest_actuator0)
        idx = 0;
    else if (pThis==&scrDeviceTest_actuator1)
        idx = 1;
    if (pThis==&scrDeviceTest_actuator2)
        idx = 2;
    if (pThis==&scrDeviceTest_actuator3)
        idx = 3;

    if (idx>=0) {
        Actuator* actuator = mgr.actuator(idx);
        bool active = !actuator->isActive();
        actuator->setActive(active);
        SetActuatorButtonState(pThis, active, idx);
    }
}

void ScreenDeviceTest_OnMain()
{
    for (unsigned i=0; i<arraySize(actuator_views); i++) {
        const D4D_OBJECT* obj = actuator_views[i];
        Actuator* actuator = actuatorForView(obj);
        SetActuatorButtonState(obj, actuator->isActive(), i);
    }

    static uint32_t last = 0;
    static uint32_t updateTime = 0;
    uint32_t now = millis();
    // make sure updating is only taking 25% of CPU time
    if (now-last>=4*updateTime) {
        last = now;
        mgr.update();
        updateTime = millis()-now;
    }
}

void ScreenDeviceTest_OnActivate()
{
}

void ScreenDeviceTest_OnDeactivate()
{
}

Byte ScreenDeviceTest_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    return 0;
}
