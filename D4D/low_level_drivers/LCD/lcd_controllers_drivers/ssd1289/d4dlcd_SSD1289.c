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
* @file      d4dlcd_SSD1289.c
*
* @author     Petr Gargulak
*
* @version   0.0.39.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - ssd1289 lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_ssd1289_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_ssd1289_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ssd1289_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\LCD\lcd_controllers_drivers\ssd1289\d4dlcd_ssd1289.h"
  #include "low_level_drivers\LCD\lcd_hw_interface\common_drivers\d4dlcdhw_common.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCD_Init_SSD1289(void);
  static unsigned char D4DLCD_SetWindow_SSD1289(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_SSD1289(D4DLCD_ORIENTATION new_orientation);
  static void D4DLCD_Send_PixelColor_SSD1289(D4D_COLOR value) ;
  static D4D_COLOR D4DLCD_Get_PixelColor_SSD1289(void);
  static void D4DLCD_Flush_SSD1289(D4DLCD_FLUSH_MODE mode);
  static unsigned char D4DLCD_DeInit_SSD1289(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_ssd1289 =
  {
    D4DLCD_Init_SSD1289,
    D4DLCD_SetWindow_SSD1289,
    D4DLCD_SetOrientation_SSD1289,
    D4DLCD_Send_PixelColor_SSD1289,
    D4DLCD_Get_PixelColor_SSD1289,
    D4DLCD_Flush_SSD1289,
    D4DLCD_Delay_ms_Common,
    D4DLCD_DeInit_SSD1289,
  };

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/
  static D4DLCD_ORIENTATION d4dlcd_orientation = Portrait;
  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/
  // sends Command/Data sequence word (16 bit) or array with words - first 1 command, then 1 x data
  static void D4DLCD_Command_Data(const unsigned short data[], unsigned short count)
  {
    int i;

    for (i=0; i<count; i+=2)
    {
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(data[i]);
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(data[i+1]);
    }
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Init_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Init_SSD1289(void)
  {
    const unsigned short init_data[] =
    {
        0x0000,  0x0001,

        0x0003,  0xAEAC, 0x000C,  0x0007, 0x000D,  0x000F, 0x000E,  0x2900, 0x001E,  0x00B3,

        0x0001,  0x2B3F, 0x0002,  0x0600, 0x0010,  0x0000, 0x0011,  0x60B0,

        0x0005,  0x0000, 0x0006,  0x0000, 0x0016,  0xEF1C, 0x0017,  0x0003, 0x0007,  0x0233,
        0x000B,  0x5312, 0x000F,  0x0000,

        0x0041,  0x0000, 0x0042,  0x0000, 0x0048,  0x0000, 0x0049,  0x013F, 0x0044,  0xEF00,
        0x0045,  0x0000, 0x0046,  0x013F, 0x004A,  0x0000, 0x004B,  0x0000,

        0x0030,  0x0707, 0x0031,  0x0704, 0x0032,  0x0204, 0x0033,  0x0201, 0x0034,  0x0203,
        0x0035,  0x0204, 0x0036,  0x0204, 0x0037,  0x0502, 0x003A,  0x0302, 0x003B,  0x0500,
        0x0023 , 0x0000, 0x0024 , 0x0000
    }  ;

    if(D4D_LLD_LCD_HW.D4DLCDHW_Init == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_ReadCmdWord == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_PinCtl == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer == NULL)
      return 0;
    if(D4D_LLD_LCD_HW.D4DLCDHW_DeInit == NULL)
      return 0;


    if(!D4D_LLD_LCD_HW.D4DLCDHW_Init())
      return 0;

    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_BACKLIGHT_PIN, D4DHW_PIN_SET_1);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_BACKLIGHT_PIN, D4DHW_PIN_OUT);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_OUT);

    D4DLCD_Delay_ms_Common(300);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);   D4DLCD_Delay_ms_Common(75);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_1);   D4DLCD_Delay_ms_Common(500);

    D4DLCD_Command_Data(&init_data[0], 2);    D4DLCD_Delay_ms_Common(15);
    D4DLCD_Command_Data(&init_data[2], 10);   D4DLCD_Delay_ms_Common(15);
    D4DLCD_Command_Data(&init_data[12], 8);   D4DLCD_Delay_ms_Common(150);
    D4DLCD_Command_Data(&init_data[20], 14);  D4DLCD_Delay_ms_Common(20);
    D4DLCD_Command_Data(&init_data[34], 18);  D4DLCD_Delay_ms_Common(20);
    D4DLCD_Command_Data(&init_data[52], 24);  D4DLCD_Delay_ms_Common(31);


    return 1;

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_DeInit_SSD1289(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function sets the logic window in memory of LCD driver
  //
  // PARAMETERS:  unsigned int x1 - left cordination of logic window
  //              unsigned int y1 - top cordination of logic window
  //              unsigned int x2 - right cordination of logic window
  //              unsigned int y2 - bottom cordination of logic window
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetWindow_SSD1289(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
    unsigned short x1_x2;
    unsigned short Addr1, Addr2;

    switch (d4dlcd_orientation)
    {
        default:
            // Invalid! Fall through to portrait mode
        case Portrait:
            Addr1 = x1;
            Addr2 = y1;
            x1_x2 = (unsigned short)((x2<<8) + x1);   // pack X-Values into one word
            break;

        case Portrait180:
            Addr1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - x1);
            Addr2 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - y1);
            x1_x2 = (unsigned short)((Addr1<<8) + (D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - x2));    // pack X-Values into one word
            y1 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - y2);
            y2 = Addr2;
            break;

        case Landscape:
            Addr1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y1);
            Addr2 = x1;
            x1_x2 = (unsigned short)((Addr1<<8) + (D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y2));    // pack X-Values into one word
            y1 = x1;
            y2 = x2;
            break;

        case Landscape180:
            Addr1 = y1;
            Addr2 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x1);    // pack X-Values into one word
            x1_x2 = (unsigned short)((y2<<8) + y1);
            y1 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x2);
            y2 = Addr2;
            break;
    }

    //Set Window
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0044);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)x1_x2);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0045);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)y1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0046);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)y2);

   // Set Start Address counter

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x004e);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)Addr1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x004f);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)Addr2);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0022);

    // Dummy read to prepare possible erad operation
    //(void)D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_SSD1289(D4DLCD_ORIENTATION new_orientation)
  {
    unsigned short LCD_EntryMode = 0;

    d4dlcd_orientation = new_orientation;

    switch (d4dlcd_orientation)
    {
        default: // Invalid! Fall through to portrait mode
        case Portrait:
            LCD_EntryMode = 0x60b0; break;  // &B110000010110000
        case Portrait180:
            LCD_EntryMode = 0x6080; break;  // &B110000010000000
        case Landscape:
            LCD_EntryMode = 0x60a8; break;  // &B110000010101000
        case Landscape180:
            LCD_EntryMode = 0x6098; break;  // &B110000010011000
    }
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0011);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(LCD_EntryMode);

    (void)D4DLCD_SetWindow_SSD1289(0, 0, 1, 1);

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Send_PixelColor_SSD1289(D4D_COLOR value)
  {

#ifdef D4D_COLOR_TRANSPARENT
    if(value == D4D_COLOR_TRANSPARENT)
      D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
    else
#endif
    {
      #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
        Word color = D4D_COLOR_RGB565(D4D_COLOR_GET_R(value), D4D_COLOR_GET_G(value), D4D_COLOR_GET_B(value));
        D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(color);
      #else
        D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)value);
      #endif

    }
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Read_PixelColor_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static D4D_COLOR D4DLCD_Get_PixelColor_SSD1289(void)
  {
    #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
      Word color =  D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
      return D4D_COLOR_RGB(D4D_COLOR565_GET_R(color), D4D_COLOR565_GET_G(color), D4D_COLOR565_GET_B(color));
    #else
      return D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
    #endif
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_SSD1289
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_SSD1289(D4DLCD_FLUSH_MODE mode)
  {
    D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer(mode);
  }


#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ssd1289_ID)
