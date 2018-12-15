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

#include "WidgetsScreen.h"
#include "BrewBlox.h"
#include "ProcessValueWidget.h"
#include "blox/DisplaySettingsBlock.h"
#include "connectivity.h"
#include "screen.h"
#include <algorithm>
#include <array>
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

SmallColorScheme TOP_BAR_SCHEME = {
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in standard state
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in disabled state
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in focused state
    D4D_COLOR_RGB(0, 0, 0),      ///< The object background color in captured state
    D4D_COLOR_RGB(64, 100, 128), ///< The object fore color in standard state
    D4D_COLOR_RGB(64, 64, 64),   ///< The object fore color in disabled state
    D4D_COLOR_RGB(64, 100, 200), ///< The object fore color in focused state
    D4D_COLOR_RGB(64, 100, 255), ///< The object fore color in captured state
};

D4D_DECLARE_LABEL(scrWidgets_usb_icon, "\x21", 0, 0, 20, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_ICON, nullptr, nullptr);
D4D_DECLARE_LABEL(scrWidgets_usb_text, "USB", 20, 0, 20, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_REGULAR, nullptr, nullptr);

D4D_DECLARE_LABEL(scrWidgets_wifi_icon, wifi_icon, 40, 0, 20, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_ICON, nullptr, nullptr);

#undef D4D_LBL_TXT_PRTY_DEFAULT
#define D4D_LBL_TXT_PRTY_DEFAULT (D4D_TXT_PRTY_ALIGN_H_LEFT_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
D4D_DECLARE_LABEL(scrWidgets_wifi_ip, wifi_ip, 60, 0, 15 * 6, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_REGULAR, nullptr, nullptr);

#undef D4D_LBL_TXT_PRTY_DEFAULT
#define D4D_LBL_TXT_PRTY_DEFAULT (D4D_TXT_PRTY_ALIGN_H_CENTER_MASK | D4D_TXT_PRTY_ALIGN_V_CENTER_MASK)
char screen_title[40] = "Edit this screen in the web interface";
D4D_DECLARE_LABEL(scrWidgets_title, screen_title, 40, 220, 240, 20, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&TOP_BAR_SCHEME), FONT_REGULAR, nullptr, nullptr);

std::array<WidgetWrapper, 6> widgetWrappers = {
    WidgetWrapper(0),
    WidgetWrapper(1),
    WidgetWrapper(2),
    WidgetWrapper(3),
    WidgetWrapper(4),
    WidgetWrapper(5),
};

D4D_DECLARE_STD_SCREEN_BEGIN(widgets_screen, scrWidgets_)
&scrWidgets_usb_icon,
    &scrWidgets_usb_text,
    &scrWidgets_wifi_icon,
    &scrWidgets_wifi_ip,
    &scrWidgets_title,
    widgetWrappers[0].pObj(),
    widgetWrappers[1].pObj(),
    widgetWrappers[2].pObj(),
    widgetWrappers[3].pObj(),
    widgetWrappers[4].pObj(),
    widgetWrappers[5].pObj(),
    D4D_DECLARE_SCREEN_END();

std::vector<std::unique_ptr<WidgetBase>> WidgetsScreen::widgets;

void
WidgetsScreen::loadSettings()
{
    auto& settings = DisplaySettingsBlock::settings();
    if (settings.name[0] != 0) {
        D4D_SetText(&scrWidgets_title, settings.name);
    }

    widgets.clear();
    pb_size_t numWidgets = std::min(settings.widgets_count, pb_size_t(sizeof(settings.widgets) / sizeof(settings.widgets[0])));
    for (pb_size_t i = 0; i < numWidgets; ++i) {
        blox_DisplaySettings_Widget widgetDfn = settings.widgets[i];
        auto pos = widgetDfn.pos;
        if (pos == 0 || pos > 6) {
            continue; // invalid position on screen
        }
        switch (widgetDfn.which_obj) {
        case blox_DisplaySettings_Widget_ProcessValue_tag:
            WidgetWrapper& wrapper = widgetWrappers[pos - 1];
            wrapper.setName(widgetDfn.name);
            wrapper.setColor(widgetDfn.bg_color[0], widgetDfn.bg_color[1], widgetDfn.bg_color[2]);
            auto w = std::make_unique<ProcessValueWidget>(wrapper, cbox::obj_id_t(widgetDfn.obj.ProcessValue));
            widgets.push_back(std::move(w));
        }
    }

    D4D_InvalidateScreen(&widgets_screen, D4D_TRUE);
}

void
WidgetsScreen::activate()
{
    D4D_ActivateScreen(&widgets_screen, D4D_TRUE);
    loadSettings();
}

void
WidgetsScreen::updateUsb()
{
    bool connected = serialConnected();
    D4D_EnableObject(&scrWidgets_usb_icon, connected);
    D4D_EnableObject(&scrWidgets_usb_text, connected);
}

void
WidgetsScreen::updateWiFi()
{
    auto signal = wifiSignal();
    printWiFiIp(wifi_ip);

    if (signal >= 0) {
        wifi_icon[0] = 0x22;
        D4D_EnableObject(&scrWidgets_wifi_icon, false);
        D4D_EnableObject(&scrWidgets_wifi_ip, false);
        return;
    }
    if (signal >= -65) {
        wifi_icon[0] = 0x25;
    } else if (signal >= -70) {
        wifi_icon[0] = 0x24;
    } else {
        wifi_icon[0] = 0x23;
    }
    D4D_EnableObject(&scrWidgets_wifi_icon, true);
    D4D_EnableObject(&scrWidgets_wifi_ip, true);
}

void
WidgetsScreen::updateWidgets()
{
    for (auto& w : widgets) {
        if (w) {
            w->update();
        }
    }
}

void
scrWidgets_OnInit()
{
}

void
scrWidgets_OnMain()
{
    if (DisplaySettingsBlock::newSettingsReceived()) {
        WidgetsScreen::loadSettings();
    }
    WidgetsScreen::updateUsb();
    WidgetsScreen::updateWiFi();
    WidgetsScreen::updateWidgets();
}

void
scrWidgets_OnActivate()
{
}

void
scrWidgets_OnDeactivate()
{
}

uint8_t
scrWidgets_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    return D4D_FALSE; // don't block further processing
}
