/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewBlox.
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

#include "ProcessValuesScreen.h"
#include "../screen.h"
#include "../widget_color_scheme.h"
#include "BrewBlox.h"
#include "ProcessValueWidget.h"
#include "application.h"
#include <algorithm>
#include <vector>

#define str(s) #s
#define xstr(s) str(s)

char wifi_ip[16] = "0.0.0.0";
char wifi_icon[] = "\x22";

#define SCR_PV_X0 5
#define SCR_PV_Y0 20
#define SCR_PV_CX 100
#define SCR_PV_CX_GAP 5
#define SCR_PV_CX_OFFSET (SCR_PV_CX + SCR_PV_CX_GAP)
#define SCR_PV_CY 98
#define SCR_PV_CY_GAP 5
#define SCR_PV_CY_OFFSET (SCR_PV_CY + SCR_PV_CY_GAP)

WIDGET_COLOR_SCHEME TOP_BAR_SCHEME = {
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in standard state
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in disabled state
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in focused state
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in captured state
    D4D_COLOR_RGB(64, 100, 128), ///< The object fore color in standard state
    D4D_COLOR_RGB(64, 64, 64),   ///< The object fore color in disabled state
    D4D_COLOR_RGB(64, 100, 200), ///< The object fore color in focused state
    D4D_COLOR_RGB(64, 100, 255), ///< The object fore color in captured state
};

D4D_DECLARE_LABEL(scrPV_usb_icon, "\x21", 0, 0, 20, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_ICON, nullptr, nullptr);
D4D_DECLARE_LABEL(scrPV_usb_text, "USB", 20, 0, 20, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_REGULAR, nullptr, nullptr);

D4D_DECLARE_LABEL(scrPV_wifi_icon, wifi_icon, 40, 0, 20, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_ICON, nullptr, nullptr);

#undef D4D_LBL_TXT_PRTY_DEFAULT
#define D4D_LBL_TXT_PRTY_DEFAULT (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
D4D_DECLARE_LABEL(scrPV_wifi_ip, wifi_ip, 60, 0, 15 * 6, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_REGULAR, nullptr, nullptr);

#undef D4D_LBL_TXT_PRTY_DEFAULT
#define D4D_LBL_TXT_PRTY_DEFAULT (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
D4D_DECLARE_LABEL(scrPV_title, "All Processes", 40, 220, 240, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_REGULAR, nullptr, nullptr);

D4D_DECLARE_PV(scrPV_pv00, SCR_PV_X0 + SCR_PV_CX_OFFSET * 0, SCR_PV_Y0 + SCR_PV_CY_OFFSET * 0, SCR_PV_CX, SCR_PV_CY);
D4D_DECLARE_PV(scrPV_pv01, SCR_PV_X0 + SCR_PV_CX_OFFSET * 1, SCR_PV_Y0 + SCR_PV_CY_OFFSET * 0, SCR_PV_CX, SCR_PV_CY);
D4D_DECLARE_PV(scrPV_pv02, SCR_PV_X0 + SCR_PV_CX_OFFSET * 2, SCR_PV_Y0 + SCR_PV_CY_OFFSET * 0, SCR_PV_CX, SCR_PV_CY);
D4D_DECLARE_PV(scrPV_pv10, SCR_PV_X0 + SCR_PV_CX_OFFSET * 0, SCR_PV_Y0 + SCR_PV_CY_OFFSET * 1, SCR_PV_CX, SCR_PV_CY);
D4D_DECLARE_PV(scrPV_pv11, SCR_PV_X0 + SCR_PV_CX_OFFSET * 1, SCR_PV_Y0 + SCR_PV_CY_OFFSET * 1, SCR_PV_CX, SCR_PV_CY);
D4D_DECLARE_PV(scrPV_pv12, SCR_PV_X0 + SCR_PV_CX_OFFSET * 2, SCR_PV_Y0 + SCR_PV_CY_OFFSET * 1, SCR_PV_CX, SCR_PV_CY);

D4D_DECLARE_STD_SCREEN_BEGIN(screen_process_values, scrPV_)

D4D_DECLARE_SCREEN_OBJECT(scrPV_usb_icon)
D4D_DECLARE_SCREEN_OBJECT(scrPV_usb_text)
D4D_DECLARE_SCREEN_OBJECT(scrPV_wifi_icon)
D4D_DECLARE_SCREEN_OBJECT(scrPV_wifi_ip)
D4D_DECLARE_SCREEN_OBJECT(scrPV_title)

D4D_DECLARE_SCREEN_OBJECT(scrPV_pv00)
D4D_DECLARE_SCREEN_OBJECT(scrPV_pv01)
D4D_DECLARE_SCREEN_OBJECT(scrPV_pv02)
D4D_DECLARE_SCREEN_OBJECT(scrPV_pv10)
D4D_DECLARE_SCREEN_OBJECT(scrPV_pv11)
D4D_DECLARE_SCREEN_OBJECT(scrPV_pv12)

D4D_DECLARE_SCREEN_END()

std::vector<ProcessValueWidget> ProcessValuesScreen::pvWidgets;

void
ProcessValuesScreen::init()
{
    pvWidgets.emplace_back(&scrPV_pv00);
    pvWidgets.emplace_back(&scrPV_pv01);
    pvWidgets.emplace_back(&scrPV_pv02);
    pvWidgets.emplace_back(&scrPV_pv10);
    pvWidgets.emplace_back(&scrPV_pv11);
    pvWidgets.emplace_back(&scrPV_pv12);
}

void
ProcessValuesScreen::activate()
{
    init();
    D4D_ActivateScreen(&screen_process_values, D4D_TRUE);
}

void
ProcessValuesScreen::updateUsb()
{
    bool connected = Serial.isConnected();
    D4D_EnableObject(&scrPV_usb_icon, connected);
    D4D_EnableObject(&scrPV_usb_text, connected);
}

void
ProcessValuesScreen::updateWiFi()
{
    int signal = WiFi.RSSI();
    IPAddress ip = WiFi.localIP();
    snprintf(wifi_ip, 16, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    if (signal >= 0) {
        wifi_icon[0] = 0x22;
        D4D_EnableObject(&scrPV_wifi_icon, false);
        D4D_EnableObject(&scrPV_wifi_ip, false);
        return;
    }
    if (signal >= -65) {
        wifi_icon[0] = 0x25;
    } else if (signal >= -70) {
        wifi_icon[0] = 0x24;
    } else {
        wifi_icon[0] = 0x23;
    }
    D4D_EnableObject(&scrPV_wifi_icon, true);
    D4D_EnableObject(&scrPV_wifi_ip, true);
}
/*
void
ProcessValuesScreen::searchPVs()
{
    auto it = objects.cbegin();

    for (auto& w : pvWidgets) {
        if (it != objects.cend()) {
            if (it->object()->implements(cbox::interfaceId<ProcessValue<temp_t>>())) {
                w.setId(it->id());
            }
            ++it;
        } else {
            w.setId(0);
        }
    }
}*/

void
ProcessValuesScreen::updatePVs()
{
    for (auto& w : pvWidgets) {
        w.update();
    }
}

void
scrPV_OnInit()
{
}

void
scrPV_OnMain()
{
    ProcessValuesScreen::updateUsb();
    ProcessValuesScreen::updateWiFi();
    ProcessValuesScreen::updatePVs();
}

void
scrPV_OnActivate()
{
}

void
scrPV_OnDeactivate()
{
}

uint8_t
scrPV_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    return D4D_FALSE; // don't block further processing
}
