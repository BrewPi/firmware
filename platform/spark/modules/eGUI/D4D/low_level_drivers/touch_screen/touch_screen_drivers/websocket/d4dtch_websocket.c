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
* @file      d4dtch_websocket.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Jan-14-2014
*
* @brief     D4D driver - websocket touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_websocket_ID 1


// copilation enable preprocessor condition
// the string d4dtch_websocket_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_websocket_ID)

  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DTCH_Init_websocket(void);
  static unsigned char D4DTCH_DeInit_websocket(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_websocket(void);
  static unsigned char D4DTCH_GetPositionRaw_websocket(unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_websocket =
  {
     D4DTCH_Init_websocket,
     D4DTCH_GetPositionRaw_websocket,
     D4DTCH_GetRawLimits_websocket,
     D4DTCH_DeInit_websocket
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
  * @note    This should initilize all neccessary things to run websocket touch screen driver.
  *******************************************************************************/
  static unsigned char D4DTCH_Init_websocket(void)
  {
    return 0;
  }

  /**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run websocket touch screen driver.
  *******************************************************************************/
  static unsigned char D4DTCH_DeInit_websocket(void)
  {
    return 0;
  }

  /**************************************************************************/ /*!
  * @brief   The function returns pointer on touch screen raw limits structure.
  * @return  pointer on touch screen raw limit structure
  * @note    This structure is used to calibrate the touch screen algorithm
  *******************************************************************************/
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_websocket(void)
  {
	  static D4D_TOUCHSCREEN_LIMITS limits = { D4D_SCREEN_SIZE_LONGER_SIDE*4, 0, 0,  D4D_SCREEN_SIZE_LONGER_SIDE, D4D_SCREEN_SIZE_SHORTER_SIDE };
	  return &limits;
  }


	  unsigned short touch_x, touch_y, touch_valid, touch_pressed;

  /**************************************************************************/ /*!
  * @brief   Reads  touch screen and returns raw uncompensated X, Y coordinates if screen touched
  * @param   TouchPositionX - Pointer to X coordinate
  * @param   TouchPositionY - Pointer to Y coordinate
  * @return  0 - no screen touch;  1 - screen touch,
  * @note    This functon is used to get the touch screen events
  *******************************************************************************/
  static unsigned char D4DTCH_GetPositionRaw_websocket (unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY)
  {
	  if (touch_valid) {
		  *TouchPositionX = touch_x;
		  *TouchPositionY = touch_y;
		  // if the device is still pressed, then continue returning these values.
		  touch_valid = touch_pressed;
		  return 1;
	  }
     return 0;
  }

void websocket_touch_clear() {
	touch_valid = 0;
	touch_pressed = 0;
}

void websocket_touch(uint16_t x, uint16_t y, uint8_t pressed)
{
	touch_x = D4D_SCREEN_SIZE_LONGER_SIDE-x;
	touch_y = D4D_SCREEN_SIZE_SHORTER_SIDE-y;
	//touch_valid = 1;
	touch_valid = pressed;
}

  /*! @} End of doxd4d_tch_func                                         */

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_websocket_ID)
