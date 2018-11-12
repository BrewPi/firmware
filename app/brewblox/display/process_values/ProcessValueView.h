/*
 * Copyright 2018 BrewBlox / BrewPi B.V.
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

#pragma once

#include "ProcessValue.h"
#include "Temperature.h"
#include "cbox/CboxPtr.h"
#include "process_value_screen.h"
#include "process_value_widget.h"
#include <vector>

class ProcessValueView {
    const D4D_OBJECT* pObject;
    cbox::CboxPtr<ProcessValue<temp_t>> pvLookup;

public:
    ProcessValueView(const D4D_OBJECT* obj)
        : pObject(obj)
    {
    }

    void setId(const cbox::obj_id_t& id)
    {
        pvLookup.setId(id);
    }

    cbox::obj_id_t getId()
    {
        pvLookup.getId();
    }

    void update();

    void invalidate()
    {
        D4D_InvalidateObject(pObject, 1);
    }

    void setEnabled(bool enabled)
    {
        D4D_EnableObject(pObject, enabled);
        D4D_EnableObject(D4D_PV_VALUE(pObject), enabled);
        D4D_EnableObject(D4D_PV_SETTING(pObject), enabled);
        D4D_EnableObject(D4D_PV_NAME(pObject), enabled);
    }
};

class ProcessValueScreenMgr {
private:
    std::vector<ProcessValueView> pvViews;
    cbox::ObjectContainer& objects;

public:
    ProcessValueScreenMgr(cbox::ObjectContainer& _objects)
        : objects(_objects)
        , pvViews({
              ProcessValueView(&scrProcessValues00),
              ProcessValueView(&scrProcessValues01),
              ProcessValueView(&scrProcessValues02),
              ProcessValueView(&scrProcessValues10),
              ProcessValueView(&scrProcessValues11),
              ProcessValueView(&scrProcessValues12),
          })
    {
    }

    void update();

    void search();
}
