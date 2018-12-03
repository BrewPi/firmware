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

#ifndef CONNECTED_DEVICE_WIDGET_H
#define CONNECTED_DEVICE_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../fonts.h"
#include "../widget_color_scheme.h"
#include "d4d.h"

#define INACTIVE_BG_COLOR D4D_COLOR_RGB(24, 24, 24)
#define ACTIVE_BG_COLOR D4D_COLOR_RGB(140, 0, 25)
#define INACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(64, 64, 64)
#define ACTIVE_FG_LOW_COLOR D4D_COLOR_RGB(160, 160, 160)
#define INACTIVE_FS_COLOR D4D_COLOR_RGB(128, 128, 128)
#define ACTIVE_FG_COLOR D4D_COLOR_RGB(255, 255, 255)

const WIDGET_COLOR_SCHEME color_scheme_pv = {
    ACTIVE_BG_COLOR,       // bkg active
    INACTIVE_BG_COLOR,     // bkg disable
    ACTIVE_BG_COLOR,       // focus
    ACTIVE_BG_COLOR,       // capture
    ACTIVE_FG_LOW_COLOR,   // fg
    INACTIVE_FG_LOW_COLOR, // fg disabled
    ACTIVE_FG_LOW_COLOR,   // fg focus
    ACTIVE_FG_LOW_COLOR,   // fg capture
};

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

#define D4D_DECLARE_PV(ref, x, y, cx, cy)                                                                                                                                                                                    \
    D4D_EXTERN_OBJECT(ref);                                                                                                                                                                                                  \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_value_relations, &ref, NULL);                                                                                                                                                         \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_setting_relations, &ref, NULL);                                                                                                                                                       \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_name_relations, &ref, NULL);                                                                                                                                                          \
    char ref##_value_buf[12];                                                                                                                                                                                                \
    char ref##_setting_buf[12];                                                                                                                                                                                              \
    char ref##_name_buf[12];                                                                                                                                                                                                 \
    _D4D_DECLARE_LABEL(D4D_CONST, ref##_value, ref##_value_buf, 0, 0, cx, (((cy)*2) / 3), 0, NULL, ref##_value_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&color_scheme_pv), FONT_ARIAL7, NULL, NULL);                \
    _D4D_DECLARE_LABEL(D4D_CONST, ref##_setting, ref##_setting_buf, 0, (((cy)*2) / 3), cx, ((cy) / 3), 0, NULL, ref##_setting_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&color_scheme_pv), FONT_ARIAL7, NULL, NULL); \
    _D4D_DECLARE_LABEL(D4D_CONST, ref##_name, ref##_name_buf, 0, (((cy)*2) / 3), cx, ((cy) / 3), 0, NULL, ref##_name_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(&color_scheme_pv), FONT_ARIAL7, NULL, NULL);          \
    D4D_DECLARE_OBJECT_RELATIONS(ref##_btn_relations, NULL, &ref##_value, &ref##_setting, &ref##_name);                                                                                                                      \
    _D4D_DECLARE_BUTTON(D4D_CONST, ref, NULL, x, y, cx, cy, 0, NULL, ref##_btn_relations, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH), NULL, NULL, AS_D4D_COLOR_SCHEME(&color_scheme_pv), FONT_ARIAL7, NULL, NULL, NULL);

#ifdef __cplusplus
}
#endif

#endif /* CONNECTED_DEVICE_WIDGET_H */
