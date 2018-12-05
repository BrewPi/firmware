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
* @file      d4dlcdhw_gpio8080_8b.c
*
* @author     Petr Gargulak
*
* @version   0.0.11.0
*
* @date      Aug-6-2012
*
* @brief     D4D driver - gpio8080_8b hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_gpio8080_8b_ID 1


// copilation enable preprocessor condition
// the string d4dtch_gpio8080_8b_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_gpio8080_8b_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/gpio_8080_8bit/d4dlcdhw_gpio8080_8b.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/
  #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    #error The eGUI low level driver "d4dlcdhw_gpio8080_8b" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
  #endif
  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_Gpio8080_8b(void);
  static unsigned char D4DLCDHW_DeInit_Gpio8080_8b(void);
  static void D4DLCDHW_SendDataWord_Gpio8080_8b(unsigned short value);
  static void D4DLCDHW_SendCmdWord_Gpio8080_8b(unsigned short cmd);
  static unsigned short D4DLCDHW_ReadDataWord_Gpio8080_8b(void);
  static unsigned short D4DLCDHW_ReadCmdWord_Gpio8080_8b(void);
  static unsigned char D4DLCDHW_PinCtl_Gpio8080_8b(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_Gpio8080_8b(D4DLCD_FLUSH_MODE mode);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHW_FUNCTIONS d4dlcdhw_gpio8080_8b =
  {
    D4DLCDHW_Init_Gpio8080_8b,
    D4DLCDHW_SendDataWord_Gpio8080_8b,
    D4DLCDHW_SendCmdWord_Gpio8080_8b,
    D4DLCDHW_ReadDataWord_Gpio8080_8b,
    D4DLCDHW_ReadCmdWord_Gpio8080_8b,
    D4DLCDHW_PinCtl_Gpio8080_8b,
    D4DLCD_FlushBuffer_Gpio8080_8b,
    D4DLCDHW_DeInit_Gpio8080_8b
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_Init_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_Init_Gpio8080_8b(void)
  {
    #ifdef D4DLCD_DISPLAY_MCU_USER_INIT
      D4DLCD_DISPLAY_MCU_USER_INIT
    #endif

    D4DLCD_ASSERT_CS;
    D4DLCD_ASSERT_DC;

    D4DLCD_INIT_CS;
    D4DLCD_INIT_DC;


    D4DLCD_INIT_DATA;    // 8 bit data port all outputs

    D4DLCD_INIT_WR;
    D4DLCD_INIT_RD;

    D4DLCD_DEASSERT_WR;
    D4DLCD_DEASSERT_RD;

    return 1;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_DeInit_Gpio8080_8b(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD
  //
  // PARAMETERS:  unsigned short value    variable to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_SendDataWord_Gpio8080_8b(unsigned short value)
  {
    OUTPUT_ALL(D4DLCD_DATA);
    D4DLCD_ASSERT_CS;

    D4DLCD_DATA_WRITE((unsigned char) ((value >> 8) & 0xff)); // msb!
    D4DLCD_ASSERT_WR;
    D4DLCD_DEASSERT_WR;

    D4DLCD_DATA_WRITE((unsigned char) (value & 0xff));           // lsb!
    D4DLCD_ASSERT_WR;
    D4DLCD_DEASSERT_WR;

    D4DLCD_DEASSERT_CS;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendCmdWord_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit command into LCD
  //
  // PARAMETERS:  unsigned short cmd    command to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_SendCmdWord_Gpio8080_8b(unsigned short cmd)
  {
    D4DLCD_ASSERT_DC;                        // DataCmd := 0
    D4DLCDHW_SendDataWord_Gpio8080_8b(cmd);
    D4DLCD_DEASSERT_DC;                      // DataCmd := 1
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadDataWord_Gpio8080_8b(void)
  {
    unsigned short value;

    INPUT_ALL(D4DLCD_DATA);
    D4DLCD_DEASSERT_DC;
    D4DLCD_ASSERT_CS;

    D4DLCD_ASSERT_RD;
    D4DLCD_DEASSERT_RD;
    value = D4DLCD_DATA;     // msb!

    D4DLCD_ASSERT_RD;
    D4DLCD_DEASSERT_RD;
    value |= (unsigned short) ((D4DLCD_DATA << 8) & 0xff00); // msb!

    D4DLCD_DEASSERT_CS;

    return value;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadCmdWord_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadCmdWord_Gpio8080_8b(void)
  {
    unsigned short tmp_res;
    D4DLCD_ASSERT_DC;                        // DataCmd := 0
    tmp_res = D4DLCDHW_ReadDataWord_Gpio8080_8b();
    D4DLCD_DEASSERT_DC;                      // DataCmd := 1

    return tmp_res;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_Gpio8080_8b(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
    switch(pinId)
    {
      case D4DLCD_RESET_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DLCD_RESET);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DLCD_RESET);
            break;
          case D4DHW_PIN_SET_1:
            D4DLCD_DEASSERT_RESET;
            break;
          case D4DHW_PIN_SET_0:
            D4DLCD_ASSERT_RESET;
            break;
        }
        break;
      case D4DLCD_BACKLIGHT_PIN:
        switch(setState)
        {

          #ifdef D4DLCD_BACKLIGHT

          case D4DHW_PIN_OUT:
            OUTPUT(D4DLCD_BACKLIGHT);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DLCD_BACKLIGHT);
            break;
          case D4DHW_PIN_SET_1:
            D4DLCD_DEASSERT_BACKLIGHT;
            break;
          case D4DHW_PIN_SET_0:
            D4DLCD_ASSERT_BACKLIGHT;
            break;

         #endif

        }
        break;

      case D4DLCD_CS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_SET_1:
            D4DLCD_DEASSERT_CS;
            break;
          case D4DHW_PIN_SET_0:
            D4DLCD_ASSERT_CS;
            break;
        }
        break;

    }
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_Gpio8080_8b
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_Gpio8080_8b(D4DLCD_FLUSH_MODE mode)
  {
    D4D_UNUSED(mode);
  }

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_gpio8080_8b_ID)
