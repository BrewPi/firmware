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
#include "../fonts/fonts.h"
#include "SmallColorScheme.h"
#include "cbox/CboxPtr.h"
#include "d4d.hpp"
#include <vector>

class WidgetWrapper {
public:
    const D4D_COOR x;
    const D4D_COOR y;
    static const D4D_COOR cx = 100;
    static const D4D_COOR cy = 98;
    SmallColorScheme colorScheme;
    D4D_CLR_SCHEME* scheme = AS_D4D_COLOR_SCHEME(&colorScheme);
    D4D_OBJECT_DATA wrapperObjectData = {(D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_NOTINIT), NULL};

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
    D4D_OBJECT_DATA btnData = {(D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH | D4D_OBJECT_F_NOTINIT), NULL};
    D4D_OBJECT* btnRelations[2] = {&wrapperObject, nullptr};
    D4D_BUTTON btnParams = {
        {widget_name, 15, FONT_REGULAR, &btn_strPrties, 15, 0},
        nullptr,           /* pBmpNormal */
        nullptr,           /* pBmpFocus */
        &btn_status,       /* Status clear */
        buttonClickHandler /* Click event */
    };

    // rendered as child of widget object
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
        0,               ///< The initializations object flags.
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
    D4D_OBJECT wrapperObject;

public:
    WidgetWrapper(uint8_t pos);
    virtual ~WidgetWrapper() = default;

    D4D_OBJECT* pObj()
    {
        return &wrapperObject;
    }

    void setName(char* newName);
    void resetName();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setClickHandler(void* obj, void (*func)(void*));
    void resetClickHandler();
    void addChildren(std::vector<D4D_OBJECT*> children);
    void resetChildren();
    void invalidate();
    void setEnabled(bool enabled);
};