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
* @file      d4d_lldapi.h
*
* @author     Petr Gargulak
*
* @version   0.0.22.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - resistive touch screen driver function header file
*
*******************************************************************************/

#ifndef __D4D_LLD_API_H
#define __D4D_LLD_API_H


/******************************************************************************
* Includes
******************************************************************************/
#include "d4d_user_cfg.h"
#include "common_files/d4d_types.h"

/******************************************************************************
* D4D LLD API types
*
*//*! @addtogroup doxd4d_lldapi_types
* @{
*******************************************************************************/

/*! @brief D4D low level screen orientation enumeration type.*/
typedef enum
{
  Portrait      = D4D_ORIENT_PORTRAIT,          ///< Orientation Portrait
  Portrait180   = D4D_ORIENT_PORTRAIT180,       ///< Orientation Portrait up side down
  Landscape     = D4D_ORIENT_LANDSCAPE,         ///< Orientation LandScape
  Landscape180  = D4D_ORIENT_LANDSCAPE180       ///< Orientation LandScape up side down
} D4DLCD_ORIENTATION;

/*! @brief D4D low level MCU pin state enumeration type*/
typedef enum
{
  D4DHW_PIN_PULL_UP_ON, ///< Switch on Pull Up resistor on pin
  D4DHW_PIN_PULL_UP_OFF,///< Switch off Pull Up resistor on pin
  D4DHW_PIN_OUT,        ///< Switch pin to output mode
  D4DHW_PIN_IN,         ///< Switch pin to input mode (equivalent to high-Z)
  D4DHW_PIN_SET_0,      ///< Set pin output register to logic 0
  D4DHW_PIN_SET_1,      ///< Set pin output register to logic 1
  D4DHW_PIN_GET,        ///< Get the pin input value
  D4DHW_PIN_ADC_ON,     ///< Switch on the pin for read by ADC
  D4DHW_PIN_ADC_OFF     ///< Switch off the pin for read by ADC
}D4DHW_PIN_STATE;

/*! @brief D4D low level MCU types definition for analog resistive touch screen signals.*/
typedef enum
{
  D4DTCH_X_PLUS_PIN,    ///< Analog touch screen X+ signal
  D4DTCH_X_MINUS_PIN,   ///< Analog touch screen X- signal
  D4DTCH_Y_PLUS_PIN,    ///< Analog touch screen Y+ signal
  D4DTCH_Y_MINUS_PIN    ///< Analog touch screen Y- signal
}D4DTCHHW_PINS;

/*! @brief D4D low level MCU types definition for general LCD screen control signals.*/
typedef enum
{
  D4DLCD_RESET_PIN,     ///< LCD device reset signal
  D4DLCD_BACKLIGHT_PIN, ///< LCD device backlight enable signal
  D4DLCD_CS_PIN         ///< LCD device chip select signal
}D4DLCDHW_PINS;

/*! @brief D4D low level touch screen limitation structure.*/
/*! @note  This structure contains the limitation data for touch screen driver.*/
typedef struct D4D_TOUCHSCREEN_LIMITS_S
{
  unsigned short rawDataScale;  ///< The scale mask of ADC convertor
  unsigned short touchMinX;     ///< Minimal valid raw value of axis X
  unsigned short touchMinY;     ///< Minimal valid raw value of axis Y
  unsigned short touchOffMaxX;  ///< Maximal valid raw value of axis X
  unsigned short touchOffMaxY;  ///< Maximal valid raw value of axis Y
}D4D_TOUCHSCREEN_LIMITS;

/*! @brief D4D low level frame buffer description structure.*/
/*! @note  This structure is used only with frame buffer type of low level driver.
           The size of frame buffer could be computed by (lcd_x_max * lcd_y_max * bpp_byte). */
typedef struct D4DLCD_FRAMEBUFF_DESC_S
{
  unsigned long fb_start_addr;  ///< The address of frame buffer
  unsigned short lcd_x_max;     ///< The resolution of LCD in axis X
  unsigned short lcd_y_max;     ///< The resolution of LCD in axis Y
  unsigned char bpp_byte;	///< The bytes per pixel
}D4DLCD_FRAMEBUFF_DESC;

/*! @brief D4D low level mouse pointer structure.*/
/*! @note  This structure contains the mouse pointer description from mouse driver driver.
            Is used just if mouse pointer is used in project.*/
typedef struct
{
  D4D_COOR size_x;      ///< The size of omuse pointer in axis X
  D4D_COOR size_y;      ///< The size of omuse pointer in axis Y
  LWord  bmpFormat;     ///< The bitmap format of mouse pointer (corresponded to main eGUI internal bitmap types)
}D4DMOUSE_DESC;

/*! @brief D4D low level eGUI flush screen types enumeration.*/
typedef enum
{
  D4DLCD_FLSH_ELEMENT,          ///< The flush after one basic element is draw
  D4DLCD_FLSH_SCR_START,        ///< The notification that the start of screen is done
  D4DLCD_FLSH_SCR_END,          ///< The notification that the end of screen is draw
  D4DLCD_FLSH_FORCE             ///< The notification of forcing flush by user code
} D4DLCD_FLUSH_MODE;

/*! @brief D4D low level standard LCD interface API structure.*/
/*! @note  This structure contains all needed API function pointers to currently used driver.
           All the function MUST be defined in low level driver */
typedef struct D4DLCD_FUNCTIONS_S
{
  unsigned char (*D4DLCD_Init)(void);                   ///< The LCD driver initialization function
  unsigned char (*D4DLCD_SetWindow)(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);        ///< The LCD driver Set logic window function. Into this logic window will be draw pixels
  unsigned char (*D4DLCD_SetOrientation)(D4DLCD_ORIENTATION new_orientation);   ///< The LCD driver set orientation function
  void (*D4DLCD_Send_PixelColor)(D4D_COLOR value);      ///< The LCD driver send pixel function. The pixels will be draw on position (automaticaly incremented) sets by D4DLCD_SetWindow function.
  D4D_COLOR (*D4DLCD_Read_PixelColor)(void);            ///< The LCD driver read pixel function. The pixels will be read on position (automaticaly incremented) sets by D4DLCD_SetWindow function.
  void (*D4DLCD_FlushBuffer)(D4DLCD_FLUSH_MODE mode);   ///< The LCD driver flush function.
  void (*D4DLCD_Delay_ms)(unsigned short period);       ///< The LCD driver delay function.
  unsigned char (*D4DLCD_DeInit)(void);                 ///< The LCD driver deinicialization function
  unsigned char (*D4DLCD_FillRect)(D4D_COLOR value);    ///< The LCD Driver fill rect function
}D4DLCD_FUNCTIONS;


/*! @brief D4D low level standard LCD Hardware interface API structure.*/
/*! @note  This structure contains all needed API function pointers to currently used hardware interface driver.
           All the function MUST be defined in hardware interface low level driver */
typedef struct D4DLCDHW_FUNCTIONS_S
{
  unsigned char (*D4DLCDHW_Init)(void);         ///< The LCD HW interface driver initialization function
  void (*D4DLCDHW_SendDataWord)(Word value);    ///< The LCD HW interface driver send data word function
  void (*D4DLCDHW_SendCmdWord)(Word cmd);       ///< The LCD HW interface driver send command word function
  Word (*D4DLCDHW_ReadDataWord)(void);          ///< The LCD HW interface driver read data word function
  Word (*D4DLCDHW_ReadCmdWord)(void);           ///< The LCD HW interface driver read command word function
  unsigned char (*D4DLCDHW_PinCtl)(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);      ///< The LCD HW interface driver control HW pins function
  void (*D4DLCDHW_FlushBuffer)(D4DLCD_FLUSH_MODE mode);         ///< The LCD HW interface driver flush data function
  unsigned char (*D4DLCDHW_DeInit)(void);       ///< The LCD HW interface driver deinicialization function
}D4DLCDHW_FUNCTIONS;


/*! @brief D4D low level frame buffer LCD interface API structure.*/
/*! @note  This structure contains all needed API function pointers to currently used frame buffer driver.
           All the function MUST be defined in low level driver */
typedef struct D4DLCDHWFB_FUNCTIONS_S
{
  unsigned char (*D4DLCDHW_Init)(void);         ///< The frame buffer LCD driver initialization function
  void (*D4DLCDHW_WriteData)(unsigned long addr, D4D_COLOR value);      ///< The frame buffer LCD driver write data to buffer function
  D4D_COLOR (*D4DLCDHW_ReadData)(unsigned long addr);                   ///< The frame buffer LCD driver read data from buffer function
  D4DLCD_FRAMEBUFF_DESC* (*D4DLCDHW_GetFbDescriptor)(void);             ///< The frame buffer LCD driver get frame buffer descriptor function
  unsigned char (*D4DLCDHW_PinCtl)(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);      ///< The frame buffer LCD driver control HW pins function
  void (*D4DLCDHW_FlushBuffer)(D4DLCD_FLUSH_MODE mode);                 ///< The frame buffer LCD driver flush data function
  unsigned char (*D4DLCDHW_DeInit)(void);                               ///< The frame buffer LCD driver deinitialization function
}D4DLCDHWFB_FUNCTIONS;



/*! @brief D4D low level touch screen interface API structure.*/
/*! @note  This structure contains all needed API function pointers to currently used touch screen driver.
           All the function MUST be defined in low level driver */
typedef struct D4DTCH_FUNCTIONS_S
{
  unsigned char (*D4DTCH_Init)(void);                           ///< The touch screen driver initialization function
  unsigned char (*D4DTCH_GetPositionRaw)(unsigned short *TouchPositionX, unsigned short *TouchPositionY);  ///< The touch screen driver get raw data function
  D4D_TOUCHSCREEN_LIMITS* (*D4DTCH_GetRawLimits)(void);         ///< The touch screen driver get touch screen hardware limit structure function
  unsigned char (*D4DTCH_DeInit)(void);                         ///< The touch screen driver deinitialization function
}D4DTCH_FUNCTIONS;


/*! @brief D4D low level touch screen hardware interface API structure.*/
/*! @note  This structure contains all needed API function pointers to currently used touch screen hardware interface driver.
           All the function MUST be defined in low level driver */
typedef struct D4DTCHHW_FUNCTIONS_S
{
  unsigned char (*D4DTCHHW_Init)(void);                         ///< The touch screen hardware interface driver initialization function
  unsigned short (*D4DTCHHW_ReadTouchAxis)(D4DTCHHW_PINS pinId);///< The touch screen hardware interface driver read one axis function
  D4D_TOUCHSCREEN_LIMITS* (*D4DTCHHW_GetRawLimits)(void);       ///< The touch screen hardware interface driver get hardware limits function
  unsigned char (*D4DTCHHW_PinCtl)(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);      ///< The touch screen hardware interface driver control HW pins function
  unsigned char (*D4DTCHHW_DeInit)(void);                       ///< The touch screen hardware interface driver deinitialization function
}D4DTCHHW_FUNCTIONS;

/*! @brief D4D low level mouse pointer interface API structure.*/
/*! @note  This structure contains all needed API function pointers to currently used mouse pointer interface driver.
           All the function MUST be defined in low level driver */
typedef struct D4DMOUSE_FUNCTIONS_S
{
  unsigned char (*D4DMOUSE_Init)(void);                         ///< The mouse pointer interface driver initialization function
  void (*D4DMOUSE_SetCoor)(unsigned short x, unsigned short y); ///< The mouse pointer interface driver set current coordination function
  void (*D4DMOUSE_SetPointerBmp)(void** pPntrData, D4D_INDEX cnt, D4D_COLOR keyColor);  ///< The mouse pointer interface driver set pointer to mouse cursors bitmaps function
  void (*D4DMOUSE_SetPointer)(D4D_INDEX cnt, D4D_BOOL show);    ///< The mouse pointer interface driver set cursor index and also visibility function
  D4DMOUSE_DESC* (*D4DMOUSE_GetDescriptor)(void);               ///< The mouse pointer interface driver get gescriptor function
  unsigned char (*D4DMOUSE_DeInit)(void);                       ///< The mouse pointer interface driver deinitialization function
}D4DMOUSE_FUNCTIONS;

/*! @} End of doxd4d_lldapi_types                                             */

// Include the right low level driver for LCD/TFT

#ifndef D4D_LLD_LCD
  #error "You must #define symbol D4D_LLD_LCD in d4d_user_cfg.h header file"
#endif

extern const D4DLCD_FUNCTIONS D4D_LLD_LCD;

#ifdef D4D_LLD_TCH
  extern const D4DTCH_FUNCTIONS D4D_LLD_TCH;
#endif

#ifdef D4D_LLD_TCH_HW
  extern const D4DTCHHW_FUNCTIONS D4D_LLD_TCH_HW;
#endif

#ifdef D4D_LLD_MOUSE
  extern const D4DMOUSE_FUNCTIONS D4D_LLD_MOUSE;
#endif


#endif /* __D4D_LLD_API_H */










