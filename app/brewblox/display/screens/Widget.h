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
#include "SmallColorScheme.h"
#include "cbox/CboxPtr.h"
#include "screen.h"
#include <algorithm>
#include <vector>

class WidgetWrapper {
public:
    const D4D_COOR x;
    const D4D_COOR y;
    static const D4D_COOR cx = 100;
    static const D4D_COOR cy = 98;
    SmallColorScheme colorScheme;
    D4D_CLR_SCHEME* scheme = AS_D4D_COLOR_SCHEME(&colorScheme);
    static const D4D_OBJECT_FLAGS initFlags = (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH);
    D4D_OBJECT_DATA widgetObjectData = {((initFlags | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};

    D4D_STR_PROPERTIES lbl_strPrties = {D4D_LBL_FNT_PRTY_DEFAULT, D4D_LBL_TXT_PRTY_DEFAULT};
    D4D_LABEL placeholder_lbl = {{"unassigned", 11, FONT_REGULAR, &lbl_strPrties, 11, 0}};
    const D4D_OBJECT* placeholder_relations[2] = {pObj(), nullptr};
    D4D_OBJECT_DATA placeholderData = {((D4D_OBJECT_F_VISIBLE | /* D4D_OBJECT_F_ENABLED | */ D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};
    D4D_OBJECT placeholder = {
        {0, 0},                // D4D_POINT position
        {cx, cy},              // D4D_SIZE                              size;                 ///< Size of the object.
        0,                     // D4D_COOR                              radius;               ///< Object corners radius.
        nullptr,               // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
        &placeholder_lbl,      // void*                                 pParam;               ///< The object depends parameters.
        &d4d_labelSysFunc,     // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
        nullptr,               // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
        nullptr,               // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
        placeholder_relations, // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
        initFlags,             ///< The initializations object flags.
        scheme,                ///< Pointer on used color scheme.
        &placeholderData,      ///< Pointer on runtime object data.
    };

    std::vector<D4D_OBJECT*> objects;
    D4D_OBJECT widgetObject;

public:
    static Byte onUsrMessage(D4D_MESSAGE_S* pMsg)
    {
        /*if (pMsg->nMsgId == D4D_MSG_TOUCHED) {
            reinterpret_cast<WidgetW*>(pMsg->pObject->userData)->onClick();
        }*/
        return 1;
    }

    virtual void onClick(){

    };

    WidgetWrapper(uint8_t pos)
        : x(5 + 105 * (pos % 3))
        , y((pos <= 2) ? 20 : 123)
        , colorScheme(makeColorScheme(30, 50, 100))
        , objects{nullptr, &placeholder, nullptr}
        , widgetObject{
              {x, y},            // D4D_POINT position
              {cx, cy},          // D4D_SIZE                              size;                 ///< Size of the object.
              0,                 // D4D_COOR                              radius;               ///< Object corners radius.
              nullptr,           // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
              nullptr,           // void*                                 pParam;               ///< The object depends parameters.
              nullptr,           // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
              onUsrMessage,      // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
              this,              // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
              objects.data(),    // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
              initFlags,         ///< The initializations object flags.
              scheme,            ///< Pointer on used color scheme.
              &widgetObjectData, ///< Pointer on runtime object data.
          }
    {
    }
    virtual ~WidgetWrapper() = default;

    D4D_OBJECT* pObj()
    {
        return &widgetObject;
    }

    void setChildren(std::vector<D4D_OBJECT*> children)
    {
        objects.resize(1); // only keep parent
        if (children.size() == 0) {
            objects.push_back(&placeholder);
        } else {
            objects.insert(objects.end(), children.cbegin(), children.cend());
        }
        objects.push_back(nullptr);
        widgetObject.pRelations = objects.data();
    }

    void invalidate()
    {
        D4D_InvalidateObject(&widgetObject, D4D_TRUE);
    }

    void setEnabled(bool enabled)
    {
        D4D_EnableObject(pObj(), enabled);
    }

    static constexpr SmallColorScheme makeColorScheme(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        auto r_lighter = uint8_t(std::min(uint16_t(r) * 3 / 2, 255));
        auto g_lighter = uint8_t(std::min(uint16_t(g) * 3 / 2, 255));
        auto b_lighter = uint8_t(std::min(uint16_t(b) * 3 / 2, 255));

        SmallColorScheme scheme = {
            D4D_COLOR_RGB(r, g, b),                         ///< The object background color in standard state
            D4D_COLOR_RGB(40, 40, 40),                      ///< The object background color in disabled state
            D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in focused state
            D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in captured state
            D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in standard state
            D4D_COLOR_RGB(128, 128, 128),                   ///< The object fore color in disabled state
            D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in focused state
            D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in captured state
        };
        return scheme;
    }
};