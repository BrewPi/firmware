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

#include "ConnectivityDisplay.h"

char controller_wifi_ip[16]= "";
ControllerWiFiView wifiView;
ControllerWiFiPresenter wifiPresenter(wifiView);

ControllerUSBView usbView;
ControllerUSBPresenter usbPresenter(usbView);


void ControllerUSBView::update(bool serialConnected)
{
    if(obj != nullptr){
        obj->clrScheme->fore = serialConnected ? D4D_COLOR_LIGHT_GREY : D4D_COLOR_GREY;
    }
}

void ControllerUSBPresenter::update(){
    bool serialConnected = Serial.isConnected();
    view_.update(serialConnected);
}

void ControllerWiFiView::update(bool wifiConnected, char * const ipAddress)
{
    if(obj != nullptr){
        obj->clrScheme->fore = wifiConnected ? D4D_COLOR_LIGHT_GREY : D4D_COLOR_GREY;
        D4D_SetText(obj, ipAddress);
    }
}

void ControllerWiFiPresenter::update(){
    char ipAddressString[16];
    bool wifiConnected = WiFi.ready();
    piLink.ipAddressAsString(ipAddressString);
    view_.update(wifiConnected, ipAddressString);
}
