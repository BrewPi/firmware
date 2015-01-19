/**************************************************************************
*
* Copyright 2014 by Petr Gargulak. eGUI Community.
* Copyright 2009-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the eGUI project give you
* permission to link the eGUI sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based
* on this library.
* If you modify the eGUI sources, you may extend this exception
* to your version of the eGUI sources, but you are not obligated
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************//*!
*
* @file      d4d_object.h
*
* @author     Petr Gargulak
*
* @version   0.0.20.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver object functions header file
*
*******************************************************************************/

#ifndef __D4D_OBJECT_H
#define __D4D_OBJECT_H

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* D4D OBJECT setting  constants
*
*//*! @addtogroup doxd4d_object_const
* @{
*******************************************************************************/

// Common flags as for initial as for runtime
/*!
  @defgroup doxd4d_object_const_flags D4D OBJECT Defines masks of object behaviour flags
  This section specifies masks of object behaviour flags.
  @ingroup doxd4d_object_const
*/

/**
 * @addtogroup doxd4d_object_const_flags
 * @{
 */

#define D4D_OBJECT_F_VISIBLE        0x0001      ///< Object after initialization is visible on the screen
#define D4D_OBJECT_F_ENABLED        0x0002      ///< Object after initialization is enabled
#define D4D_OBJECT_F_TABSTOP        0x0004      ///< Object can be focused
#define D4D_OBJECT_F_TOUCHENABLE    0x0008      ///< Object has enabled touch screen capability
#define D4D_OBJECT_F_FASTTOUCH      0x0010      ///< Object has enabled fast touch screen capability. This option supports only a some objects (button, check box, etc).
#define D4D_OBJECT_F_FOCUSRECT      0x0020      ///< Object has an outlined rectangle
#define D4D_OBJECT_F_TIMETICK       0x0040      ///< Object will gets the time tick events
#define D4D_OBJECT_F_RESERVED1      0x0080      ///< For future use

#define D4D_OBJECT_F_BEVEL_MASK     0x0300      ///< Object bevel border mask
#define D4D_OBJECT_F_BEVEL_SHIFT    8           ///< Object bevel border bit shift

#define D4D_OBJECT_F_BEVEL_RAISED   0x0100      ///< Object bevel border raised mask
#define D4D_OBJECT_F_BEVEL_DENTED   0x0200      ///< Object bevel border dented mask

#define D4D_OBJECT_F_MOUSE_MASK         0x0C00  ///< Object mouse cursor type mask
#define D4D_OBJECT_F_MOUSE_SHIFT        10      ///< Object mouse cursor type shift

#define D4D_OBJECT_F_MOUSE_NORMAL       0x0400  ///< Object mouse cursor type normal mask
#define D4D_OBJECT_F_MOUSE_BUSY         0x0800  ///< Object mouse cursor type busy mask
#define D4D_OBJECT_F_MOUSE_UNAVAILABLE  0x0C00  ///< Object mouse cursor type unavailable mask

#define D4D_OBJECT_F_SYSTEM_MASK        0x000FFFFF      ///< Object flags system part mask
#define D4D_OBJECT_F_OBJECT_MASK        0xFFF00000      ///< Object flags widget part mask
#define D4D_OBJECT_F_OBJECT_SHIFT       20              ///< Object flags widget part shift

/**@}*/

/*! @brief The relations object index to relation array for parent object. */
#define D4D_OBJECT_USR_DATA_PARENT_IX   0
/*! @brief The relations object index to relation array for first child object. */
#define D4D_OBJECT_USR_DATA_CHILD_IX    1

/*! @} End of doxd4d_object_const                                               */
/******************************************************************************
* Internal constants
******************************************************************************/
// just runtime flags
#define D4D_OBJECT_F_REDRAW             0x10000
#define D4D_OBJECT_F_REDRAWC            0x20000
#define D4D_OBJECT_F_REDRAWSTATE        0x40000
#define D4D_OBJECT_F_NOTINIT            0x80000

struct D4D_OBJECT_S;
struct D4D_SCREEN_S;
struct D4D_MESSAGE_S;
struct D4D_CLR_SCHEME_S;

/******************************************************************************
* D4D Object types
*
*//*! @addtogroup doxd4d_object_type
* @{
*******************************************************************************/

/*! @brief The object flags type. The masks are described here \ref doxd4d_object_const_flags.*/
typedef LWord D4D_OBJECT_FLAGS;

/*! @brief The object init flags type. The masks are described here \ref doxd4d_object_const_flags.*/
typedef LWord D4D_OBJECT_INITFLAGS;

/*! @brief The object system function needed for each object - this is part of D4D_OBJECT main structure.*/
typedef struct D4D_OBJECT_SYS_FUNCTION_S
{
  D4D_TCHAR* strName;                                   ///< Object type (widget) name, it also could be used as a identifier of object type
  void (*OnSysMessage)(struct D4D_MESSAGE_S* pMsg);     ///< Object system message receiver function pointer
  D4D_BOOL (*CheckCoordinates)(struct D4D_OBJECT_S* pObj, D4D_POINT point);     ///< Object check coordinates function pointer (return true or false if the input point fits into the object, is used just for non standard objects)
  struct D4D_STRING_S* (*GetTextBuffer)(struct D4D_OBJECT_S* pObj);             ///< Object get text buffer function pointer (the function returns the pointer of the object text buffer, if exists)
}D4D_OBJECT_SYS_FUNCTION;

/*! @brief The object changeble data structure - this is part of D4D_OBJECT main structure.*/
typedef struct D4D_OBJECT_DATA_S
{
  D4D_OBJECT_FLAGS flags;               ///< runtime object flags
  struct D4D_SCREEN_S* pScreen;         ///< pointer to object screen owner - it is placed in RAM bacause one object could be used in multiply screens
}D4D_OBJECT_DATA, *D4D_OBJECT_DATA_PTR;

/*! @brief The object user data type definition. */
typedef void* D4D_OBJECT_USR_DATA;

/*! @brief The object relations  type definition. In fact this is array of object pointer , where on position 0 is parent object(if exists) and on other positions are children.*/
typedef const struct D4D_OBJECT_S* const * D4D_OBJECT_RELATIONS;

/**************************************************************************/ /*!
* @brief   Type definition of object on User message handler function
* @param   pMsg - pointer to message structure
* @return  none
* @note    This is button call back function used to notify user application about
*          On Click button event.
*******************************************************************************/
typedef Byte (*D4D_ON_USR_MSG)(struct D4D_MESSAGE_S* pMsg);

// this is allocated in ROM always
/*! @brief The object main structure type definition. */
/*! @note This is main object (widget) data structure typically placed in ROM. It contains all common properties of objects */
typedef struct D4D_OBJECT_S
{
  D4D_POINT                             position;             ///< Position on the screen/object.
  D4D_SIZE                              size;                 ///< Size of the object.
  D4D_COOR                              radius;               ///< Object corners radius.
  D4D_MARGIN*                           pMargin;              ///< Object inner margin.
  void*                                 pParam;               ///< The object depends parameters.
  D4D_OBJECT_SYS_FUNCTION*              pObjFunc;             ///< The pointer on object system functions.
  D4D_ON_USR_MSG                        OnUsrMessage;         ///< The pointer on user message.
  D4D_OBJECT_USR_DATA                   userData;             ///< The pointer on user data container (user pointer and optionaly parent/children).
  D4D_OBJECT_RELATIONS                  pRelations;           ///< Relationship between the objects.
  D4D_OBJECT_INITFLAGS                  initFlags;            ///< The initializations object flags.
  struct D4D_CLR_SCHEME_S*              clrScheme;            ///< Pointer on used color scheme.
  D4D_OBJECT_DATA_PTR                   pData;                ///< Pointer on runtime object data.
} D4D_OBJECT;

/*! @brief The type definition of object pointer to ROM.*/
typedef const D4D_OBJECT* D4D_OBJECT_PTR;




/*! @} End of doxd4d_object_type                                               */
/******************************************************************************
* Internal types
******************************************************************************/


/******************************************************************************
* D4D Object macros
*
*//*! @addtogroup doxd4d_object_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that externs the D4D_OBJECT struxture
* @param   name of object structure
* @note    This is help macro to write clear D4D code to extern D4D_OBJECT to other C file
*******************************************************************************/
#define D4D_EXTERN_OBJECT(name) \
    extern const D4D_OBJECT name;

/**************************************************************************/ /*!
* @brief   Macro that gets the pointer to object data from object structure pointer
* @param   pObject - object structure pointer
* @return  pointer to object runtime data structure
* @note    This is help macro to write clear D4D code to get object data pointer
*******************************************************************************/
#define D4D_GET_OBJECT_DATA(pObject) ((pObject)->pData)

/**************************************************************************/ /*!
* @brief   Macro that find out the width of object graphic border by object flags
* @param   flags - object flags
* @return  witdh of object frame in pixels
* @note    This is help macro to find out object frame width
*******************************************************************************/
#define D4D_BORDER_WIDTH(flags) (((flags) & D4D_OBJECT_F_BEVEL_MASK)? (D4D_BEVEL_WIDTH) : (((flags) & D4D_OBJECT_F_FOCUSRECT)? 1:0))

/**************************************************************************/ /*!
* @brief   Macro that gets the type of bevel from the object flags
* @param   pObject - object structure pointer
* @return  the type of bevel (D4D_BEVEL)
* @note    This macro separate from object flags the type of object bevel in type D4D_BEVEL.
*******************************************************************************/
#define D4D_OBJECT_FLAGS2BEVEL(pObject) ((D4D_BEVEL)(((pObject)->pData->flags & D4D_OBJECT_F_BEVEL_MASK) >> D4D_OBJECT_F_BEVEL_SHIFT))


/**************************************************************************/ /*!
* @brief   Macro that create the object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of object
* @param   x - coordination of object in X axis
* @param   y - coordination of object in Y axis
* @param   cx - size of object in X axis
* @param   cy - size of object in Y axis
* @param   rad - radius of corners
* @param   margin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   relations - pointer to relation array (Could be NULL)
* @param   onusrmsg - pointer to object user message (Could be NULL)
* @param   sysFunc - pointer to object system functions structure (*\ref D4D_OBJECT_SYS_FUNCTION)
* @param   param - pointer to widget parameters
* @param   initFlags - init flags of object corresponding to \ref doxd4d_object_const_flags
* @param   userData - user data of  object
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw object

* @note    This macro create complete \ref D4D_OBJECT structure, including the object data sub structure. Is ussually used by graphic widgets declaration macros.
*******************************************************************************/
#define D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, rad, margin, relations, onusrmsg, sysFunc , param, initFlags, userData, pScheme) \
    D4D_OBJECT_DATA name##_Data = {(D4D_OBJECT_FLAGS)((initFlags | D4D_OBJECT_F_NOTINIT) & D4D_OBJECT_F_SYSTEM_MASK), NULL}; \
    type D4D_OBJECT name = \
    { \
        {x, y},\
        {cx, cy},\
        rad,\
        margin,\
        (void*)param, \
        (D4D_OBJECT_SYS_FUNCTION*)sysFunc, \
        onusrmsg, \
        (D4D_OBJECT_USR_DATA)userData, \
        relations,\
        initFlags, \
        pScheme, \
        &(name##_Data) \
    };

/**************************************************************************/ /*!
* @brief   Macro that defines at one line relations array
* @param   name - name of the object relation array
* @param   pParent - pointer to parent object - could be NULL if there is no parent object
* @param   ... - pointers to children objects - could be NULL if there is no child object
* @note    This macro simplify definition of object relation arrays
*******************************************************************************/
 #define D4D_DECLARE_OBJECT_RELATIONS(name, pParent, ...)\
    const D4D_OBJECT * const name[] =  {pParent, __VA_ARGS__ , NULL};

 // next way how to define object relation array

/**************************************************************************/ /*!
* @brief   Macro that defines at multi line relations array
* @param   name - name of the object relation array
* @param   pParent - pointer to parent object - could be NULL if there is no parent object
* @note    This macro is second way how to define object relation array, must be followed by \ref D4D_DECLARE_OBJECT_RELATIONS_CHILD or \ref D4D_DECLARE_OBJECT_RELATIONS_END
*******************************************************************************/
 #define D4D_DECLARE_OBJECT_RELATIONS_BEGIN(name, pParent)\
    const D4D_OBJECT * const name[] = \
    {\
     pParent,

/**************************************************************************/ /*!
* @brief   Macro that defines at multi line relations array - the add child part
* @param   child - name of the child object
* @note    This macro is second way how to define object relation array, must be followed by \ref D4D_DECLARE_OBJECT_RELATIONS_CHILD or \ref D4D_DECLARE_OBJECT_RELATIONS_END
*******************************************************************************/
 #define D4D_DECLARE_OBJECT_RELATIONS_CHILD(child) &child,

/**************************************************************************/ /*!
* @brief   Macro that defines at multi line relations array - the final part
* @note    This macro is second way how to define object relation array, it closes the robject relation parts definition
*******************************************************************************/
 #define D4D_DECLARE_OBJECT_RELATIONS_END NULL };

/*! @} End of doxd4d_object_macro                                             */
 #define D4D_OBJECT_MAX_TEXT_LEN(str) 1000



/******************************************************************************
* Public functions
******************************************************************************/
D4D_BOOL D4D_ObjectCheckCoordinates(D4D_OBJECT* pThis, D4D_POINT point);
D4D_BOOL D4D_IsEnabled(D4D_OBJECT* pObject);
D4D_BOOL D4D_IsVisible(D4D_OBJECT* pObject);
D4D_BOOL D4D_IsMineFocus(D4D_OBJECT* pObject);
void D4D_EnableTabStop(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable);
void D4D_EnableObject(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable);
void D4D_EnableTouchScreen(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable, D4D_BOOL bFastTouch);
void D4D_ShowObject(D4D_OBJECT_PTR pObject, D4D_BOOL bShow);
void D4D_EnableTimeTicks(D4D_OBJECT_PTR pObj, D4D_BOOL bEnable);
void D4D_InvalidateObject(D4D_OBJECT_PTR pObject, D4D_BOOL bComplete);
void* D4D_GetUserPointer(D4D_OBJECT *pThis);
void D4D_CaptureKeys(D4D_OBJECT_PTR pObj);
D4D_OBJECT* D4D_GetCapturedObject(void);
D4D_SIZE D4D_GetClientScreenSize(D4D_OBJECT* pObject);

#endif  /* __D4D_OBJECT_H */
