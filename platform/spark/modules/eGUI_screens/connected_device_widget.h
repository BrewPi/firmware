/* 
 * File:   connected_device_widget.h
 * Author: mat
 *
 * Created on 25 February 2015, 21:19
 */

#ifndef CONNECTED_DEVICE_WIDGET_H
#define	CONNECTED_DEVICE_WIDGET_H

#ifdef	__cplusplus
extern "C" {
#endif

#undef     D4D_BTN_F_DEFAULT
#define D4D_BTN_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_BTN_F_CONT_RECT)
    
#include "d4d.h"    
    
void D4D_CDV_SetValueText(const D4D_OBJECT* pObj, const char* text);
void D4D_CDV_SetConnectionText(const D4D_OBJECT* pObj, const char* text);
    
    
/*! @brief This is connected device init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_CDV_TXT_PRTY_DEFAULT
  #define D4D_CDV_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

#define D4D_CDV_VALUE(pObj)        ((const D4D_OBJECT*)((pObj)->pRelations[1]))
#define D4D_CDV_CONNECTION(pObj)          ((const D4D_OBJECT*)((pObj)->pRelations[2]))


#define D4D_DECLARE_CDV(type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_EXTERN_OBJECT(name); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_value_relations, &name, NULL); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_connection_relations, &name, NULL); \
    char name##_value_buf[10]; \
    char name##_connection_buf[17]; \
    _D4D_DECLARE_LABEL(D4D_CONST, name##_value, name##_value_buf, 0, 0, cx/3, (cy), 0, NULL, name##_value_relations, D4D_LBL_F_DEFAULT, NULL, font_value, NULL, NULL); \
    _D4D_DECLARE_LABEL(D4D_CONST, name##_connection, name##_connection_buf, cx/3, 0, cx*2/3, (cy), 0, NULL, name##_connection_relations, D4D_LBL_F_DEFAULT, NULL, font_connection, NULL, NULL); \
    D4D_DECLARE_OBJECT_RELATIONS(name##_btn_relations, NULL, &name##_value, &name##_connection); \
    _D4D_DECLARE_BUTTON(D4D_CONST, name, NULL, x, y, cx, cy, 0, NULL, name##_btn_relations, D4D_BTN_F_DEFAULT, NULL, NULL, NULL, FONT_ARIAL7, NULL, NULL, NULL);

#define D4D_DECLARE_CDV_INDEX(idx, count, type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_DECLARE_CDV(type, name##idx, x, y+(idx*(cy/count)), cx, (cy/count)-4, font_value, font_connection)

#define D4D_DECLARE_CDV_LIST(count, type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_DECLARE_CDV_INDEX(0, count, type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_DECLARE_CDV_INDEX(1, count, type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_DECLARE_CDV_INDEX(2, count, type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_DECLARE_CDV_INDEX(3, count, type, name, x, y, cx, cy, font_value, font_connection) \
    D4D_DECLARE_CDV_INDEX(4, count, type, name, x, y, cx, cy, font_value, font_connection) 
    



#ifdef	__cplusplus
}
#endif

#endif	/* CONNECTED_DEVICE_WIDGET_H */

