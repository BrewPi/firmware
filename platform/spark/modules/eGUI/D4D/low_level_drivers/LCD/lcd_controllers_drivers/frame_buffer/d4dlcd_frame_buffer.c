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


// #define D4DLCDHWFB_DIRECT_MEM_ACCESS 1

// compilation enable preprocessor condition
// the string d4dlcd_frame_buffer_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_frame_buffer_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.h"

  #include "low_level_drivers/LCD/lcd_hw_interface/common_drivers/d4dlcdhw_common.h"
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

	static void D4DLCD_Advance(void);

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

  	  WriteRegion writeRegion;

	static int32_t win_home_addr;
	static int32_t win_y_inc;
	static int32_t win_x_inc;

	static int32_t pix_cnt;

	static int8_t bpp_byte;


	static D4DLCD_ORIENTATION lcd_orient = Portrait;
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

  	writeRegion.win_cur_addr = p_fbDesc->fb_start_addr;
  	writeRegion.max_width = p_fbDesc->lcd_x_max;
  	writeRegion.max_height = p_fbDesc->lcd_y_max;
  	writeRegion.win_width = 0;
  	writeRegion.win_height = 0;

  	if(p_fbDesc->lcd_x_max < p_fbDesc->lcd_y_max)
  	{
  		lcd_orient  = Portrait;
  	}
  	else
  	{
  		lcd_orient  = Landscape;
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

  uint32_t address_for(uint16_t x, uint16_t y)
  {
	  return (p_fbDesc->fb_start_addr +
			  (((p_fbDesc->lcd_x_max * y) + x) * bpp_byte));
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
	  writeRegion.win_x = x1;
	  writeRegion.win_y = y1;
	  writeRegion.win_cur_w = 0;
	  writeRegion.win_cur_h = 0;
	  writeRegion.win_width = (unsigned short)(x2 - x1);
	  writeRegion.win_height = (unsigned short)(y2 - y1);

	switch(lcd_orient)
  	{
  		case Landscape:
			win_home_addr = address_for(x1, y1);
			win_y_inc = (writeRegion.max_width - writeRegion.win_width) * bpp_byte;
			win_x_inc = (bpp_byte);
  			break;

  		case Landscape180:
  			win_home_addr = (signed long)((p_fbDesc->fb_start_addr + (pix_cnt * bpp_byte)) - (p_fbDesc->lcd_x_max * writeRegion.win_y + writeRegion.win_x) * bpp_byte);
			win_y_inc = (signed long)((p_fbDesc->lcd_x_max - writeRegion.win_width) * -1 * bpp_byte);
			win_x_inc = (signed long)(-1 * bpp_byte);
			break;

  		case Portrait180:
  			win_home_addr = (signed long)(p_fbDesc->fb_start_addr + (p_fbDesc->lcd_x_max * writeRegion.win_x + p_fbDesc->lcd_x_max - writeRegion.win_y) * bpp_byte);
			win_y_inc = (signed long)((writeRegion.win_width * p_fbDesc->lcd_x_max + 1) * -1 * bpp_byte);
			win_x_inc = (signed long)((p_fbDesc->lcd_x_max) * bpp_byte);
  			break;

  		case Portrait:
  			win_home_addr = (signed long)((p_fbDesc->fb_start_addr + (pix_cnt * bpp_byte)) - (p_fbDesc->lcd_x_max * writeRegion.win_x + p_fbDesc->lcd_x_max - writeRegion.win_y) * bpp_byte);
			win_y_inc = (signed long)((writeRegion.win_width * p_fbDesc->lcd_x_max + 1) * bpp_byte);
			win_x_inc = (signed long)((p_fbDesc->lcd_x_max) * -1 * bpp_byte);
  			break;

  	}

	writeRegion.win_cur_addr = win_home_addr;

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
	lcd_orient = new_orientation;
	uint32_t width = writeRegion.max_width;
	uint32_t height = writeRegion.max_height;
	D4DLCD_SetWindow_FrameBuffer(0, 0, width, height);
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
  	  *((D4D_COLOR*)writeRegion.win_cur_addr) = value;

  	#else
#ifdef D4D_COLOR_TRANSPARENT
        if(value != D4D_COLOR_TRANSPARENT)
#endif
   	      D4D_LLD_LCD_HW.D4DLCDHW_WriteData(writeRegion.win_cur_addr, value);
  	#endif
        D4DLCD_Advance();

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
  	return 0;
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
  }


  static inline void D4DLCD_Advance()
  {
	  // cur_w, cur_h are relative to the current window region
	  	writeRegion.win_cur_w++;
		if(writeRegion.win_cur_w > writeRegion.win_width)
		{
			writeRegion.win_cur_w = 0;
			writeRegion.win_cur_h++;
			if(writeRegion.win_cur_h > writeRegion.win_height)
			{
				writeRegion.win_cur_h = 0;
				writeRegion.win_cur_addr = (win_home_addr);
			}
			else
				writeRegion.win_cur_addr += win_y_inc;
		}
		else
			writeRegion.win_cur_addr += win_x_inc;

  }


#endif //(D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_frame_buffer_ID)
