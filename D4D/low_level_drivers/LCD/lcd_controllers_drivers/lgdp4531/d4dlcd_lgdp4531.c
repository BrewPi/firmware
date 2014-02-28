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
* @file      d4dlcd_lgdp4531.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - lgdp4531 lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_lgdp4531_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_lgdp4531_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_lgdp4531_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\LCD\lcd_controllers_drivers\lgdp4531\d4dlcd_lgdp4531.h"
  #include "low_level_drivers\LCD\lcd_hw_interface\common_drivers\d4dlcdhw_common.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/
  #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    #error The eGUI low level driver "d4dlcd_lgdp4531" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
  #endif
  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCD_Init_LGDP4531(void);
  static unsigned char D4DLCD_SetWindow_LGDP4531(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_LGDP4531(D4DLCD_ORIENTATION new_orientation);
  static void D4DLCD_Send_PixelColor_LGDP4531(D4D_COLOR value) ;
  static D4D_COLOR D4DLCD_Get_PixelColor_LGDP4531(void);
  static void D4DLCD_Flush_LGDP4531(D4DLCD_FLUSH_MODE mode);
  static unsigned char D4DLCD_DeInit_LGDP4531(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_lgdp4531 =
  {
    D4DLCD_Init_LGDP4531,
    D4DLCD_SetWindow_LGDP4531,
    D4DLCD_SetOrientation_LGDP4531,
    D4DLCD_Send_PixelColor_LGDP4531,
    D4DLCD_Get_PixelColor_LGDP4531,
    D4DLCD_Flush_LGDP4531,
    D4DLCD_Delay_ms_Common,
    D4DLCD_DeInit_LGDP4531,
  };

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  const unsigned int init_data1[] =
  {
      0x0010,  0x0628, 0x0012,  0x0006, 0x0013,  0x0A32, 0x0011,  0x0040, 0x0015,  0x0050,
      0x0012,  0x0016,
      // Delay 15ms
  };
  const unsigned int init_data2[] =
  {
      0x0010,  0x5660,
      // Delay 15ms
  };
  const unsigned int init_data3[] =
  {
      0x0013,  0x2A4E, 0x0001,  0x0100, 0x0002,  0x0300, 0x0003,  0x1038, 0x0008,  0x0202,
      0x000A,  0x0000, 0x0030,  0x0000, 0x0031,  0x0402, 0x0032,  0x0106, 0x0033,  0x0700,
      0x0034,  0x0104, 0x0035,  0x0301, 0x0036,  0x0707, 0x0037,  0x0305, 0x0038,  0x0208,
      0x0039,  0x0F0B,
      // Delay 15ms
  };
  const unsigned int init_data4[] =
  {
      0x0041,  0x0002, 0x0060,  0x2700, 0x0061,  0x0001, 0x0090,  0x0119, 0x0092,  0x010A,
      0x0093,  0x0004, 0x00A0,  0x0100, 0x0007,  0x0001,
      // Delay 15ms
  };
  const unsigned int init_data5[] =
  {
      0x0007,  0x0021,
      // Delay 15ms
  };
  const unsigned int init_data6[] =
  {
      0x0007,  0x0023,
      // Delay 15ms
  };
  const unsigned int init_data7[] =
  {
      0x0007,  0x0033,
      // Delay 15ms
  };
  const unsigned int init_data8[] =
  {
      0x0007,  0x0133,
      // Delay 15ms
  };
  const unsigned int init_data9[] =
  {
      0x00A0,  0x0000,
      // Delay 20ms
  };

  static D4DLCD_ORIENTATION d4dlcd_orientation = Portrait;

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/
  // sends Command/Data sequence word (16 bit) or array with words - first 1 command, then 1 x data
  static void D4DLCD_Index_Data(const unsigned int data[], unsigned int count)
  {
    int i;

    for (i=0; i<count; i+=2)
    {
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned short)data[i]);
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)data[i+1]);
    }
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Init_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Init_LGDP4531(void)
  {

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

    // Initial synchronization transfers
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0);

    D4DLCD_Index_Data(init_data1, 12);   D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data2, 2);    D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data3, 32);   D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data4, 16);   D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data5, 2);    D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data6, 2);    D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data7, 2);    D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data8, 2);    D4DLCD_Delay_ms_Common(15);
    D4DLCD_Index_Data(init_data9, 2);    D4DLCD_Delay_ms_Common(20);



    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_DeInit_LGDP4531(void)
  {

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function sets the logic window in memory of LCD driver
  //
  // PARAMETERS:  unsigned short x1 - left cordination of logic window
  //              unsigned short y1 - top cordination of logic window
  //              unsigned short x2 - right cordination of logic window
  //              unsigned short y2 - bottom cordination of logic window
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetWindow_LGDP4531(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
    unsigned short Addr1, Addr2;

    switch (d4dlcd_orientation)
    {
        default:
            // Invalid! Fall through to portrait mode
        case Portrait:
            Addr1 = x1;
            Addr2 = y1;
            break;

        case Portrait180:
            Addr1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - x1);
            Addr2 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - y1);
            x1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - x2);
            x2 = Addr1;    // pack X-Values into one word

            y1 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - y2);
            y2 = Addr2;
            break;

        case Landscape:
            Addr1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y1);
            Addr2 = x1;
            y1 = x1;
            x1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y2);
            y2 = x2;
            x2 = Addr1;
            break;

        case Landscape180:
            Addr1 = y1;
            Addr2 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x1);    // pack X-Values into one word
            x1 = y1;
            y1 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x2);
            x2 = y2;
            y2 = Addr2;
            break;
    }

    //Set Window
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0050);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)x1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0051);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)x2);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0052);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)y1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0053);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)y2);

   // Set Start Address counter

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0020);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)Addr1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0021);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)Addr2);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0022);

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_LGDP4531(D4DLCD_ORIENTATION new_orientation)
  {
    unsigned short LCD_EntryMode;

    d4dlcd_orientation = new_orientation;

    switch (d4dlcd_orientation)
    {
        default: // Invalid! Fall through to portrait mode
        case Portrait:
            LCD_EntryMode = 0x1030; break;  // &B000100000110000
        case Portrait180:
            LCD_EntryMode = 0x1000; break;  // &B000100000000000
        case Landscape:
            LCD_EntryMode = 0x1028; break;  // &B000100000101000
        case Landscape180:
            LCD_EntryMode = 0x1018; break;  // &B000100000011000
    }
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0003);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(LCD_EntryMode);

    (void)D4DLCD_SetWindow_LGDP4531(0, 0, 1, 1);

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Send_PixelColor_LGDP4531(D4D_COLOR value)
  {
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(value);
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Get_PixelColor_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static D4D_COLOR D4DLCD_Get_PixelColor_LGDP4531(void)
  {
    return D4D_LLD_LCD_HW.D4DLCDHW_ReadDataWord();
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_LGDP4531
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_LGDP4531(D4DLCD_FLUSH_MODE mode)
  {
    D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer(mode);
  }



#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_lgdp4531_ID)
