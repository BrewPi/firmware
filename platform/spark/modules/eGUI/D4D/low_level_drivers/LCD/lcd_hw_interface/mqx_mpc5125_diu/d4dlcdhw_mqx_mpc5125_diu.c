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
* @file      d4dlcdhw_mqx_mpc5125_diu.c
*
* @author     Petr Gargulak
*
* @version   0.0.8.0
*
* @date      Aug-6-2012
*
* @brief     D4D driver - mqx_mpc5125_diu hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_mqx_mpc5125_diu_ID 1


// copilation enable preprocessor condition
// the string d4dtch_mqx_mpc5125_diu_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_mpc5125_diu_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/mqx_mpc5125_diu/d4dlcdhw_mqx_mpc5125_diu.h"
  #include <mqx.h>
  /******************************************************************************
  * Macros
  ******************************************************************************/
  #if D4D_COLOR_SYSTEM != D4D_COLOR_SYSTEM_RGB565
    #error The eGUI low level driver "d4dlcdhw_mqx_mpc5125_diu" not supported selected type of D4D_COLOR_SYSTEM. To run this driver just select D4D_COLOR_SYSTEM_RGB565.
  #endif
  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_MqxMpc5125_DIU(void);
  static unsigned char D4DLCDHW_DeInit_MqxMpc5125_DIU(void);
  static void D4DLCDHW_WriteData_MqxMpc5125_DIU(unsigned long addr, unsigned short value);
  static unsigned short D4DLCDHW_ReadData_MqxMpc5125_DIU(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU(void);
  static unsigned char D4DLCDHW_PinCtl_MqxMpc5125_DIU(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_MqxMpc5125_DIU(D4DLCD_FLUSH_MODE mode);


  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHWFB_FUNCTIONS d4dlcdhw_mqx_mpc5125_diu =
  {
    D4DLCDHW_Init_MqxMpc5125_DIU,
    D4DLCDHW_WriteData_MqxMpc5125_DIU,
    D4DLCDHW_ReadData_MqxMpc5125_DIU,
    D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU,
    D4DLCDHW_PinCtl_MqxMpc5125_DIU,
    D4DLCD_FlushBuffer_MqxMpc5125_DIU,
    D4DLCDHW_DeInit_MqxMpc5125_DIU
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

	static D4DLCD_FRAMEBUFF_DESC d4dlcdhw_mqx_mpc5125_diu_desc;
	static DISPLAY_CONTEXT_STRUCT_PTR d4dlcdhw_gdc_ptr = NULL;
	static DISPLAY_PIXEL_BUFFER_PTR d4dlcdhw_pixbuff_ptr = NULL;
	static DISPLAY_VIEW_STRUCT_PTR  d4dlcdhw_view_ptr = NULL;
  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_Init_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_Init_MqxMpc5125_DIU(void)
  {
		char buffer[64];

		sprintf (buffer, "%dx%dx%d", D4DLCDHWFB_X, D4DLCDHWFB_Y, D4DLCDHWFB_REFRESH);
		d4dlcdhw_gdc_ptr = display_initialize(buffer);
		if(d4dlcdhw_gdc_ptr == NULL)
			return 0;




		d4dlcdhw_pixbuff_ptr = display_create_pixel_buffer(D4DLCDHWFB_X, D4DLCDHWFB_Y, RGB24, NULL);

		d4dlcdhw_mqx_mpc5125_diu_desc.bpp_byte = d4dlcdhw_pixbuff_ptr->BPP;
		d4dlcdhw_mqx_mpc5125_diu_desc.lcd_x_max = d4dlcdhw_pixbuff_ptr->X;
		d4dlcdhw_mqx_mpc5125_diu_desc.lcd_y_max = d4dlcdhw_pixbuff_ptr->Y;
		d4dlcdhw_mqx_mpc5125_diu_desc.fb_start_addr = (unsigned long) d4dlcdhw_pixbuff_ptr->FB;

  	d4dlcdhw_view_ptr = display_create_view(d4dlcdhw_gdc_ptr, d4dlcdhw_pixbuff_ptr);

  	if(d4dlcdhw_view_ptr == NULL)
  	{
  		D4DLCDHW_DeInit_MqxMpc5125_DIU();
  		return 0;
  	}

  	display_center_view(d4dlcdhw_view_ptr);
  	display_show_view(d4dlcdhw_view_ptr, DIU_PLANE_1);

  	return 1;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_DeInit_MqxMpc5125_DIU(void)
  {
		display_destroy_view(d4dlcdhw_view_ptr);
		display_destroy_pixel_buffer(d4dlcdhw_pixbuff_ptr);
		display_deinit(d4dlcdhw_gdc_ptr);
  	return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD
  //
  // PARAMETERS:  unsigned long addr  		address to write data
  //							unsigned short value    variable to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_WriteData_MqxMpc5125_DIU(unsigned long addr, unsigned short value)
  {


 	#if 0
 			unsigned char color;

 			color = D4D_COLOR_GET_R(value);
 			*((unsigned char*)addr) = color;

 			color = D4D_COLOR_GET_G(value);
 			*((unsigned char*)addr+1) = color;

 			color = D4D_COLOR_GET_B(value);
 			*((unsigned char*)addr+2) = color;
 #else
 			unsigned long color;

 			color = *((unsigned long*)addr);
 			color &= 0x000000ff;
 			color |= (D4D_COLOR_GET_R(value)) << 24;
  		color |= (D4D_COLOR_GET_G(value)) << 16;
 	 		color |= (D4D_COLOR_GET_B(value)) << 8;

			*((unsigned long*)addr) = color;
 #endif
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
  //
  // PARAMETERS:  unsigned long addr  		address to read data
  //
  // RETURNS:     unsigned short - the read value
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DLCDHW_ReadData_MqxMpc5125_DIU(unsigned long addr)
  {
  	unsigned short ret;
  	unsigned long color;

  	color = (*((unsigned long*)addr)) & 0xff000000;

  	ret = D4D_COLOR_RGB(((color & 0xffffff00)>> 3), ((color & 0xffff00ff)>> (2 + 8)), ((color & 0xff00ffff)>> (3 + 16)));

  	return ret;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function return the pointer on filled frame buffer descriptor
  //
  // PARAMETERS:  none
  //
  // RETURNS:     D4DLCD_FRAMEBUFF_DESC* - pointer on frame buffer descriptor
  //
  //-----------------------------------------------------------------------------
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_MqxMpc5125_DIU(void)
  {
  	return (D4DLCD_FRAMEBUFF_DESC*) &d4dlcdhw_mqx_mpc5125_diu_desc;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_MqxMpc5125_DIU(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
		(void)pinId;
		(void)setState;
		return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_MqxMpc5125_DIU
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_MqxMpc5125_DIU(D4DLCD_FLUSH_MODE mode)
  {
		D4D_UNUSED(mode);
  }
#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_mqx_mpc5125_diu_ID)
