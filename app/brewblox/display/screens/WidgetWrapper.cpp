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

#include "WidgetWrapper.h"
#include "SmallColorScheme.h"
#include "cbox/CboxPtr.h"
#include "screen.h"
#include <algorithm>
#include <vector>

extern "C" {
#include "common_files/d4d_private.h" // include to recursively set screen pointer in new children
}

WidgetWrapper::WidgetWrapper(uint8_t pos)
    : x(5 + 105 * (pos % 3))
    , y((pos <= 2) ? 20 : 123)
    , colorScheme(makeSmallColorScheme(30, 50, 100))
    , objects{nullptr, &btnObject, nullptr}
    , wrapperObject{
          {x, y},               // D4D_POINT position
          {cx, cy},             // D4D_SIZE                              size;                 ///< Size of the object.
          0,                    // D4D_COOR                              radius;               ///< Object corners radius.
          nullptr,              // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
          &widget_params,       // void*                                 pParam;               ///< The object depends parameters.
          &d4d_groupBoxSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
          nullptr,              // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
          this,                 // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
          objects.data(),       // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
          0,                    ///< The initializations object flags.
          scheme,               ///< Pointer on used color scheme.
          &wrapperObjectData,   ///< Pointer on runtime object data.
      }
{
    resetName();
}

void
WidgetWrapper::setName(char* newName)
{
    D4D_SetText(&btnObject, newName);
}

void
WidgetWrapper::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    colorScheme = makeSmallColorScheme(r, g, b);
}

void
WidgetWrapper::resetName()
{
    D4D_SetText(&btnObject, "unassigned");
}

void
WidgetWrapper::setClickHandler(void* obj, void (*func)(void*))
{
    clickHandlerFunction = func;
    clickHandlerObject = obj;
}

void
WidgetWrapper::resetClickHandler()
{
    clickHandlerFunction = nullptr;
    clickHandlerObject = nullptr;
}

void
WidgetWrapper::setChildren(std::vector<D4D_OBJECT*> children)
{
    objects.resize(2); // keep parent and button
    objects.insert(objects.end(), children.cbegin(), children.cend());
    objects.push_back(nullptr);
    wrapperObject.pRelations = objects.data();
    // ensure new children and their children have the correct screen pointer
    D4D_SetObjectScreenPointer(&wrapperObject, wrapperObject.pData->pScreen);
}

void
WidgetWrapper::resetChildren()
{
    objects.resize(2); // keep parent and button
    objects.push_back(nullptr);
    wrapperObject.pRelations = objects.data();
}

void
WidgetWrapper::invalidate()
{
    D4D_InvalidateObject(&wrapperObject, D4D_TRUE);
}

void
WidgetWrapper::setEnabled(bool enabled)
{
    D4D_EnableObject(&wrapperObject, enabled);
    D4D_EnableObject(&btnObject, enabled);
}
