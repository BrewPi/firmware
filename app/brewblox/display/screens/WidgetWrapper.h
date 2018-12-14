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
    static const D4D_OBJECT_FLAGS initFlags = (D4D_OBJECT_F_VISIBLE | /*D4D_OBJECT_F_ENABLED | */ D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH);
    D4D_OBJECT_DATA widgetObjectData = {((initFlags | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};

    void* clickHandlerObject = nullptr;
    void (*clickHandlerFunction)(void*) = nullptr;

    static void buttonClickHandler(D4D_OBJECT* btnObj)
    {
        auto thisPtr = reinterpret_cast<WidgetWrapper*>(btnObj->userData);
        if (thisPtr && thisPtr->clickHandlerFunction) {
            thisPtr->clickHandlerFunction(thisPtr->clickHandlerObject);
        }
    }

    D4D_GROUP_BOX widget_params = {
        {nullptr, 0, 0, 0, 0, 0}, // group box is only used to pain the background
    };

    char widget_name[15];
    D4D_BUTTON_STATUS btn_status = {0};
    D4D_STR_PROPERTIES btn_strPrties = {D4D_LBL_FNT_PRTY_DEFAULT, (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_BOTTOM_MASK)};
    D4D_OBJECT_DATA btnData = {((D4D_OBJECT_F_VISIBLE | /* D4D_OBJECT_F_ENABLED | */ D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL};
    D4D_OBJECT* btnRelations[2] = {&widgetObject, nullptr};
    D4D_BUTTON btnParams = {
        {widget_name, 15, FONT_REGULAR, &btn_strPrties, 15, 0},
        nullptr,           /* pBmpNormal */
        nullptr,           /* pBmpFocus */
        &btn_status,       /* Status clear */
        buttonClickHandler /* Click event */
    };

    D4D_OBJECT btnObject = {
        {0, 0},          // D4D_POINT position
        {cx, 94},        // D4D_SIZE                              size;                 ///< Size of the object.
        0,               // D4D_COOR                              radius;               ///< Object corners radius.
        nullptr,         // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
        &btnParams,      // void*                                 pParam;               ///< The object depends parameters.
        &d4d_btnSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
        nullptr,         // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
        this,            // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
        btnRelations,    // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
        initFlags,       ///< The initializations object flags.
        scheme,          ///< Pointer on used color scheme.
        &btnData,        ///< Pointer on runtime object data.
    };

    const D4D_OBJECT_SYS_FUNCTION widgetSysFunc = {
        D4D_DEFSTR("Widget"),
        nullptr,
        nullptr,
        nullptr,
    };

    std::vector<D4D_OBJECT*> objects;
    D4D_OBJECT widgetObject;

public:
    WidgetWrapper(uint8_t pos)
        : x(5 + 105 * (pos % 3))
        , y((pos <= 2) ? 20 : 123)
        , colorScheme(makeColorScheme(30, 50, 100))
        , objects{nullptr, &btnObject, nullptr}
        , widgetObject{
              {x, y},               // D4D_POINT position
              {cx, cy},             // D4D_SIZE                              size;                 ///< Size of the object.
              0,                    // D4D_COOR                              radius;               ///< Object corners radius.
              nullptr,              // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
              &widget_params,       // void*                                 pParam;               ///< The object depends parameters.
              &d4d_groupBoxSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
              nullptr,              // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
              this,                 // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
              objects.data(),       // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
              initFlags,            ///< The initializations object flags.
              scheme,               ///< Pointer on used color scheme.
              &widgetObjectData,    ///< Pointer on runtime object data.
          }
    {
        resetName();
    }
    virtual ~WidgetWrapper() = default;

    D4D_OBJECT*
    pObj()
    {
        return &widgetObject;
    }

    void setName(char* newName)
    {
        D4D_SetText(&btnObject, newName);
    }

    void resetName()
    {
        D4D_SetText(&btnObject, "unassigned");
    }

    void setClickHandler(void* obj, void (*func)(void*))
    {
        clickHandlerFunction = func;
        clickHandlerObject = obj;
    }

    void resetClickHandler()
    {
        clickHandlerFunction = nullptr;
        clickHandlerObject = nullptr;
    }

    void
    setChildren(std::vector<D4D_OBJECT*> children)
    {
        objects.resize(2); // keep parent and button
        objects.insert(objects.end(), children.cbegin(), children.cend());
        for (auto it = objects.begin() + 2; it != objects.end(); ++it) {
            auto pChild = *it;
            pChild->pData->pScreen = widgetObjectData.pScreen; // ensure children are owned by the correct screen
        }
        objects.push_back(nullptr);
        widgetObject.pRelations = objects.data();
    }

    void
    resetChildren()
    {
        objects.resize(2); // keep parent and button
        objects.push_back(nullptr);
        widgetObject.pRelations = objects.data();
    }

    void
    invalidate()
    {
        D4D_InvalidateObject(&widgetObject, D4D_TRUE);
    }

    void
    setEnabled(bool enabled)
    {
        D4D_EnableObject(pObj(), enabled);
    }

    static constexpr SmallColorScheme
    makeColorScheme(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        auto r_lighter = uint8_t(std::min(uint16_t(r) * 3 / 2, 255));
        auto g_lighter = uint8_t(std::min(uint16_t(g) * 3 / 2, 255));
        auto b_lighter = uint8_t(std::min(uint16_t(b) * 3 / 2, 255));

        SmallColorScheme scheme = {
            D4D_COLOR_RGB(r, g, b),                         ///< The object background color in standard state
            D4D_COLOR_RGB(24, 24, 24),                      ///< The object background color in disabled state
            D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in focused state
            D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in captured state
            D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in standard state
            D4D_COLOR_RGB(48, 48, 48),                      ///< The object fore color in disabled state
            D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in focused state
            D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in captured state
        };
        return scheme;
    }
};