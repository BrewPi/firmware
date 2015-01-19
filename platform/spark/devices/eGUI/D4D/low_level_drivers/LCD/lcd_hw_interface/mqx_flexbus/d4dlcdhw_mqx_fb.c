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
* @file      d4dlcdhw_mqx_fb.c
*
* @author     Petr Gargulak
*
* @version   0.0.11.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - Mqx hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_mqx_fb_ID 1


// copilation enable preprocessor condition
// the string d4dtch_Mqx_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_fb_ID)

// include of low level driver heaser file
// it will be included into wole project only in case that this driver is selected in main D4D configuration file
#include <mqx.h>
#include <bsp.h>
#include <lwgpio.h>


#ifndef BSP_EXTERNAL_LCD_DC
   #error Define BSP_EXTERNAL_LCD_DC in bsp.
#endif

#ifndef BSP_EXTERNAL_LCD_BASE
   #error Define BSP_EXTERNAL_LCD_BASE in bsp.
#endif

extern void _bsp_flexbus_lcd_setup (const uint32_t base_address);
/******************************************************************************
* Macros
******************************************************************************/
#if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
  #error The eGUI low level driver "d4dlcdhw_mqx_fb" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
#endif
/******************************************************************************
* Internal function prototypes
******************************************************************************/

static unsigned char D4DLCDHW_Init_Mqx(void);
static void D4DLCDHW_SendDataWord_Mqx(unsigned short value);
static void D4DLCDHW_SendCmdWord_Mqx(unsigned short cmd);
static unsigned short D4DLCDHW_ReadDataWord_Mqx(void);
static unsigned short D4DLCDHW_ReadCmdWord_Mqx(void);
static unsigned char D4DLCDHW_PinCtl_Mqx(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
static void D4DLCD_FlushBuffer_Mqx(D4DLCD_FLUSH_MODE mode);
static unsigned char D4DLCDHW_DeInit_Mqx(void);

/**************************************************************//*!
*
* Global variables
*
******************************************************************/

// the main structure that contains low level driver api functions
// the name fo this structure is used for recognizing of configured low level driver of whole D4D
// so this name has to be used in main configuration header file of D4D driver to enable this driver
const D4DLCDHW_FUNCTIONS d4dlcdhw_mqx_fb =
{
   D4DLCDHW_Init_Mqx,
   D4DLCDHW_SendDataWord_Mqx,
   D4DLCDHW_SendCmdWord_Mqx,
   D4DLCDHW_ReadDataWord_Mqx,
   D4DLCDHW_ReadCmdWord_Mqx,
   D4DLCDHW_PinCtl_Mqx,
   D4DLCD_FlushBuffer_Mqx,
   D4DLCDHW_DeInit_Mqx
};
/**************************************************************//*!
*
* Local variables
*
******************************************************************/

#if defined BSP_LCD_RESET
   static LWGPIO_STRUCT lcd_reset_pin;
#endif

#if defined BSP_LCD_BACKLIGHT
   static LWGPIO_STRUCT lcd_backlight_pin;
#endif

/**************************************************************//*!
*
* Functions bodies
*
******************************************************************/


//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_Init_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     1 ok; 2 gpio init failed
//
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_Init_Mqx(void)
{
  unsigned char result = 1;

  #if defined BSP_LCD_RESET
    /* initialize lwgpio handle (lcd_reset_pin) for BSP_LCD_RESET pin (defined in mqx/source/bsp/<bsp_name>/<bsp_name>.h file) */
    if (!lwgpio_init(&lcd_reset_pin, BSP_LCD_RESET, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_HIGH))
    {
      printf("Initializing LCD_RESET GPIO as output failed.\n");
      return D4D_FALSE;
    }
    /* swich pin functionality (MUX) to GPIO mode */
    lwgpio_set_functionality(&lcd_reset_pin, BSP_LCD_RESET_MUX_GPIO);
  #endif

  #if defined BSP_LCD_BACKLIGHT
    /* initialize lwgpio handle (lcd_backlight_pin) for BSP_LCD_BACKLIGHT pin (defined in mqx/source/bsp/<bsp_name>/<bsp_name>.h file) */
    if (!lwgpio_init(&lcd_backlight_pin, BSP_LCD_BACKLIGHT, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_LOW))
    {
      printf("Initializing LCD_BACKLIGHT GPIO as output failed.\n");
      return D4D_FALSE;
    }
    /* swich pin functionality (MUX) to GPIO mode */
    lwgpio_set_functionality(&lcd_backlight_pin, BSP_LCD_BACKLIGHT_MUX_GPIO);
  #endif

  // Init the FlexBus Peripheral
  _bsp_flexbus_lcd_setup((uint32_t)BSP_EXTERNAL_LCD_BASE);

  return result;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_Init_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     none
//
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_DeInit_Mqx(void)
{
   return 1;
}
//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_SendDataWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit variable into LCD
//
// PARAMETERS:  unsigned short value    variable to send
//
// RETURNS:     none
//
//-----------------------------------------------------------------------------
static void D4DLCDHW_SendDataWord_Mqx(unsigned short value)
{
   *((unsigned short*)BSP_EXTERNAL_LCD_DC) = value;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_SendCmdWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit command into LCD
//
// PARAMETERS:  unsigned short cmd    command to send
//
// RETURNS:     none
//
//-----------------------------------------------------------------------------
static void D4DLCDHW_SendCmdWord_Mqx(unsigned short cmd)
{
   *((unsigned short*)BSP_EXTERNAL_LCD_BASE) = cmd;
}


//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this
//              function is supported)
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//
//-----------------------------------------------------------------------------
static unsigned short D4DLCDHW_ReadDataWord_Mqx(void)
{
   return *((unsigned short*)BSP_EXTERNAL_LCD_BASE);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadCmdWord_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this
//              function is supported)
//
// PARAMETERS:  none
//
// RETURNS:     unsigned short - the readed value
//
//-----------------------------------------------------------------------------
static unsigned short D4DLCDHW_ReadCmdWord_Mqx(void)
{
   return *((unsigned short*)BSP_EXTERNAL_LCD_BASE);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_PinCtl_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: allows control GPIO pins for LCD conrol purposes
//
// PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
//              D4DHW_PIN_STATE setState - Pin action
// RETURNS:     for Get action retuns the pin value
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_PinCtl_Mqx(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
{
   switch( pinId )
   {
      case D4DLCD_RESET_PIN:
         #if defined BSP_LCD_RESET
         switch( setState )
         {
            case D4DHW_PIN_SET_1:
               lwgpio_set_value(&lcd_reset_pin, LWGPIO_VALUE_HIGH);
               break;

            case D4DHW_PIN_SET_0:
               lwgpio_set_value(&lcd_reset_pin, LWGPIO_VALUE_LOW);
               break;

            default:
               break;
         }
         #endif
         break;

      case D4DLCD_BACKLIGHT_PIN:
         #if defined BSP_LCD_BACKLIGHT
         switch( setState )
         {
            case D4DHW_PIN_SET_1:
               lwgpio_set_value(&lcd_backlight_pin, LWGPIO_VALUE_HIGH);
               break;

            case D4DHW_PIN_SET_0:
               lwgpio_set_value(&lcd_backlight_pin, LWGPIO_VALUE_LOW);
               break;

            default:
               break;
         }
         #endif
         break;

      default:
         break;
   }
   return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCD_FlushBuffer_Mqx
// SCOPE:       Low Level Driver API function
// DESCRIPTION: For buffered low level interfaces is used to inform
//              driver the complete object is drawed and pending pixels should be flushed
//
// PARAMETERS:  none
//
// RETURNS:     none
//-----------------------------------------------------------------------------
static void D4DLCD_FlushBuffer_Mqx(D4DLCD_FLUSH_MODE mode)
{
  D4D_UNUSED(mode);
}

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_Mqx_ID)
