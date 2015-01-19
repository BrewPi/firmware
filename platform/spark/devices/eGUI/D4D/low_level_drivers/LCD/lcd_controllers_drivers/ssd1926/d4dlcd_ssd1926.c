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
* @file      d4dlcd_ssd1926.c
*
* @author     Petr Gargulak
*
* @version   0.0.9.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - ssd1926 lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_ssd1926_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_ssd1926_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ssd1926_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\LCD\lcd_controllers_drivers\ssd1926\d4dlcd_ssd1926.h"
  #include "low_level_drivers\LCD\lcd_hw_interface\common_drivers\d4dlcdhw_common.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  #ifndef HORIZONTAL_RES
    #define HORIZONTAL_RES 240
  #endif

  #ifndef VERTICAL_RES
    #define VERTICAL_RES 320
  #endif

  #ifndef HORIZONTAL_PULSE_WIDTH
    #define HORIZONTAL_PULSE_WIDTH 25
  #endif

  #ifndef HORIZONTAL_BACK_PORCH
    #define HORIZONTAL_BACK_PORCH 8
  #endif

  #ifndef HORIZONTAL_FRONT_PORCH
    #define HORIZONTAL_FRONT_PORCH 8
  #endif

  #ifndef VERTICAL_PULSE_WIDTH
    #define VERTICAL_PULSE_WIDTH 25
  #endif

  #ifndef VERTICAL_BACK_PORCH
    #define VERTICAL_BACK_PORCH 8
  #endif

  #ifndef VERTICAL_FRONT_PORCH
    #define VERTICAL_FRONT_PORCH 8
  #endif



  #define HORIZONTAL_TOTAL (HORIZONTAL_PULSE_WIDTH + HORIZONTAL_BACK_PORCH + HORIZONTAL_FRONT_PORCH + HORIZONTAL_RES) // HT register

  #define HORIZONTAL_DISPLAY_PERIOD_START (HORIZONTAL_PULSE_WIDTH + HORIZONTAL_BACK_PORCH) // HDPS

  #define VERTICAL_TOTAL (VERTICAL_PULSE_WIDTH + VERTICAL_BACK_PORCH + VERTICAL_FRONT_PORCH + VERTICAL_RES) // VT register

  #define VERTICAL_DISPLAY_PERIOD_START (VERTICAL_PULSE_WIDTH + VERTICAL_BACK_PORCH) // VDPS


  /******************************************************************************
  * Types
  ******************************************************************************/

  typedef struct
  {
    unsigned short address;
    unsigned char data;
  }D4DLCD_REGISTER;

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCD_Init_SSD1926(void);
  static unsigned char D4DLCD_SetWindow_SSD1926(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_SSD1926(D4DLCD_ORIENTATION new_orientation);
  static unsigned char D4DLCD_Send_PixelColor_SSD1926(unsigned short value) ;
  static unsigned short D4DLCD_Read_PixelColor_SSD1926(void);
  static void D4DLCD_Flush_SSD1926(D4DLCD_FLUSH_MODE mode);
  static unsigned char D4DLCD_DeInit_SSD1926(void);

  static void D4DLCD_SetRegister(D4DLCD_REGISTER* pReg);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_ssd1926 =
  {
    D4DLCD_Init_SSD1926,
    D4DLCD_SetWindow_SSD1926,
    D4DLCD_SetOrientation_SSD1926,
    D4DLCD_Send_PixelColor_SSD1926,
    D4DLCD_Read_PixelColor_SSD1926,
    D4DLCD_Flush_SSD1926,
    D4DLCD_Delay_ms_Common,
    D4DLCD_DeInit_SSD1926,
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

  static void D4DLCD_SetRegister(D4DLCD_REGISTER* pReg)
  {
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned char)(pReg->address >> 8));
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned char)(pReg->address));
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned char)(pReg->data));
  }


  // sends Command/Data sequence word (16 bit) or array with words - first 1 command, then 1 x data
  static void D4DLCD_Command_Data(const unsigned int data[], unsigned int count)
  {
    int i;

    for (i=0; i<count; i+=2)
    {
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned short)0x00);
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned short)data[i]>>8);
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord((unsigned short)data[i]);
      //(void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)data[i+1]>>8);
      (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)data[i+1]);
    }
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Init_SSD1926
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Init_SSD1926(void)
  {

    const D4DLCD_REGISTER ssd1926_reg_default[] =
    {
      // PLL Init
      { REG_PLL_CONFIG_0, 0x0a},
      { REG_PLL_CONFIG_1, 0xc8},
      { REG_PLL_CONFIG_2, 0xae},
      { REG_PLL_CONFIG_0, 0x8a},
      // Video buffer setup
      { REG_MEMCLK_CONFIG, 0x00},
      // Pixel output clock setup
      { REG_PCLK_FREQ_RATIO_0, 0x00},
      { REG_PCLK_FREQ_RATIO_1, 0x00},
      { REG_PCLK_FREQ_RATIO_2, 0x02},
      // Panel type 18bit 0x61 / 24bit 0x71
      { REG_PANEL_TYPE,0x61},
      // Horizontal total
      { REG_HORIZ_TOTAL_0, (HORIZONTAL_TOTAL / 8)},
      { REG_HORIZ_TOTAL_1, (HORIZONTAL_TOTAL % 8)},
      // horizontal display period
      { REG_HDP, (HORIZONTAL_RES / 8) - 1},
      // Horizontal Display Period Start Position
      { REG_HDP_START_POS0, (unsigned char)(HORIZONTAL_DISPLAY_PERIOD_START)},
      { REG_HDP_START_POS1, (unsigned char)(HORIZONTAL_DISPLAY_PERIOD_START>>8)},
      // horizontal synchronization pulse
      { REG_HSYNC_PULSE_WIDTH, (HORIZONTAL_PULSE_WIDTH - 1)},
      // Vertical total
      { REG_VERT_TOTAL0, (VERTICAL_TOTAL / 8)},
      { REG_VERT_TOTAL1, (VERTICAL_TOTAL % 8)},
      // vertical display period
      { REG_VDP0, (VERTICAL_RES - 1) / 8},
      { REG_VDP1, (VERTICAL_RES - 1) % 8},
      // Vertical Display Period Start Position
      { REG_VDP_START_POS0, (unsigned char)(VERTICAL_DISPLAY_PERIOD_START)},
      { REG_VDP_START_POS1, (unsigned char)(VERTICAL_DISPLAY_PERIOD_START>>8)},
      // vertical synchronization pulse
      { REG_VSYNC_PULSE_WIDTH, (VERTICAL_PULSE_WIDTH - 1)},
      // special effect register - normal rotation
      { REG_SPECIAL_EFFECTS, 0x40},
      // Main window start address
      { REG_MAIN_WIN_DISP_START_ADDR0, 0x00},
      { REG_MAIN_WIN_DISP_START_ADDR1, 0x00},
      { REG_MAIN_WIN_DISP_START_ADDR2, 0x00},
      // Main Window offset
      { REG_MAIN_WIN_ADDR_OFFSET0, (unsigned char)(HORIZONTAL_RES / 2)},
      { REG_MAIN_WIN_ADDR_OFFSET1, (unsigned char)((HORIZONTAL_RES / 2)>>8)},
      // display mode 16bpp
      { REG_DISPLAY_MODE, 0x04},
      // rgb settings
      { REG_RGB_SETTING, 0xc0},
      // power managment
      { REG_POWER_SAVE_CONFIG, 0x00},
      // invert
      //{ REG_LSHIFT_POLARITY, 0x01},
      // GPIO control
      { REG_GPIO_STATUS_CONTROL1, 0x80}
    };


    int i;

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

    D4DLCD_Delay_ms_Common(400);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);   D4DLCD_Delay_ms_Common(75);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_1);   D4DLCD_Delay_ms_Common(500);

    // set up all register to defaults states !
    for (i=0; i<(sizeof(ssd1926_reg_default) / sizeof(D4DLCD_REGISTER)); i++)
      D4DLCD_SetRegister((D4DLCD_REGISTER*)(&ssd1926_reg_default[i]));

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_SSD1926
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_DeInit_SSD1926(void)
  {

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_SSD1926
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
  static unsigned char D4DLCD_SetWindow_SSD1926(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
    unsigned short Addr1, Addr2;

    switch (d4dlcd_orientation)
    {
        default:
            // Invalid! Fall through to portrait mode
        case Portrait:
            Addr1 = x1;
            Addr2 = y1;
            //x1_x2 = (unsigned short)((x2<<8) + x1);   // pack X-Values into one word
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
            x1 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y2);
            x2 = (unsigned short)(D4D_SCREEN_SIZE_SHORTER_SIDE - 1 - y1);
            y1 = x1;
            y2 = x2;
            break;

        case Landscape180:
            Addr1 = y1;
            Addr2 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x1);    // pack X-Values into one word
            x1 = y1;
            x2 = y2;
            y1 = (unsigned short)(D4D_SCREEN_SIZE_LONGER_SIDE - 1 - x2);
            y2 = Addr2;
            break;
    }

        /*
      // ROTATION MODE
      #if (DISP_ORIENTATION == 0)

      #define  WIN_START_ADDR		0ul
      #define  ROTATION           0

      #elif (DISP_ORIENTATION == 90)

      #define  WIN_START_ADDR		( ((DWORD)GetMaxX()+1) / 2 - 1 )
      #define  ROTATION           1

      #elif (DISP_ORIENTATION == 180)

      #define  WIN_START_ADDR		( ( ((DWORD)GetMaxX()+1)*(GetMaxY()+1) )/2-1 )
      #define  ROTATION           2

      #elif (DISP_ORIENTATION == 270)

      #define  WIN_START_ADDR		( ( ((DWORD)GetMaxX()+1)* GetMaxY() )/2 )
      #define  ROTATION           3

      #endif
    */


    //Set Window
   /*
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0050);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)x1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0051);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)x2);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0052);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)y1);

    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0053);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)y2);
     */
   // Set Start Address counter

 //   (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x004e);
 //   (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)Addr1);

 //   (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x004f);
 //   (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((unsigned short)Addr2);

   // (void)D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x0022);

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_SSD1926
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_SSD1926(D4DLCD_ORIENTATION new_orientation)
  {
    D4DLCD_REGISTER lcd_reg = { REG_SPECIAL_EFFECTS, 0x40};

    d4dlcd_orientation = new_orientation;


    switch (d4dlcd_orientation)
    {
        default: // Invalid! Fall through to portrait mode
        case Portrait:
            break;
        case Portrait180:
            lcd_reg.data |= 0b10; break;
        case Landscape:
            lcd_reg.data |= 0b01; break;
        case Landscape180:
            lcd_reg.data |= 0b11; break;
    }

    D4DLCD_SetRegister(&lcd_reg);

    (void)D4DLCD_SetWindow_SSD1926(0, 0, 1, 1);

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_SSD1926
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Send_PixelColor_SSD1926(unsigned short value)
  {
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(value>>8);
    (void)D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(value);
    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Read_PixelColor_SSD1926
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCD_Read_PixelColor_SSD1926(void)
  {

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_SSD1926
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_SSD1926(D4DLCD_FLUSH_MODE mode)
  {
     D4D_UNUSED(mode);
  }



#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ssd1926_ID)
