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
* @file      d4dlcd_FSA506.c
*
* @author     Petr Gargulak
*
* @version   0.0.19.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - fsa506 lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_fsa506_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_fsa506_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_fsa506_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\LCD\lcd_controllers_drivers\fsa506\d4dlcd_fsa506.h"
  #include "low_level_drivers\LCD\lcd_hw_interface\common_drivers\d4dlcdhw_common.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/
  #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    #error The eGUI low level driver "d4dlcd_fsa506" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
  #endif
  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCD_Init_FSA506(void);
  static unsigned char D4DLCD_SetWindow_FSA506(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_FSA506(D4DLCD_ORIENTATION new_orientation);
  static void D4DLCD_Send_PixelColor_FSA506(D4D_COLOR Value) ;
  static D4D_COLOR D4DLCD_Get_PixelColor_FSA506(void);
  static void D4DLCD_Flush_FSA506(D4DLCD_FLUSH_MODE mode);
  static unsigned char D4DLCD_DeInit_FSA506(void);

  static void FSA506_Command_Write(Byte Addr, Byte Data);
  static Byte FSA506_Command_Read(Byte Addr);
  static void FSA506_DisplayRAM_WriteDisable(void);
  static void FSA506_DisplayRAM_WriteEnable(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_fsa506 =
  {
    D4DLCD_Init_FSA506,
    D4DLCD_SetWindow_FSA506,
    D4DLCD_SetOrientation_FSA506,
    D4DLCD_Send_PixelColor_FSA506,
    D4DLCD_Get_PixelColor_FSA506,
    D4DLCD_Flush_FSA506,
    D4DLCD_Delay_ms_Common,
    D4DLCD_DeInit_FSA506,
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
  // FUNCTION:    D4DLCD_Init_FSA506
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Init_FSA506(void)
  {
    Byte temp_reg = 0;
    Word i = 0;

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

    D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_BACKLIGHT_PIN, D4DHW_PIN_SET_1);
    D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_BACKLIGHT_PIN, D4DHW_PIN_OUT);

    D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);
    D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_OUT);

    D4DLCD_Delay_ms_Common(300);
    D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_0);   D4DLCD_Delay_ms_Common(75);
    D4D_LLD_LCD_HW.D4DLCDHW_PinCtl(D4DLCD_RESET_PIN, D4DHW_PIN_SET_1);   D4DLCD_Delay_ms_Common(500);


     temp_reg = FSA506_Command_Read(0x40);
     FSA506_Command_Write(0x40,0x12);	/*[7:6] Reserved
                                              [5] PLL control pins to select out frequency range
										      0: 20MHz ~ 100MHz 1: 100MHz ~ 300MHz
										      [4] Reserved [3] Reserved
										      [2:1] Output Driving Capability
										      00: 4mA  01: 8mA  10: 12mA  11: 16mA
										      [0] Output slew rate
										      0: Fast 1: Slow
										    */

    temp_reg = FSA506_Command_Read(0x40);
		FSA506_Command_Write(0x41,0x01);	//Set PLL=40Mhz * (0x42) / (0x41)
		FSA506_Command_Write(0x42,0x01);	//0x41 [7:6] Reserved [5:0] PLL Programmable pre-divider, 6bit(1~63)
											//0x42 [7:6] Reserved [5:0] PLL Programmable loop divider, 6bit(1~63)

		FSA506_Command_Write(0x00,0x00); // MSB of horizontal start coordinate value
    FSA506_Command_Write(0x01,0x00); // LSB of horizontal start coordinate value
    FSA506_Command_Write(0x02,0x01); // MSB of horizontal end coordinate value
    FSA506_Command_Write(0x03,0x3F); // LSB of horizontal end coordinate value
    FSA506_Command_Write(0x04,0x00); // MSB of vertical start coordinate value
    FSA506_Command_Write(0x05,0x00); // LSB of vertical start coordinate value
    FSA506_Command_Write(0x06,0x00); // MSB of vertical end coordinate value
		FSA506_Command_Write(0x07,0xEF); // LSB of vertical end coordinate value

		FSA506_Command_Write(0x08,0x01); 	// MSB of input image horizontal resolution
		FSA506_Command_Write(0x09,0x40); 	// LSB of input image horizontal resolution
		FSA506_Command_Write(0x0a,0x00);	//[17:16] bits of memory write start address
		FSA506_Command_Write(0x0b,0x00);	//[15:8] bits of memory write start address
		FSA506_Command_Write(0x0c,0x00);	//[7:0] bits of memory write start address

		//FSA506_Command_Write(0x10,_Register_0x10_CLKSelect_Mod4+_Register_0x10_PanelSelect_Parallel+_Register_0x10_Blanking_On);
		FSA506_Command_Write(0x10,0x0d);

		                                    /* Please set
											  [7] Output data bits swap	  0: Normal 1:Swap
		                                      [6] Output test mode enable 0: disable 1: enable
											  [5:4] Serial mode data out bus selection
											        00: X_ODATA17 ~ X_ODATA12 active , others are set to zero
											        01: X_ODATA11 ~ X_ODATA06 active , others are set to zero
													10: X_ODATA05 ~ X_ODATA00 active , others are set to zero
													11: reserved
											  [3] Output data blanking
											         0: set output data to 0
											         1: Normal display
											  [2] Parallel or serial mode selection
											         0: serial data out
												     1: parallel data output[7:0] bits of memory write start address
			          						 [1:0] Output clock selection
											         00: system clock divided by 2
													 01: system clock divided by 4
													 10: system clock divided by 8
													 11: reserved */

	   FSA506_Command_Write(0x11,0x05);     /*[7] Reserved
	                                          [6:4] Even line of serial panel data out sequence or data bus order of parallel panel
											  000: RGB
											  001: RBG
											  010: GRB
											  011: GBR
											  100: BRG
											  101: BGR
											  Others: reserved
											  [3] Reversed
											  [2:0] Odd line of serial panel data out sequence
											  000: RGB
											  001: RBG
											  010: GRB
											  011: GBR
											  100: BRG
											  101: BGR
											  Others: reserved
											 */
     FSA506_Command_Write(0x12,0x00);     //[7:4] Reserved [3:0] MSB of output H sync. pulse start position
     FSA506_Command_Write(0x13,0x00);		  //[7:0] LSB of output H sync. pulse start position
	   FSA506_Command_Write(0x14,0x00);     //[7:4] Reserved [3:0] MSB of output H sync. pulse width

	   FSA506_Command_Write(0x15,0x10); 	  //[7:0] LSB of output H sync. pulse width
	   FSA506_Command_Write(0x16,0x00); 	  //[7:4] Reserved [3:0] MSB of output DE horizontal start position
	   FSA506_Command_Write(0x17,0x38); 	  //[7:0] LSB of output DE horizontal start position
	   FSA506_Command_Write(0x18,0x01); 	  //[7:4] Reserved [3:0] MSB of output DE horizontal active region in pixel
	   FSA506_Command_Write(0x19,0x40); 	  //[7:0] LSB of output DE horizontal active region in pixel

	   FSA506_Command_Write(0x1a,0x01); 	  //[7:4] Reserved [3:0] MSB of output H total in pixel
	   FSA506_Command_Write(0x1b,0xb8); 	  //[7:0] LSB of output H total in pixel

	   FSA506_Command_Write(0x1c,0x00); 	  //[7:4] Reserved [3:0] MSB of output V sync. pulse start position
	   FSA506_Command_Write(0x1d,0x00); 	  //[7:0] of output V sync. pulse start position

	   FSA506_Command_Write(0x1e,0x00); 	  //[7:4] Reserved [3:0] MSB of output V sync. pulse width
	   FSA506_Command_Write(0x1f,0x08); 	  //[7:0] LSB of output V sync. pulse width

	   FSA506_Command_Write(0x20,0x00); 	  //[7:4] Reserved [3:0] MSB of output DE vertical start position
	   FSA506_Command_Write(0x21,0x12); 	  //[7:0] LSB of output DE vertical start position

     FSA506_Command_Write(0x22,0x00); 	  //[7:4] Reserved [3:0] MSB of output DE vertical active region in line
	   FSA506_Command_Write(0x23,0xf0); 	  //[7:0] LSB of output DE vertical active region in line

	   FSA506_Command_Write(0x24,0x01); 	  //[7:4] Reversed [3:0] MSB of output V total in line
	   FSA506_Command_Write(0x25,0x09); 	  //[7:0] LSB of output V total in line

	   FSA506_Command_Write(0x26,0x00); 	  //[7:2] Reserved [1:0] [17:16] bits of memory read start address
	   FSA506_Command_Write(0x27,0x00);		  //[7:0] [15:8] bits of memory read start address
	   FSA506_Command_Write(0x28,0x00);		  //[7:0] [7:0] bits of memory read start address

	   FSA506_Command_Write(0x29,0x01);       //[7:1] Reversed [0] Load output timing related setting (H sync., V sync. and DE) to take effect

	   FSA506_Command_Write(0x2d,0x08);		  /* [7:4] Reserved
	                                             [3] Output pin X_DCON level control
												 [2] Output clock inversion	 0: Normal 1: Inverse
												 [1:0] Image rotate
												 00: 0¢X  01: 90¢X  10: 270¢X 11: 180¢X
 											 */

      FSA506_Command_Write(0x30,0x00);		 //[7:4] Reserved [3:0] MSB of image horizontal shift value
  	  FSA506_Command_Write(0x31,0x00);		 //[7:0] LSB of image horizontal shift value

  	  FSA506_Command_Write(0x32,0x00);		 //[7:4] Reserved [3:0] MSB of image vertical shift value
  	  FSA506_Command_Write(0x33,0x00);		 //[7:0] LSB of image vertical shift value

  	  FSA506_Command_Write(0x34,0x01);		 //[7:4] Reserved [3:0] MSB of image horizontal physical resolution in memory
  	  FSA506_Command_Write(0x35,0x40);		 //[7:0] LSB of image horizontal physical resolution in memory

  	  FSA506_Command_Write(0x36,0x00);		 //[7:4] Reserved [3:0] MSB of image vertical physical resolution in memory
  	  FSA506_Command_Write(0x37,0xF0);		 //[7:0] LSB of image vertical physical resolution in memory

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_FSA506
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_DeInit_FSA506(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_FSA506
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
  static unsigned char D4DLCD_SetWindow_FSA506(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
    FSA506_DisplayRAM_WriteDisable();
    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x00);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)((x1)>>8));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)(x1));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)((x2)>>8));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)(x2));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)(y1>>8));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)(y1));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)((y2)>>8));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)(y2));

    FSA506_DisplayRAM_WriteEnable();

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_FSA506
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_FSA506(D4DLCD_ORIENTATION new_orientation)
  {
    Byte data;

    switch (new_orientation)
      {
        default:
              // Invalid! Fall through to portrait mode
        case Portrait:
          data = 0x0A;
          break;
        case Portrait180:
          data = 0x09;
          break;
        case Landscape:
          data = 0x08;
          break;
        case Landscape180:
          data = 0x0B;
          break;
      }
    FSA506_DisplayRAM_WriteDisable();
    FSA506_Command_Write(0x2d,data);
    D4DLCD_SetWindow_FSA506(0, 0, D4D_SCREEN_SIZE_LONGER_SIDE - 1, D4D_SCREEN_SIZE_SHORTER_SIDE - 1);

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_FSA506
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Send_PixelColor_FSA506(D4D_COLOR Value)
  {
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)((Value)>>8));
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord((Byte)(Value));
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Get_PixelColor_FSA506
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static D4D_COLOR D4DLCD_Get_PixelColor_FSA506(void)
  {
     return  0;
  }

    //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_FSA506
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_FSA506(D4DLCD_FLUSH_MODE mode)
  {
     D4D_UNUSED(mode);
  }


  // NON API function

  static void FSA506_Command_Write(Byte Addr, Byte Data) {

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(Addr);
    D4D_LLD_LCD_HW.D4DLCDHW_SendDataWord(Data);

  }

  static Byte FSA506_Command_Read(Byte Addr) {

    D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(Addr);
    return (Byte)D4D_LLD_LCD_HW.D4DLCDHW_ReadCmdWord();
  }

  static void FSA506_DisplayRAM_WriteDisable(void)
  {

  	   D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0x80);

  }

  static void FSA506_DisplayRAM_WriteEnable(void)
  {

  	   D4D_LLD_LCD_HW.D4DLCDHW_SendCmdWord(0xc1);

  }


#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_fsa506_ID)
