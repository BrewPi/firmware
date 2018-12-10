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
#include "../screen.h"
#include "../widget_color_scheme.h"
#include "BrewBlox.h"
#include "ProcessValue.h"
#include "Temperature.h"
#include "cbox/CboxPtr.h"
#include <algorithm>

#define INACTIVE_BG_COLOR D4D_COLOR_RGB(24, 24, 24)
#define ACTIVE_BG_COLOR D4D_COLOR_RGB(140, 0, 25)
#define INACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(64, 64, 64)
#define ACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(160, 160, 160)
#define INACTIVE_FS_COLOR D4D_COLOR_RGB(128, 128, 128)
#define ACTIVE_FG_COLOR D4D_COLOR_RGB(255, 255, 255)

constexpr auto
pvColorScheme(const uint8_t r, const uint8_t g, const uint8_t b)
{
    auto r_lighter = uint8_t(std::min(uint16_t(r) * 3 / 2, 255));
    auto g_lighter = uint8_t(std::min(uint16_t(g) * 3 / 2, 255));
    auto b_lighter = uint8_t(std::min(uint16_t(b) * 3 / 2, 255));

    WIDGET_COLOR_SCHEME scheme = {
        D4D_COLOR_RGB(r, g, b),                         ///< The object background color in standard state
        D4D_COLOR_RGB(24, 24, 24),                      ///< The object background color in disabled state
        D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in focused state
        D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in captured state
        D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in standard state
        D4D_COLOR_RGB(128, 128, 128),                   ///< The object fore color in disabled state
        D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in focused state
        D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in captured state
    };
    return scheme;
}

void
D4D_PV_SetValueText(const D4D_OBJECT* pObj, const char* text);
void
D4D_PV_SetSettingText(const D4D_OBJECT* pObj, const char* text);
void
D4D_PV_SetNameText(const D4D_OBJECT* pObj, const char* text);

/*! @brief This is process value init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/

#define D4D_PV_TXT_PRTY_DEFAULT (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)

#define D4D_PV_VALUE(pObj) ((const D4D_OBJECT*)((pObj)->pRelations[1]))
#define D4D_PV_SETTING(pObj) ((const D4D_OBJECT*)((pObj)->pRelations[2]))
#define D4D_PV_NAME(pObj) ((const D4D_OBJECT*)((pObj)->pRelations[3]))

#define D4D_DECLARE_PV(ref, x, y, cx, cy)                                                                                                                                                                                                 \
    D4D_EXTERN_OBJECT(ref);                                                                                                                                                                                                               \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_value_relations, &ref, nullptr);                                                                                                                                                                   \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_setting_relations, &ref, nullptr);                                                                                                                                                                 \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_name_relations, &ref, nullptr);                                                                                                                                                                    \
    char ref##_value_buf[12];                                                                                                                                                                                                             \
    char ref##_setting_buf[12];                                                                                                                                                                                                           \
    char ref##_name_buf[12];                                                                                                                                                                                                              \
    auto ref##_scheme = pvColorScheme(30, 50, 100);                                                                                                                                                                                       \
    _D4D_DECLARE_LABEL(D4D_CONST, ref##_value, ref##_value_buf, 0, 0, cx, (((cy)*2) / 3), 0, nullptr, ref##_value_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&ref##_scheme), FONT_NUMBER_LARGE, nullptr, nullptr);                 \
    _D4D_DECLARE_LABEL(D4D_CONST, ref##_setting, ref##_setting_buf, 0, (((cy)*2) / 3), cx, ((cy) / 3), 0, nullptr, ref##_setting_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&ref##_scheme), FONT_NUMBER_MEDIUM, nullptr, nullptr); \
    _D4D_DECLARE_LABEL(D4D_CONST, ref##_name, ref##_name_buf, 0, (((cy)*2) / 3), cx, ((cy) / 3), 0, nullptr, ref##_name_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&ref##_scheme), FONT_REGULAR, nullptr, nullptr);                \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_btn_relations, nullptr, &ref##_value, &ref##_setting, &ref##_name);                                                                                                                                \
    _D4D_DECLARE_BUTTON(D4D_CONST, ref, nullptr, x, y, cx, cy, 0, nullptr, ref##_btn_relations, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH), nullptr, nullptr, AS_D4D_COLOR_SCHEME(&ref##_scheme), FONT_REGULAR, nullptr, nullptr, nullptr);

class ProcessValueWidget {
private:
    const D4D_OBJECT* pObject;
    cbox::CboxPtr<ProcessValue<temp_t>> pvLookup;

public:
    ProcessValueWidget(const D4D_OBJECT* obj)
        : pObject(obj)
        , pvLookup(brewbloxBox().makeCboxPtr<ProcessValue<temp_t>>())
    {
    }

    void setId(const cbox::obj_id_t& id)
    {
        pvLookup.setId(id);
    }

    cbox::obj_id_t getId()
    {
        return pvLookup.getId();
    }

    void update();

    void invalidate()
    {
        D4D_InvalidateObject(pObject, D4D_TRUE);
    }

    void setEnabled(bool enabled);
};