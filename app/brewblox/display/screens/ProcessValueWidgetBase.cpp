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

#include "ProcessValueWidgetBase.h"
#include "BrewBlox.h"
#include "ProcessValue.h"
#include "Temperature.h"
#include "d4d.hpp"

ProcessValueWidgetBase::ProcessValueWidgetBase(WidgetWrapper& myWrapper)
    : WidgetBase(myWrapper)
    , lbl_relations{wrapper.pObj(), nullptr}
    , value{
          {0, wrapper.cy / 2 - 10}, // D4D_POINT position
          {wrapper.cx, 21},         // D4D_SIZE                              size;                 ///< Size of the object.
          0,                        // D4D_COOR                              radius;               ///< Object corners radius.
          nullptr,                  // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
          &value_lbl,               // void*                                 pParam;               ///< The object depends parameters.
          &d4d_labelSysFunc,        // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
          nullptr,                  // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
          nullptr,                  // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
          lbl_relations,            // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
          0,                        ///< The initializations object flags.
          wrapper.scheme,           ///< Pointer on used color scheme.
          &valueData,               ///< Pointer on runtime object data.
      }
    , setting{
          {0, 12},           // D4D_POINT position
          {wrapper.cx, 15},  // D4D_SIZE                              size;                 ///< Size of the object.
          0,                 // D4D_COOR                              radius;               ///< Object corners radius.
          nullptr,           // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
          &setting_lbl,      // void*                                 pParam;               ///< The object depends parameters.
          &d4d_labelSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
          nullptr,           // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
          nullptr,           // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
          lbl_relations,     // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
          0,                 ///< The initializations object flags.
          wrapper.scheme,    ///< Pointer on used color scheme.
          &settingData,      ///< Pointer on runtime object data.
      }
    , icons{
          {0, wrapper.cy - 31}, // D4D_POINT position
          {wrapper.cx, 15},     // D4D_SIZE                              size;                 ///< Size of the object.
          0,                    // D4D_COOR                              radius;               ///< Object corners radius.
          nullptr,              // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
          &icons_lbl,           // void*                                 pParam;               ///< The object depends parameters.
          &d4d_labelSysFunc,    // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
          nullptr,              // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
          nullptr,              // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
          lbl_relations,        // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
          0,                    ///< The initializations object flags.
          wrapper.scheme,       ///< Pointer on used color scheme.
          &iconsData,           ///< Pointer on runtime object data.
      }
{
    wrapper.addChildren({&value, &setting, &icons});
    wrapper.setEnabled(D4D_FALSE); // start widget disabled
}
