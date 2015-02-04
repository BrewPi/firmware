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
* @file      d4dtchhw_template.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - template hardware touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCHHW_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtchhw_template_ID 1


// copilation enable preprocessor condition
// the string d4dtch_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_template_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\touch_screen\touch_screen_hw_interface\template\d4dtchhw_template.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DTCHHW_Init_Template(void);
  static unsigned char D4DTCHHW_DeInit_Template(void);
  static unsigned short D4DTCHHW_ReadTouchAxis_Template(D4DTCHHW_PINS pinId);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_Template(void);
  static unsigned char D4DTCHHW_PinCtl_Template(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCHHW_FUNCTIONS d4dtchhw_template =
  {
    D4DTCHHW_Init_Template,
    D4DTCHHW_ReadTouchAxis_Template,
    D4DTCHHW_GetRawLimits_Template,
    D4DTCHHW_PinCtl_Template,
    D4DTCHHW_DeInit_Template
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

  /******************************************************************************
  * Begin of D4D_TCH HW public functions
  *//*! @addtogroup doxd4d_tchhw_func
  * @{
  *******************************************************************************/

  /**************************************************************************/ /*!
  * @brief   The function is used for initialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should initilize all neccessary things to run template touch screen hardware interface driver.
  *******************************************************************************/
  static unsigned char D4DTCHHW_Init_Template(void)
  {

  }

  /**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run template touch screen hardware interface driver.
  *******************************************************************************/
  static unsigned char D4DTCHHW_DeInit_Template(void)
  {

  }

  /**************************************************************************/ /*!
  * @brief   The function is used to read one RAW nonfiltered value of the touch screen signal
  * @param   pinId - the pin definition
  * @return  ADC measured value
  * @note    This function reads the specified ADC channel
  *******************************************************************************/
  static unsigned short D4DTCHHW_ReadTouchAxis_Template(D4DTCHHW_PINS pinId)
  {

  }

  /**************************************************************************/ /*!
  * @brief   The function returns pointer on touch screen raw limits structure.
  * @return  pointer to the ADC Raw limit structure
  * @note    This function returns pointer to ADC description
  *******************************************************************************/
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_Template(void)
  {

  }

  /**************************************************************************/ /*!
  * @brief   The function allows control GPIO pins for touch screen conrol purposes
  * @param   pinId - the pin definition
  * @param   setState - the pin action/state definition
  * @return  for Get action retuns the pin value
  * @note    This function controls the additional control pins for touch screen measurement
  *******************************************************************************/
  static unsigned char D4DTCHHW_PinCtl_Template(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState)
  {

  }

  /*! @} End of doxd4d_tchhw_func                                         */

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtchhw_template_ID)
