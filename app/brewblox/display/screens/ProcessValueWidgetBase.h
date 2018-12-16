/*
 * Copyright 2018 BrewPi B.V.
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
#include "ProcessValue.h"
#include "Temperature.h"
#include "WidgetBase.h"
#include "cbox/CboxPtr.h"
#include "d4d.hpp"

class ProcessValueWidgetBase : public WidgetBase {
protected:
    char value_buf[12] = {0};
    char setting_buf[12] = {0};

    const D4D_OBJECT* lbl_relations[2];

    D4D_STR_PROPERTIES value_strPrties = {D4D_LBL_FNT_PRTY_DEFAULT, (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_BOTTOM_MASK)};
    D4D_STR_PROPERTIES setting_strPrties = {D4D_LBL_FNT_PRTY_DEFAULT, (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)};

    D4D_LABEL value_lbl = {{value_buf, D4D_TEXT_LEN(value_buf), FONT_NUMBER_LARGE, &value_strPrties, 12, 0}};
    D4D_LABEL setting_lbl = {{setting_buf, D4D_TEXT_LEN(setting_buf), FONT_NUMBER_MEDIUM, &setting_strPrties, 12, 0}};

    D4D_OBJECT_DATA valueData = {((D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};
    D4D_OBJECT_DATA settingData = {((D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};

    D4D_OBJECT value;
    D4D_OBJECT setting;

public:
    ProcessValueWidgetBase(WidgetWrapper& myWrapper);

    void
    setValue(const char* buf, bool enabled)
    {
        D4D_SetText(&value, buf);
        D4D_EnableObject(&value, true);
    }

    void
    setSetting(const char* buf, bool enabled)
    {
        D4D_SetText(&setting, buf);
        D4D_EnableObject(&setting, true);
    }

    virtual ~ProcessValueWidgetBase() = default;
};