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
* @file      d4dlcdhw_template_fb.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - template_fb hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


/******************************************************************************
* D4D LCD HW Driver setting  constants
*
*//*! @addtogroup doxd4d_lcdhwfb_const
* @{
*******************************************************************************/

/*! @brief  Identification string of driver - must be same as name D4DLCDHWFB_FUNCTIONS structure + "_ID"
 it is used for enable the code for compilation   */
#define d4dlcdhw_template_fb_ID 1

 /*! @} End of doxd4d_lcdhwfb_const                                           */

// copilation enable preprocessor condition
// the string d4dtch_template_fb_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_template_fb_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\LCD\lcd_hw_interface\template_fb\d4dlcdhw_template_fb.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_TemplateFb(void);
  static unsigned char D4DLCDHW_DeInit_TemplateFb(void);
  static void D4DLCDHW_WriteData_TemplateFb(unsigned long addr, D4D_COLOR value);
  static D4D_COLOR D4DLCDHW_ReadData_TemplateFb(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_TemplateFb(void);
  static unsigned char D4DLCDHW_PinCtl_TemplateFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_TemplateFb(D4DLCD_FLUSH_MODE mode);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHWFB_FUNCTIONS d4dlcdhw_template_fb =
  {
    D4DLCDHW_Init_TemplateFb,
    D4DLCDHW_WriteData_TemplateFb,
    D4DLCDHW_ReadData_TemplateFb,
    D4DLCDHW_PinCtl_TemplateFb,
    D4DLCD_FlushBuffer_TemplateFb,
    D4DLCDHW_DeInit_TemplateFb
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


  /**************************************************************************/ /*!
  * @brief   The function is used for initialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should initilize all neccessary things to run template lcd hardware frame buffer interface driver.
  *******************************************************************************/
  static unsigned char D4DLCDHW_Init_TemplateFb(void)
  {

     return 0; // failed because there in template is no code
  }


  /**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run template lcd hardware frame buffer interface driver.
  *******************************************************************************/
  static unsigned char D4DLCDHW_DeInit_TemplateFb(void)
  {

     return 0; // failed because there in template is no code
  }

  /**************************************************************************/ /*!
  * @brief   The function send the one pixel variable into frame buffer
  * @param   addr - the absolute address to frame buffer
  * @param   value - the pixel value
  * @return  none
  * @note    This function writes one pixel on specified address to frame buffer
  *******************************************************************************/
  static void D4DLCDHW_WriteData_TemplateFb(unsigned long addr, D4D_COLOR value)
  {

  }


  /**************************************************************************/ /*!
  * @brief   The function reads the one pixel variable from frame buffer
  * @param   addr - the absolute address to frame buffer
  * @return  the pixel value
  * @note    This function reads one pixel from specified address in frame buffer
  *******************************************************************************/
  static D4D_COLOR D4DLCDHW_ReadDataWord_TemplateFb(unsigned long addr)
  {

  }

  /**************************************************************************/ /*!
  * @brief   The function return the pointer on filled frame buffer descriptor
  * @return  pointer to frame buffer descriptor
  * @note    Just to handle pointer to frame buffer descriptor
  *******************************************************************************/
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_TemplateFb(void)
  {

  }


  /**************************************************************************/ /*!
  * @brief   The function allows control GPIO pins for LCD conrol purposes
  * @param   pinId - the pin definition
  * @param   setState - the pin action/state definition
  * @return  for Get action retuns the pin value
  * @note    This function controls the additional control pins
  *******************************************************************************/
  static unsigned char D4DLCDHW_PinCtl_TemplateFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {

  }

  /**************************************************************************/ /*!
  * @brief   For buffered low level interfaces is used to inform
  *            driver the complete object is drawed and pending pixels should be flushed
  * @param   mode - mode of Flush
  * @return  none
  * @note    This function just notify the driver that eGUI finish drawing element/object/screen to allow driver handle any kind of caching memmory.
  *******************************************************************************/
  static void D4DLCD_FlushBuffer_TemplateFb(D4DLCD_FLUSH_MODE mode)
  {

  }

  /*! @} End of doxd4d_tch_func                                               */

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_template_fb_ID)
