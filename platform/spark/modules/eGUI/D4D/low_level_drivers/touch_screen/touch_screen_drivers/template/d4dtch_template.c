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
* @file      d4dtch_template.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - template touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_template_ID 1


// copilation enable preprocessor condition
// the string d4dtch_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_template_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\touch_screen\touch_screen_drivers\template\d4dtch_template.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DTCH_Init_Template(void);
  static unsigned char D4DTCH_DeInit_Template(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Template(void);
  static unsigned char D4DTCH_GetPositionRaw_Template(unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_template =
  {
     D4DTCH_Init_Template,
     D4DTCH_GetPositionRaw_Template,
     D4DTCH_GetRawLimits_Template,
     D4DTCH_DeInit_Template
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
  * Begin of D4D_TCH public functions
  *//*! @addtogroup doxd4d_tch_func
  * @{
  *******************************************************************************/

  /**************************************************************************/ /*!
  * @brief   The function is used for initialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should initilize all neccessary things to run template touch screen driver.
  *******************************************************************************/
  static unsigned char D4DTCH_Init_Template(void)
  {
    return 0;
  }

  /**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run template touch screen driver.
  *******************************************************************************/
  static unsigned char D4DTCH_DeInit_Template(void)
  {
    return 0;
  }

  /**************************************************************************/ /*!
  * @brief   The function returns pointer on touch screen raw limits structure.
  * @return  pointer on touch screen raw limit structure
  * @note    This structure is used to calibrate the touch screen algorithm
  *******************************************************************************/
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Template(void)
  {
    return NULL;
  }

  /**************************************************************************/ /*!
  * @brief   Reads  touch screen and returns raw uncompensated X, Y coordinates if screen touched
  * @param   TouchPositionX - Pointer to X coordinate
  * @param   TouchPositionY - Pointer to Y coordinate
  * @return  0 - no screen touch;  1 - screen touch,
  * @note    This functon is used to get the touch screen events
  *******************************************************************************/
  static unsigned char D4DTCH_GetPositionRaw_Template (unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY)
  {
     (void)TouchPositionX;
     (void)TouchPositionY;

     return 0;
  }

  /*! @} End of doxd4d_tch_func                                         */

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_template_ID)
