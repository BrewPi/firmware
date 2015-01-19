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
* @file      d4dtch_cr_touch.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Jun-29-2012
*
* @brief     D4D driver - cr_touch touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_cr_touch_ID 1


// copilation enable preprocessor condition
// the string d4dtch_cr_touch_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_cr_touch_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\touch_screen\touch_screen_drivers\cr_touch\d4dtch_cr_touch.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DTCH_Init_CrTouch(void);
  static unsigned char D4DTCH_DeInit_CrTouch(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_CrTouch(void);
  static unsigned char D4DTCH_GetPositionRaw_CrTouch(unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_cr_touch =
  {
     D4DTCH_Init_CrTouch,
     D4DTCH_GetPositionRaw_CrTouch,
     D4DTCH_GetRawLimits_CrTouch,
     D4DTCH_DeInit_CrTouch
  };

  const D4D_TOUCHSCREEN_LIMITS d4dtchhw_crtouch_limits =
  {
    D4DTCH_FULL_SCALE,
    D4DTCH_X_TOUCH_MIN,
    D4DTCH_Y_TOUCH_MIN,
    D4DTCH_X_TOUCH_OFFMAX,
    D4DTCH_Y_TOUCH_OFFMAX
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
  // FUNCTION:    D4DTCH_Init_CrTouch
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_Init_CrTouch(void)
  {
    Byte status2;

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_Init == NULL)
      return 0;

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_DeInit == NULL)
      return 0;

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_MultiReadBytes == NULL)
      return 0;


    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_Init() != D4D_LLD_TCH_HW_CRTOUCH.COMM_OK)
      return 0;

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_SetBaudRate != NULL)
      D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_SetBaudRate(D4DTCH_IIC_BAUDRATE);

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_SetAddress != NULL)
      D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_SetAddress(D4DTCH_IIC_ADDRESS);

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_MultiReadBytes(D4D_CRTCH_STATUS2, &status2, 1) != D4D_LLD_TCH_HW_CRTOUCH.COMM_OK)
    {
      (void)D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_DeInit();
      return 0;
    }

    if((status2 & D4D_CRTCH_STATUS2_RTSD_MASK) == 0)
    {
      // The touch screen hardware is NOT detected
      (void)D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_DeInit();
      return 0;
    }

  #ifdef D4D_CRTCH_ENABLE_TOUCHPENDING
    // Enable pin port clock and configure as input GPIO
    D4D_CRTCH_TOUCHPENDING_SIM_SCGC |= D4D_CRTCH_TOUCHPENDING_SIM_SCGC_MASK;
    D4D_CRTCH_TOUCHPENDING_PCR = PORT_PCR_MUX(D4D_CRTCH_TOUCHPENDING_PCR_MUX) | D4D_CRTCH_TOUCHPENDING_PCR_FLAGS;
  #endif // D4D_CRTCH_ENABLE_TOUCHPENDING //

    // Initializations is OK
    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_DeInit_CrTouch
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_DeInit_CrTouch(void)
  {
    (void)D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_DeInit();
    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetRawLimits_CrTouch
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits
  //              structure.
  // PARAMETERS:  none
  //
  // RETURNS:     pointer on touch screen raw limit structure
  //
  //-----------------------------------------------------------------------------
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_CrTouch(void)
  {
    return (D4D_TOUCHSCREEN_LIMITS*)&d4dtchhw_crtouch_limits;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetPositionRaw_CrTouch
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y
  //              coordinates if screen touched
  // PARAMETERS:  unsigned short *TouchPositionX    Pointer to X coordinate
  //              unsigned short *TouchPositionY    Pointer to Y ccordinate
  // RETURNS:     0   no screen touch
  //              1   screen touch,
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_GetPositionRaw_CrTouch(unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY)
  {

    static unsigned char last_state = 0;
    static unsigned short lastX, lastY;
    CRTOUCH_RES_SIMPLE crtouch_res;


  #ifdef D4D_CRTCH_ENABLE_TOUCHPENDING
    // Check TOUCHPENDING GPIO for a new event (must be 0, otherwise no change)
    if(D4D_CRTCH_GPIO_PDIR & (1 << D4D_CRTCH_TOUCHPENDING_PIN)) {
      *TouchPositionX = lastX;
      *TouchPositionY = lastY;
      return last_state;
    }
  #endif // D4D_CRTCH_ENABLE_TOUCHPENDING //

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_SetAddress != NULL)
      D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_SetAddress(D4DTCH_IIC_ADDRESS);

    if(D4D_LLD_TCH_HW_CRTOUCH.D4DTCHHWCRTOUCH_MultiReadBytes( D4D_CRTCH_STATUS1, (Byte*)(&crtouch_res),  sizeof(crtouch_res)) == D4D_LLD_TCH_HW_CRTOUCH.COMM_OK)
    {
      if(crtouch_res.status1 & D4D_CRTCH_STATUS1_RTST_MASK)
      {
        if(crtouch_res.status1 & D4D_CRTCH_STATUS1_RTSRDY_MASK)
        {
    #if D4DTCH_SWAP_AXES == 0
          lastX = D4D_READ16B(crtouch_res.X);
          lastY = D4D_READ16B(crtouch_res.Y);
    #else
          lastX = D4D_READ16B(crtouch_res.Y);
          lastY = D4D_READ16B(crtouch_res.X);
    #endif

    #ifdef D4DTCH_INVERSE_X_INT
          lastX = D4DTCH_INVERSE_X_INT - lastX;
    #endif

    #ifdef D4DTCH_INVERSE_Y_INT
          lastY = D4DTCH_INVERSE_Y_INT - lastY;
    #endif

          last_state = 1;
        }
      }
      else
       last_state = 0;
    }

    *TouchPositionX = lastX;
    *TouchPositionY = lastY;
    return last_state;
  }

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_cr_touch_ID)