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

#include <algorithm>
#include <functional>

#include "Board.h"
#include "BrewBlox.h"
#include "ConnectivityDisplay.h"
#include "ProcessValue.h"
#include "ProcessValueView.h"
#include "Temperature.h"
#include "process_value_screen.h"

using namespace std::placeholders;

void
ProcessValueView::update()
{
    if (auto pv = pvLookup.const_lock()) {
        D4D_SetText(pObject->pRelations[0], "value");
        D4D_SetText(pObject->pRelations[1], "setting");
        D4D_SetText(pObject->pRelations[2], "name");
    }
}

void
ProcessValueScreenMgr::search()
{
    auto it = objects.cbegin();

    for (auto& view : pvViews) {
        if (it != objects.cend()) {
            if (it->object()->implements(cbox::interfaceId<ProcessValue<temp_t>>())) {
                view.setId(it->id());
            }
            ++it;
        } else {
            view.setId(0);
        }
    }
}

void
ProcessValueScreenMgr::update()
{
    for (auto& view : pvViews) {
        view.update();
    }
}

//ProcessValueScreenMgr pvScreenMgr;

void
ScreenProcessValues_OnInit()
{
    wifiView.setTarget(&scrProcessValues_wifi_state);
    usbView.setTarget(&scrProcessValues_usb_state);
}

void
ScreenProcessValues_OnMain()
{
    //pvScreenMgr.search();
    //pvScreenMgr.update();
    usbPresenter.update();
    wifiPresenter.update();
}

void
ScreenProcessValues_OnActivate()
{
}

void
ScreenProcessValues_OnDeactivate()
{
}

Byte
ScreenProcessValues_OnObjectMsg(D4D_MESSAGE* pMsg)
{
    D4D_UNUSED(pMsg);
    return 0;
}
