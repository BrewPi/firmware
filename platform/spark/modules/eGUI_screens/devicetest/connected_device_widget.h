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
#define	CONNECTED_DEVICE_WIDGET_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "d4d.h"
#include "../widget_color_scheme.h"
    
void D4D_CDV_SetValueText(const D4D_OBJECT* pObj, const char* text);
void D4D_CDV_SetConnectionText(const D4D_OBJECT* pObj, const char* text);
    
    
/*! @brief This is connected device init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/

#define D4D_CDV_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)

#define D4D_CDV_VALUE(pObj)        ((const D4D_OBJECT*)((pObj)->pRelations[1]))
#define D4D_CDV_CONNECTION(pObj)          ((const D4D_OBJECT*)((pObj)->pRelations[2]))



#define D4D_DECLARE_CDV(type, name, x, y, cx, cy, font_value, font_connection, c1, c2) \
    D4D_EXTERN_OBJECT(name); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_value_relations, &name, NULL); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_connection_relations, &name, NULL); \
    char name##_value_buf[10]; \
    char name##_connection_buf[17]; \
    _D4D_DECLARE_LABEL(D4D_CONST, name##_value, name##_value_buf, 0, 0, cx, (((cy)*2)/3), 0, NULL, name##_value_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(c1), font_value, NULL, NULL); \
    _D4D_DECLARE_LABEL(D4D_CONST, name##_connection, name##_connection_buf, 0, (((cy)*2)/3), cx, ((cy)/3), 0, NULL, name##_connection_relations, D4D_LBL_F_DEFAULT, AS_D4D_COLOR_SCHEME(c2), font_connection, NULL, NULL); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_btn_relations, NULL, &name##_value, &name##_connection); \
    _D4D_DECLARE_BUTTON(D4D_CONST, name, NULL, x, y, cx, cy, 0, NULL, name##_btn_relations, (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_TOUCHENABLE | D4D_OBJECT_F_FASTTOUCH ), NULL, NULL, AS_D4D_COLOR_SCHEME(c1), FONT_ARIAL7, NULL, NULL, NULL);

#define D4D_DECLARE_CDV_INDEX(idx, count, type, name, x, y, cx, cy, font_value, font_connection, c1, c2) \
    D4D_DECLARE_CDV(type, name##idx, x, (y+(idx*((cy)/count))), cx, (((cy)/count)-4), font_value, font_connection, c1, c2)

/**
 * c1 - background and temp color scheme
 * c2 - connection text color scheme
 */
#define D4D_DECLARE_CDV_LIST(type, name, x, y, cx, cy, font_value, font_connection, c1, c2) \
    D4D_DECLARE_CDV_INDEX(0, 2, type, name, x, y, cx, cy, font_value, font_connection, c1, c2) \
    D4D_DECLARE_CDV_INDEX(1, 2, type, name, x, y, cx, cy, font_value, font_connection, c1, c2)    
    



#ifdef	__cplusplus
}
#endif

#endif	/* CONNECTED_DEVICE_WIDGET_H */

