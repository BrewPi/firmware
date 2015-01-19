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
* @file      d4dlcd_frame_buffer.c
*
* @author     Petr Gargulak
*
* @version   0.0.13.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - frame_buffer lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcd_frame_buffer_ID 1


// copilation enable preprocessor condition
// the string d4dlcd_frame_buffer_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_frame_buffer_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\LCD\lcd_controllers_drivers\frame_buffer\d4dlcd_frame_buffer.h"

  #include "low_level_drivers\LCD\lcd_hw_interface\common_drivers\d4dlcdhw_common.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCD_Init_FrameBuffer(void);
  static unsigned char D4DLCD_SetWindow_FrameBuffer(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
  static unsigned char D4DLCD_SetOrientation_FrameBuffer(D4DLCD_ORIENTATION new_orientation);
  static void D4DLCD_Send_PixelColor_FrameBuffer(D4D_COLOR value) ;
  static D4D_COLOR D4DLCD_Read_PixelColor_FrameBuffer(void);
  static void D4DLCD_Flush_FrameBuffer(D4DLCD_FLUSH_MODE mode);
  static unsigned char D4DLCD_DeInit_FrameBuffer(void);

	static void D4DLCD_ComputeCurAdrr(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCD_FUNCTIONS d4dlcd_frame_buffer =
  {
    D4DLCD_Init_FrameBuffer,
    D4DLCD_SetWindow_FrameBuffer,
    D4DLCD_SetOrientation_FrameBuffer,
    D4DLCD_Send_PixelColor_FrameBuffer,
    D4DLCD_Read_PixelColor_FrameBuffer,
    D4DLCD_Flush_FrameBuffer,
    D4DLCD_Delay_ms_Common,
    D4DLCD_DeInit_FrameBuffer,
  };

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

	static unsigned long  win_cur_addr;
	static unsigned long win_cur_w;
	static unsigned long win_cur_h;

	static unsigned long win_x;
	static unsigned long win_y;
	static unsigned long win_width;
	static unsigned long win_height;

	static signed long win_const1;
	static signed long win_const2;
	static signed long win_const3;

	static unsigned long pix_cnt;

	static unsigned long bpp_byte;


	static D4DLCD_ORIENTATION lcd_orient = Portrait;
	static D4DLCD_ORIENTATION hw_orient  = Portrait;
	static D4DLCD_FRAMEBUFF_DESC* p_fbDesc = NULL;


  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Init_FrameBuffer
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_Init_FrameBuffer(void)
  {
  	if(!D4D_LLD_LCD_HW.D4DLCDHW_Init())
  		return 0;

  	p_fbDesc = D4D_LLD_LCD_HW.D4DLCDHW_GetFbDescriptor();

  	if(p_fbDesc == NULL)
  		return 0;

  	win_cur_addr = p_fbDesc->fb_start_addr;
  	win_width = 0;
  	win_height = 0;

  	if(p_fbDesc->lcd_x_max < p_fbDesc->lcd_y_max)
  	{
  		hw_orient  = Portrait;
  	}
  	else
  	{
  		hw_orient  = Landscape;
  	}

		pix_cnt = (unsigned long)(p_fbDesc->lcd_x_max * p_fbDesc->lcd_y_max);

		bpp_byte = p_fbDesc->bpp_byte;

  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_DeInit_FrameBuffer
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_DeInit_FrameBuffer(void)
  {
  	return D4D_LLD_LCD_HW.D4DLCDHW_DeInit();
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetWindow_FrameBuffer
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
  static unsigned char D4DLCD_SetWindow_FrameBuffer(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
  {
		win_x = x1;
		win_y = y1;

		win_width = (unsigned short)(x2 - x1);
		win_height = (unsigned short)(y2 - y1);

		win_cur_w = win_width;
		win_cur_h = win_height;

		switch(lcd_orient)
  	{
  		case Portrait:
				win_const1 = (signed long)(p_fbDesc->fb_start_addr + (p_fbDesc->lcd_x_max * win_y + win_x) * bpp_byte);
				win_const2 = (signed long)((p_fbDesc->lcd_x_max - win_width) * bpp_byte);
				win_const3 = (signed long)(bpp_byte);
  			break;

  		case Portrait180:
				win_const1 = (signed long)((p_fbDesc->fb_start_addr + (pix_cnt * bpp_byte)) - (p_fbDesc->lcd_x_max * win_y + win_x) * bpp_byte);
				win_const2 = (signed long)((p_fbDesc->lcd_x_max - win_width) * -1 * bpp_byte);
				win_const3 = (signed long)(-1 * bpp_byte);
				break;

  		case Landscape180:
				win_const1 = (signed long)(p_fbDesc->fb_start_addr + (p_fbDesc->lcd_x_max * win_x + p_fbDesc->lcd_x_max - win_y) * bpp_byte);
				win_const2 = (signed long)((win_width * p_fbDesc->lcd_x_max + 1) * -1 * bpp_byte);
				win_const3 = (signed long)((p_fbDesc->lcd_x_max) * bpp_byte);
  			break;

  		case Landscape:
				win_const1 = (signed long)((p_fbDesc->fb_start_addr + (pix_cnt * bpp_byte)) - (p_fbDesc->lcd_x_max * win_x + p_fbDesc->lcd_x_max - win_y) * bpp_byte);
				win_const2 = (signed long)((win_width * p_fbDesc->lcd_x_max + 1) * bpp_byte);
				win_const3 = (signed long)((p_fbDesc->lcd_x_max) * -1 * bpp_byte);
  			break;

  	}


		D4DLCD_ComputeCurAdrr();
              //  SIM_MCR &= ~SIM_MCR_LCDSTART_MASK;  //Start LCDC
              //  SIM_SCGC3 &= ~SIM_SCGC3_LCDC_MASK;
        return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_SetOrientation_FrameBuffer
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function set the new orientation of LCD
  //
  // PARAMETERS:  D4DLCD_ORIENTATION new_orientation    the requested new orientation
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCD_SetOrientation_FrameBuffer(D4DLCD_ORIENTATION new_orientation)
  {

  	if(hw_orient == Portrait)
  		lcd_orient = new_orientation;
  	else
  	{
  		switch(new_orientation)
  		{
  			case Portrait:
  				lcd_orient = Landscape;
  				break;
  		        case Portrait180:
  				lcd_orient = Landscape180;
  				break;
  			case Landscape:
  				lcd_orient = Portrait;
  				break;
  			case Landscape180:
  				lcd_orient = Portrait180;
  				break;
  		}
  	}
  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Send_PixelColor_FrameBuffer
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one pixel (Word) into LCD
  //
  // PARAMETERS:  unsigned short Value    value of pixel color
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Send_PixelColor_FrameBuffer(D4D_COLOR value)
  {
  	#if D4DLCDHWFB_DIRECT_MEM_ACCESS == 1

#ifdef D4D_COLOR_TRANSPARENT
        if(value != D4D_COLOR_TRANSPARENT)
#endif
  	  *((D4D_COLOR*)win_cur_addr) = value;
  	D4DLCD_ComputeCurAdrr();

  	#else
#ifdef D4D_COLOR_TRANSPARENT
        if(value != D4D_COLOR_TRANSPARENT)
#endif
   	      D4D_LLD_LCD_HW.D4DLCDHW_WriteData(win_cur_addr, value);
  	D4DLCD_ComputeCurAdrr();

  	#endif
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Read_PixelColor_FrameBuffer
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one Word(Pixel) from LCD (if this function is supported)
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static D4D_COLOR D4DLCD_Read_PixelColor_FrameBuffer(void)
  {
  	D4D_COLOR value;

  	#if D4DLCDHWFB_DIRECT_MEM_ACCESS == 1
			value = *((D4D_COLOR*)win_cur_addr);
			D4DLCD_ComputeCurAdrr();
			return value;
  	#else
			value = D4D_LLD_LCD_HW.D4DLCDHW_ReadData(win_cur_addr);
			D4DLCD_ComputeCurAdrr();
			return value;
  	#endif
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_Flush_FrameBuffer
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_Flush_FrameBuffer(D4DLCD_FLUSH_MODE mode)
  {
		D4D_LLD_LCD_HW.D4DLCDHW_FlushBuffer(mode);
             //   SIM_MCR|=SIM_MCR_LCDSTART_MASK;  //Start LCDC
                //SIM_SCGC3 |= SIM_SCGC3_LCDC_MASK;
  }


  static void D4DLCD_ComputeCurAdrr(void)
  {
  	win_cur_w++;
		if(win_cur_w > win_width)
		{
			win_cur_w = 0;
			win_cur_h++;
			if(win_cur_h > win_height)
			{
				win_cur_h = 0;
				win_cur_addr = (unsigned long)(win_const1);
			}
			else
				win_cur_addr += win_const2;
		}
		else
			win_cur_addr += win_const3;
  }


#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_frame_buffer_ID)
