/*
 * Copyright 2017 BrewPi / Elco Jacobs
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

extern "C" {
    #include "d4d.h"
}
#include "Platform.h"
#include "PiLink.h"

class ControllerWiFiView
{
    const D4D_OBJECT* obj;

public:
    ControllerWiFiView(){
        this->obj = nullptr;
    }

    ControllerWiFiView(const D4D_OBJECT* wifiObj){
        this->obj = wifiObj;
    }

    void setTarget(const D4D_OBJECT* wifiObj){
        this->obj = wifiObj;
    }

    void update(bool wifiConnected, char * const ipAddress);
};

class ControllerWiFiPresenter
{
public:
    ControllerWiFiView& view_;

public:

    ControllerWiFiPresenter(ControllerWiFiView& view)
        : view_(view) {}

    void update();
};

class ControllerUSBView
{
    const D4D_OBJECT* obj;

public:
    ControllerUSBView(){
        this->obj = nullptr;
    }
    ControllerUSBView(const D4D_OBJECT* usbObj){
        this->obj = usbObj;
    }

    void setTarget(const D4D_OBJECT* usbObj){
        this->obj = usbObj;
    }

    void update(bool serialConnected);
};


class ControllerUSBPresenter
{
public:
    ControllerUSBView& view_;

public:

    ControllerUSBPresenter(ControllerUSBView& view)
        : view_(view) {}

    void update();
};

extern ControllerWiFiView wifiView;
extern ControllerWiFiPresenter wifiPresenter;

extern ControllerUSBView usbView;
extern ControllerUSBPresenter usbPresenter;


