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

#pragma once
#include "../Widget.h"
#include "../screen.h"
#include "BrewBlox.h"
#include "ProcessValue.h"
#include "Temperature.h"
#include "cbox/CboxPtr.h"
#include <algorithm>

class ProcessValueWidget : public Widget {
private:
    cbox::CboxPtr<ProcessValue<temp_t>> pvLookup;

    char value_buf[12];
    char setting_buf[12];
    char name_buf[16];

    const D4D_OBJECT* lbl_relations[2] = {&widgetObject, nullptr};

    D4D_STR_PROPERTIES lbl_strPrties = {D4D_LBL_FNT_PRTY_DEFAULT, D4D_LBL_TXT_PRTY_DEFAULT};
    D4D_LABEL value_lbl = {value_buf, D4D_TEXT_LEN(value_buf), FONT_NUMBER_LARGE, &lbl_strPrties, 12, 0};
    D4D_LABEL setting_lbl = {setting_buf, D4D_TEXT_LEN(setting_buf), FONT_NUMBER_MEDIUM, &lbl_strPrties, 12, 0};
    D4D_LABEL name_lbl = {name_buf, D4D_TEXT_LEN(name_buf), FONT_REGULAR, &lbl_strPrties, 16, 0};

    D4D_OBJECT_DATA valueData = {((D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};
    D4D_OBJECT_DATA settingData = {((D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};
    D4D_OBJECT_DATA nameData = {((D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};

    D4D_OBJECT value = {
        {0, cy / 7},       // D4D_POINT position
        {cx, 25},          // D4D_SIZE                              size;                 ///< Size of the object.
        0,                 // D4D_COOR                              radius;               ///< Object corners radius.
        nullptr,           // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
        &value_lbl,        // void*                                 pParam;               ///< The object depends parameters.
        &d4d_labelSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
        nullptr,           // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
        nullptr,           // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
        lbl_relations,     // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
        initFlags,         ///< The initializations object flags.
        scheme,            ///< Pointer on used color scheme.
        &valueData,        ///< Pointer on runtime object data.
    };

    D4D_OBJECT setting = {
        {0, cy / 2},       // D4D_POINT position
        {cx, 20},          // D4D_SIZE                              size;                 ///< Size of the object.
        0,                 // D4D_COOR                              radius;               ///< Object corners radius.
        nullptr,           // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
        &name_lbl,         // void*                                 pParam;               ///< The object depends parameters.
        &d4d_labelSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
        nullptr,           // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
        nullptr,           // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
        lbl_relations,     // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
        initFlags,         ///< The initializations object flags.
        scheme,            ///< Pointer on used color scheme.
        &settingData,      ///< Pointer on runtime object data.
    };

    D4D_OBJECT name
        = {
            {0, cy - 20},      // D4D_POINT position
            {cx, 20},          // D4D_SIZE                              size;                 ///< Size of the object.
            0,                 // D4D_COOR                              radius;               ///< Object corners radius.
            nullptr,           // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
            &name_lbl,         // void*                                 pParam;               ///< The object depends parameters.
            &d4d_labelSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
            nullptr,           // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
            nullptr,           // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
            lbl_relations,     // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
            initFlags,         ///< The initializations object flags.
            scheme,            ///< Pointer on used color scheme.
            &nameData,         ///< Pointer on runtime object data.
        };

    D4D_OBJECT* relations[5] = {&widgetObject, &value, &setting, &name, nullptr};

public:
    ProcessValueWidget(uint8_t pos, const cbox::obj_id_t& id)
        : Widget(pos, relations)
        , pvLookup(brewbloxBox().makeCboxPtr<ProcessValue<temp_t>>(id))
    {
    }
    virtual ~ProcessValueWidget() = default;

    void setId(const cbox::obj_id_t& id)
    {
        pvLookup.setId(id);
    }

    cbox::obj_id_t getId()
    {
        return pvLookup.getId();
    }

    virtual void update() override final;

    virtual void onClick() override final
    {
    }

    void setEnabled(bool enabled);
};