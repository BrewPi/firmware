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

#include "ProcessValueWidget.h"
#include "BrewBlox.h"
#include "ProcessValue.h"
#include "Temperature.h"
#include "d4d.hpp"

ProcessValueWidget::ProcessValueWidget(WidgetWrapper& myWrapper, const cbox::obj_id_t& id)
    : WidgetBase(myWrapper)
    , lbl_relations{wrapper.pObj(), nullptr}
    , value{
          {0, 15},           // D4D_POINT position
          {wrapper.cx, 22},  // D4D_SIZE                              size;                 ///< Size of the object.
          0,                 // D4D_COOR                              radius;               ///< Object corners radius.
          nullptr,           // D4D_MARGIN*                           pMargin;              ///< Object inner margin.
          &value_lbl,        // void*                                 pParam;               ///< The object depends parameters.
          &d4d_labelSysFunc, // D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
          nullptr,           // D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
          nullptr,           // D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
          lbl_relations,     // D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
          0,                 ///< The initializations object flags.
          wrapper.scheme,    ///< Pointer on used color scheme.
          &valueData,        ///< Pointer on runtime object data.
      }
    , setting{
          {0, 55},           // D4D_POINT position
          {wrapper.cx, 16},  // D4D_SIZE                              size;                 ///< Size of the object.
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
    , pvLookup(brewbloxBox().makeCboxPtr<ProcessValue<temp_t>>(id))
{
    wrapper.setChildren({&value, &setting});
    wrapper.setClickHandler(this, onClickStatic);
    wrapper.setEnabled(D4D_FALSE); // start widget disabled
}

void
to_chars(const temp_t& t, char* buf, uint8_t len, uint8_t decimals)
{
    auto digits = decimals + 2;
    auto rounder = temp_t(0.5);

    for (; decimals > 0; decimals--) {
        rounder = rounder / 10;
    }

    temp_t rounded;
    if (t >= temp_t(0)) {
        rounded = t + rounder;
        auto temporary = rounded;
        while (temporary >= temp_t(10)) {
            temporary = temporary / 10;
            ++digits;
        }
    } else {
        rounded = t - rounder;
        auto temporary = rounded;
        while (temporary <= temp_t(-10)) {
            temporary = temporary / 10;
            ++digits;
        }
        digits += 1; // for minus sign
    }

    std::memset(buf, 0, len);
    if (digits > len) {
        digits = len;
    }

    using reduced_precision_t = safe_elastic_fixed_point<11, 10, int32_t>;
    cnl::to_chars(buf, &buf[digits], reduced_precision_t(rounded));
}

void
ProcessValueWidget::update()
{
    if (auto pv = pvLookup.const_lock()) {
        char buf[12];
        to_chars(pv->value(), buf, 12, 1);
        D4D_SetText(&value, buf);
        D4D_EnableObject(&value, true);
        to_chars(pv->setting(), buf, 12, 1);
        D4D_SetText(&setting, buf);
        D4D_EnableObject(&setting, true);
        wrapper.setEnabled(true);
        return;
    }
    D4D_EnableObject(&value, false);
    D4D_EnableObject(&setting, false);
    wrapper.setEnabled(false);
}
