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
* @file      d4d_graph.h
*
* @author     Petr Gargulak
*
* @version   0.0.29.0
*
* @date      Jan-14-2014
*
* @brief     D4D Driver graph object header file
*
*******************************************************************************/

#ifndef __D4D_GRAPH_H
#define __D4D_GRAPH_H

extern const D4D_OBJECT_SYS_FUNCTION d4d_graphSysFunc;

/******************************************************************************
* D4D GRAPH setting  constants
*
*//*! @addtogroup doxd4d_graph_const
* @{
*******************************************************************************/

/*! @brief This is offset of graph area from object borders.
           If not defined, it sets to 5 pixel as a default.*/
#ifndef D4D_GRAPH_BORDER_OFF
  #define D4D_GRAPH_BORDER_OFF        5
#endif

/*! @brief This is offset of value text  from graph area borders.
           If not defined, it sets to 2 pixel as a default.*/
#ifndef D4D_GRAPH_VALUE_OFF
  #define D4D_GRAPH_VALUE_OFF        2
#endif

/*! @brief This is default graph grid color definition.
           If not defined, it sets to \ref D4D_COLOR_LIGHT_GREY as a default.*/
#ifndef D4D_COLOR_GRAPH_GRID
  #define D4D_COLOR_GRAPH_GRID        D4D_COLOR_LIGHT_GREY
#endif

//********************* Graph flags for its specific flags *****************
/*!
  @defgroup doxd4d_graph_const_flags D4D GRAPH Defines masks of its specific behaviour flags
  This section specifies masks of graph behaviour flags.
  @ingroup doxd4d_graph_const
*/

/**
 * @addtogroup doxd4d_graph_const_flags
 * @{
 */
  //@note    For axis, only one setting can be used, it is not allowed to mix two flags for one axis.

  #define D4D_GRAPH_F_MODE_MASK       (0x03 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Mask of graph mode (it's used internaly by driver)
  #define D4D_GRAPH_F_MODE_NORMAL     (0x00 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< The graph runs in normal mode, this means that a graph adds new data to the screen. When it fills up the whole graph area, the object clears all the shown points and draws a new graph on a screen from new data.
  #define D4D_GRAPH_F_MODE_ROLLOVER   (0x01 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< The graph runs in roll-over mode, this means the graph adds new data to the screen. When it fills up the whole graph area, the graph clears only the latest sample and draws a new one instead of the cleared one.

  #define D4D_GRAPH_F_VALUE_X_NONE    (0x00 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Mask of graph value enable from axis X (it's used internaly by driver)
  #define D4D_GRAPH_F_VALUE_X_BOTT    (0x04 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Enables a label for axis X on the bottom side of the graph.
  #define D4D_GRAPH_F_VALUE_X_TOP     (0x08 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Enables the label for axis X on the top side of the graph.

  #define D4D_GRAPH_F_VALUE_Y_NONE    (0x00 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Mask of graph value enable from axis Y (it's used internaly by driver)
  #define D4D_GRAPH_F_VALUE_Y_LEFT    (0x10 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Enables the label for axis Y on the left side of the graph.
  #define D4D_GRAPH_F_VALUE_Y_RIGHT   (0x20 << D4D_OBJECT_F_OBJECT_SHIFT)       ///< Enables the label for axis Y on the right side of the graph.
/**@}*/

/*! @brief This is graph init flags.
           If not defined, it sets to (\ref D4D_OBJECT_F_VISIBLE | \ref D4D_OBJECT_F_ENABLED | \ref D4D_OBJECT_F_BEVEL_RAISED | \ref D4D_GRAPH_F_MODE_ROLLOVER | \ref D4D_GRAPH_F_VALUE_X_BOTT | \ref D4D_GRAPH_F_VALUE_Y_RIGHT  ) as a default.*/
#ifndef D4D_GRAPH_F_DEFAULT
  #define D4D_GRAPH_F_DEFAULT  (D4D_OBJECT_F_VISIBLE | D4D_OBJECT_F_ENABLED | D4D_OBJECT_F_BEVEL_RAISED | D4D_GRAPH_F_MODE_ROLLOVER | D4D_GRAPH_F_VALUE_X_BOTT | D4D_GRAPH_F_VALUE_Y_RIGHT  )
#endif

/*! @brief This is graph init text properties.
           If not defined, it sets to (\ref D4D_ALIGN_H_CENTER_MASK | \ref D4D_ALIGN_V_CENTER_MASK) as a default.*/
#ifndef D4D_GRAPH_TXT_PRTY_DEFAULT
  #define D4D_GRAPH_TXT_PRTY_DEFAULT  (D4D_ALIGN_H_CENTER_MASK | D4D_ALIGN_V_CENTER_MASK)
#endif

/*! @brief This is graph init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_GRAPH_FNT_PRTY_DEFAULT
  #define D4D_GRAPH_FNT_PRTY_DEFAULT  ( 0 )
#endif

/*! @brief This is graph labels init font properties.
           If not defined, it sets to ( 0 ) as a default.*/
#ifndef D4D_GRAPH_LBL_FNT_PRTY_DEFAULT
  #define D4D_GRAPH_LBL_FNT_PRTY_DEFAULT  ( 0 )
#endif

//********************* Graph traces types *****************
/*!
  @defgroup doxd4d_graph_const_traceType D4D GRAPH Defines types of graph traces.
  This section specifies types of graph traces
  @ingroup doxd4d_graph_const
*/

/**
 * @addtogroup doxd4d_graph_const_traceType
 * @{
 */
  #define D4D_GRAPH_TRACE_TYPE_MASK       0x03  ///< Just mask of type range (it's used internaly by driver)
  #define D4D_GRAPH_TRACE_TYPE_LINE       0x00  ///< The trace looks like a normal line, all individual sample points are connected by the lines.
  #define D4D_GRAPH_TRACE_TYPE_DOT        0x01  ///< The trace is drawn only from measured points.
  #define D4D_GRAPH_TRACE_TYPE_AREA       0x02  ///< The trace fills up the area under measured samples.
  #define D4D_GRAPH_TRACE_TYPE_AREA_INV   0x03  ///< The trace fills up the area of measured samples.
/**@}*/

/*! @} End of doxd4d_graph_const                                             */

/******************************************************************************
* D4D GRAPH setting  public types
*
*//*! @addtogroup doxd4d_graph_type
* @{
*******************************************************************************/

/*! @brief   Type definition of graph value type - this is standard type used for graph data input.*/
typedef Byte D4D_GRAPH_VALUE;
/*! @brief   Type definition of graph dat asample type.*/
typedef LWord D4D_GRAPH_SAMPLE_IX;

/**************************************************************************/ /*!
* @brief   Type definition of graph on change callback function
* @param   pThis - pointer to graph instance that call the function
* @return  none
* @note    This is graph call back function used to notify user application about
*          On Change graph event.
*******************************************************************************/
typedef void (*D4D_GRAPH_ON_CHANGE)(D4D_OBJECT* pThis);

/**************************************************************************/ /*!
* @brief   Type definition of graph on need label call back function
* @param   pThis - pointer to graph instance that call the function
* @param   axisX - if D4D_TRUE th egraph need label for X axis, if D4D_FALSE for Y
* @param   gridIx - index of grid line for what the graph need the label
* @param   sampleIx - index of sample index for what eGUI needs the label
* @return  The text for the labels defined by parameters
* @note    This is call back function if is defined is designed to provide option to draw on the graph owns labels.
*******************************************************************************/
typedef D4D_TCHAR* (*D4D_GRAPH_ON_NEED_LABEL)(D4D_OBJECT_PTR pThis, D4D_BOOL axisX, D4D_INDEX gridIx, D4D_GRAPH_SAMPLE_IX sampleIx);


/*! @} End of doxd4d_gauge_type                                             */

/******************************************************************************
* Private Types
******************************************************************************/


typedef Word D4D_GRAPH_DATA_LEN;


typedef struct
{
  D4D_GRAPH_VALUE *pData;
  D4D_COLOR color;
  D4D_LINETYPE line;
  Byte type;
} D4D_GRAPH_TRACE;

typedef struct
{
    D4D_GRAPH_SAMPLE_IX xPosCnt;      // position counter on X axis
    D4D_GRAPH_DATA_LEN lastShowPos;   // position in input data buffer of shown data
    D4D_GRAPH_DATA_LEN lastDataPos;   // position in input data buffer of last added data
    D4D_GRAPH_DATA_LEN initData;
    D4D_GRAPH_DATA_LEN dataShowX; // count of showed data on axis X
    D4D_COOR lblSizeY;
    D4D_COOR columnDrawed;
    D4D_INDEX drigDrawedX;
} D4D_GRAPH_DATA;

typedef struct
{
  D4D_INDEX x_cnt;
  D4D_INDEX y_cnt;
}D4D_GRAPH_GRID;

// GRAPH configuration (in ROM by default)

typedef struct
{
    D4D_STRING                  textBuff;       // GRAPH main text
    D4D_GRAPH_GRID              grid;        // graph grid information
    const D4D_GRAPH_TRACE*      traces;
    D4D_GRAPH_DATA_LEN          dataLenght;
    D4D_FONT                    labelFont;
    D4D_GRAPH_ON_NEED_LABEL     OnNeedLabelText;
    D4D_GRAPH_ON_CHANGE         OnValueChanged;
    D4D_GRAPH_DATA*             pData;
} D4D_GRAPH;

/******************************************************************************
* Macros
******************************************************************************/

// getting the GRAPH structure from general OBJECT
#define D4D_GET_GRAPH(pObj) ((D4D_GRAPH*)((pObj)->pParam))

/******************************************************************************
* D4D GRAPH setting  public macros
*
*//*! @addtogroup doxd4d_graph_macro
* @{
*******************************************************************************/

/**************************************************************************/ /*!
* @brief   Macro that create the Graph object structure in memory including all substructures
* @param   type - type of object <\ref D4D_CONST; \ref D4D_NO_CONST>
* @param   name - name of graph object
* @param   text - title text of graph
* @param   x - coordination of graph in X axis
* @param   y - coordination of graph in Y axis
* @param   cx - size of graph in X axis (width)
* @param   cy - size of graph in Y axis (height)
* @param   radius - radius of corners
* @param   pMargin - pointer to margin structure (*\ref D4D_MARGIN)(Could be NULL)
* @param   pRelations - pointer to object relation array (Could be NULL)
* @param   gx - count of grid lines in axis X
* @param   gy - count of grid lines in axis Y
* @param   dataLen - lenght of internal data buffer. This value extremly impact the memory consumption (memory = dataLen * cntOfTraces)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_graph_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   lblFontId - Identification number of the used labels text font
* @param   pOnNeedLblTxt - Pointer to the calll back function that is called when text is needed to draw a label \ref D4D_GRAPH_ON_NEED_LABEL
* @param   pUser - user data of  button
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_GRAPH_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GRAPH structure, including the object data sub structure. Is used to define all properties of graph.
*******************************************************************************/
#define _D4D_DECLARE_GRAPH_BEGIN(type, name, text, x, y, cx, cy, radius, pMargin, pRelations, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
    extern type D4D_GRAPH_TRACE  name##_traces[];\
    static D4D_GRAPH_DATA name##_data = { 0, 0, 0, 0, 0, 0, 0, 0}; /* reset data of graph*/ \
    static D4D_STR_PROPERTIES name##_strPrties = { D4D_GRAPH_FNT_PRTY_DEFAULT, D4D_GRAPH_TXT_PRTY_DEFAULT}; \
    static type D4D_GRAPH name##_params = \
    { \
        { text, D4D_TEXT_LEN(text), fontId, &name##_strPrties, D4D_OBJECT_MAX_TEXT_LEN(text), 0}, /* textBuff */ \
        { gx, gy },   /* grid info  */ \
        &(name##_traces[0]),  /* pLines     */ \
        dataLen,      /* Length of input data buffers */ \
        lblFontId,    /* Font ID for axis values */ \
        pOnNeedLblTxt, /* On need the label text*/ \
        pOnValch,      /* OnValueCanged */ \
        &(name##_data) /* RAM-based data */ \
    }; \
    \
    D4D_DECLARE_OBJECT(type, name, x, y, cx, cy, radius, pMargin, pRelations, pOnUsrMsg, &d4d_graphSysFunc, &(name##_params), (flags), pUser, pScheme)\
    \
    type D4D_GRAPH_TRACE name##_traces[] = \
    {


/**************************************************************************/ /*!
* @brief   Macro that is used to add trace to graph definition
* @param   pData - pointer to data buffer for this trace - it must have at least lenght "dataLen" from \ref _D4D_DECLARE_GRAPH_BEGIN declaration
* @param   clr - color of trace - \ref D4D_COLOR
* @param   line - line type - \ref D4D_LINETYPE
* @param   type - type of trace - \ref doxd4d_graph_const_traceType
* @note    This macro is used after the graph declaration to add new trace
* Here is an example demonstrating how to used it.
* @code
*       D4D_GRAPH_VALUE  my_traceData1[200];
*       D4D_GRAPH_VALUE  my_traceData2[200];
*       D4D_DECLARE_STD_GRAPH_BEGIN(my_graph, D4D_DEFSTR("My Nice Graph"), 10, 10, 200, 150, 4, 3, 200, MY_FONT, MY_SMALL_FONT)
*       D4D_DECLARE_GRAPH_TRACE(my_traceData1, D4D_WHITE, D4D_LINE_THIN, D4D_GRAPH_TRACE_TYPE_LINE)
*       D4D_DECLARE_GRAPH_TRACE(my_traceData2, D4D_RED, D4D_LINE_THICK, D4D_GRAPH_TRACE_TYPE_DOT)
*       D4D_DECLARE_GRAPH_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_GRAPH_TRACE(pData, clr, line, type) { pData, clr , line, type},

/**************************************************************************/ /*!
* @brief   Macro that is used to end of graph definition
* @note    This macro is used as a last declaration macro in Graph definition
* Here is an example demonstrating how to used it.
* @code
*       D4D_GRAPH_VALUE  my_traceData1[200];
*       D4D_GRAPH_VALUE  my_traceData2[200];
*       D4D_DECLARE_STD_GRAPH_BEGIN(my_graph, D4D_DEFSTR("My Nice Graph"), 10, 10, 200, 150, 4, 3, 200, MY_FONT, MY_SMALL_FONT)
*       D4D_DECLARE_GRAPH_TRACE(my_traceData1, D4D_WHITE, D4D_LINE_THIN, D4D_GRAPH_TRACE_TYPE_LINE)
*       D4D_DECLARE_GRAPH_TRACE(my_traceData2, D4D_RED, D4D_LINE_THICK, D4D_GRAPH_TRACE_TYPE_DOT)
*       D4D_DECLARE_GRAPH_END()
* @endcode
*******************************************************************************/
#define D4D_DECLARE_GRAPH_END() { NULL, D4D_COLOR_WHITE , D4D_LINE_THIN, 0}};


/**************************************************************************/ /*!
* @brief   Macro that create the Graph object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of graph object
* @param   text - title text of graph
* @param   x - coordination of graph in X axis
* @param   y - coordination of graph in Y axis
* @param   cx - size of graph in X axis (width)
* @param   cy - size of graph in Y axis (height)
* @param   gx - count of grid lines in axis X
* @param   gy - count of grid lines in axis Y
* @param   dataLen - lenght of internal data buffer. This value extremly impact the memory consumption (memory = dataLen * cntOfTraces)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_graph_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   lblFontId - Identification number of the used labels text font
* @param   pOnNeedLblTxt - Pointer to the calll back function that is called when text is needed to draw a label \ref D4D_GRAPH_ON_NEED_LABEL
* @param   pUser - user data of  button
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_GRAPH_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GRAPH structure, including the object data sub structure. Is used to define all properties of graph. Is used to define all properties of graph. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_GRAPH_BEGIN instead of this one.
*******************************************************************************/
#define D4D_DECLARE_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the Graph object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of graph object
* @param   text - title text of graph
* @param   x - coordination of graph in X axis
* @param   y - coordination of graph in Y axis
* @param   cx - size of graph in X axis (width)
* @param   cy - size of graph in Y axis (height)
* @param   gx - count of grid lines in axis X
* @param   gy - count of grid lines in axis Y
* @param   dataLen - lenght of internal data buffer. This value extremly impact the memory consumption (memory = dataLen * cntOfTraces)
* @param   fontId - Identification number of the used title text font
* @param   lblFontId - Identification number of the used labels text font

* @note    This macro create complete D4D_GRAPH structure, including the object data sub structure. Is used to define all properties of graph. Is used to define all properties of graph. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_GRAPH_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, fontId, lblFontId) \
  D4D_DECLARE_GRAPH_BEGIN(name, text, x, y, cx, cy, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, lblFontId, NULL, NULL, NULL, NULL)

// Rounded graph definition
/**************************************************************************/ /*!
* @brief   Macro that create the rounded Graph object structure in memory including all substructures with restricted count of parameters to simplify definition
* @param   name - name of graph object
* @param   text - title text of graph
* @param   x - coordination of graph in X axis
* @param   y - coordination of graph in Y axis
* @param   cx - size of graph in X axis (width)
* @param   cy - size of graph in Y axis (height)
* @param   radius - radius of corners
* @param   gx - count of grid lines in axis X
* @param   gy - count of grid lines in axis Y
* @param   dataLen - lenght of internal data buffer. This value extremly impact the memory consumption (memory = dataLen * cntOfTraces)
* @param   flags - bitmask that specifies initial \ref doxd4d_object_const_flags and \ref doxd4d_graph_const_flags
* @param   pScheme - pointer to color scheme. In case that this parameter is NULL, the default scheme color will be used for draw button
* @param   fontId - Identification number of the used title text font
* @param   lblFontId - Identification number of the used labels text font
* @param   pOnNeedLblTxt - Pointer to the calll back function that is called when text is needed to draw a label \ref D4D_GRAPH_ON_NEED_LABEL
* @param   pUser - user data of  button
* @param   pOnValch - Pointer to an on-change user callback function \ref D4D_GRAPH_ON_CHANGE
* @param   pOnUsrMsg -Pointer to an on user message callback function \ref D4D_ON_USR_MSG. This callback is called before this
*               message event is sent to the object itself. The message can be skipped by the \ref D4D_MSG_SKIP
*               return value, in a normal case the return value must be \ref D4D_MSG_NOSKIP

* @note    This macro create complete D4D_GRAPH structure, including the object data sub structure. Is used to define all properties of graph. Is used to define all properties of graph. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_GRAPH_BEGIN instead of this one.
*******************************************************************************/
#define D4D_DECLARE_RGRAPH_BEGIN(name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Macro that create the rounded Graph object structure in memory including all substructures with restricted count of parameters to simplify definition
*               The missing parameters are replaced by default values.
* @param   name - name of graph object
* @param   text - title text of graph
* @param   x - coordination of graph in X axis
* @param   y - coordination of graph in Y axis
* @param   cx - size of graph in X axis (width)
* @param   cy - size of graph in Y axis (height)
* @param   radius - radius of corners
* @param   gx - count of grid lines in axis X
* @param   gy - count of grid lines in axis Y
* @param   dataLen - lenght of internal data buffer. This value extremly impact the memory consumption (memory = dataLen * cntOfTraces)
* @param   fontId - Identification number of the used title text font
* @param   lblFontId - Identification number of the used labels text font

* @note    This macro create complete D4D_GRAPH structure, including the object data sub structure. Is used to define all properties of graph. Is used to define all properties of graph. If
*               there is missing parameter that is needed by user application used the full macro \ref _D4D_DECLARE_GRAPH_BEGIN instead of this one.
*               The main advantage is less parameters of this macro against the full version.
*******************************************************************************/
#define D4D_DECLARE_STD_RGRAPH_BEGIN(name, text, x, y, cx, cy, radius, gx, gy, dataLen, fontId, lblFontId) \
  D4D_DECLARE_RGRAPH_BEGIN(name, text, x, y, cx, cy, radius, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, lblFontId, NULL, NULL, NULL, NULL)


// IN RAM instantions macros
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_GRAPH_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_GRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, 0, NULL, NULL, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_GRAPH_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_GRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, gx, gy, dataLen, fontId, lblFontId) \
  D4D_DECLARE_STD_GRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, lblFontId, NULL, NULL, NULL, NULL)


// Rounded graph definition
/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_RGRAPH_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_RGRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, radius, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg) \
  _D4D_DECLARE_GRAPH_BEGIN(D4D_NO_CONST, name, text, x, y, cx, cy, radius, NULL, NULL, gx, gy, dataLen, flags, pScheme, fontId, lblFontId, pOnNeedLblTxt, pUser, pOnValch, pOnUsrMsg)

/**************************************************************************/ /*!
* @brief   Same as \ref D4D_DECLARE_STD_RGRAPH_BEGIN, but is created in RAM instead of the ROM memory
*******************************************************************************/
#define D4D_DECLARE_STD_RGRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, radius, gx, gy, dataLen, fontId, lblFontId) \
  D4D_DECLARE_RGRAPH_BEGIN_INRAM(name, text, x, y, cx, cy, radius, gx, gy, dataLen, (D4D_GRAPH_F_DEFAULT), NULL, fontId, lblFontId, NULL, NULL, NULL, NULL)

/*! @} End of doxd4d_graph_macro                                             */
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/*********************************************************
*
* global functions
*
*********************************************************/
D4D_BOOL D4D_GraphAddTraceData(D4D_OBJECT_PTR pObj, D4D_INDEX trace_ix,  D4D_GRAPH_VALUE value);
void D4D_GraphAddTracesData(D4D_OBJECT_PTR pObj, D4D_GRAPH_VALUE* pValues);
void D4D_GraphClearAll(D4D_OBJECT_PTR pObj);
D4D_GRAPH_SAMPLE_IX D4D_GraphGetSampleIndex(D4D_OBJECT_PTR pObj);
D4D_COOR D4D_GraphGetSizeX(D4D_OBJECT_PTR pObj);
D4D_BOOL D4D_GraphSetScaleX(D4D_OBJECT_PTR pObj, Byte mul, Byte div);
D4D_BOOL D4D_GraphSetDataWidth(D4D_OBJECT_PTR pObj, D4D_INDEX samples);


// Obsolete functions, replaced by any general
#define D4D_GraphSetText D4D_SetText




#endif /* __D4D_GRAPH_H */


