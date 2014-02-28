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
* @file      d4dtch_TSC2046.c
*
* @author     Petr Gargulak
*
* @version   0.0.14.0
*
* @date      Jun-29-2012
*
* @brief     D4D driver - tsc2046 touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_tsc2046_ID 1


// copilation enable preprocessor condition
// the string d4dtch_tsc2046_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers\touch_screen\touch_screen_drivers\tsc2046\d4dtch_tsc2046.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DTCH_Init_TSC2046(void);
  static unsigned char D4DTCH_DeInit_TSC2046(void);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_TSC2046(void);
  static unsigned char D4DTCH_GetPositionRaw_TSC2046(unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DTCH_FUNCTIONS d4dtch_tsc2046 =
  {
     D4DTCH_Init_TSC2046,
     D4DTCH_GetPositionRaw_TSC2046,
     D4DTCH_GetRawLimits_TSC2046,
     D4DTCH_DeInit_TSC2046
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
  // FUNCTION:    D4DTCH_Init_TSC2046
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_Init_TSC2046(void)
  {

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_DeInit_TSC2046
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_DeInit_TSC2046(void)
  {

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetRawLimits_TSC2046
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits
  //              structure.
  // PARAMETERS:  none
  //
  // RETURNS:     pointer on touch screen raw limit structure
  //
  //-----------------------------------------------------------------------------
  static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_TSC2046(void)
  {

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCH_GetPositionRaw_TSC2046
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y
  //              coordinates if screen touched
  // PARAMETERS:  unsigned short *TouchPositionX    Pointer to X coordinate
  //              unsigned short *TouchPositionY    Pointer to Y ccordinate
  // RETURNS:     0   no screen touch
  //              1   screen touch,
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCH_GetPositionRaw_TSC2046 (unsigned short *TouchPositionX,
                                         unsigned short *TouchPositionY)
  {
     (void)TouchPositionX;
     (void)TouchPositionY;


     return 0;
  }

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_ID)

#if 0  // will be implemented ASAP, Petr Gargulak

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
* @file      d4dtch_TSC2046.c
*
* @author     Petr Gargulak
*
* @version   0.0.10.0
*
* @date      Apr-30-2010
*
* @brief     D4D driver - touch screen driver function c file
*
******************************************************************************/

/******************************************************************************
* Driver under development
******************************************************************************/


#include "d4d.h"

#if (D4D_LLD_TCH == D4D_LLD_TCH_TSC2046)

  #include "d4dtch_TSC2046.h"


  /******************************************************************************
  * Macros
  ******************************************************************************/
  #define GET(x)           (x##_PORT) & (1 << (x))
  #define SET(x)           (x##_PORT) |= (1 << (x))
  #define RESET(x)         (x##_PORT) &= ~(1 << (x))

  #define OUTPUT(x)        (x##_DDR) |= (1 << (x))
  #define INPUT(x)         (x##_DDR) &= ~(1 << (x))

  #define PULL_UP(x)       (x##_PE) |= (1 << (x))
  #define NO_PULL_UP(x)    (x##_PE) &= ~(1 << (x))

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/



  /**************************************************************//*!
  *
  * Initialize
  *
  ******************************************************************/

  void D4DTCH_Init(void)
  {

      #ifdef D4DTCH_MCU_USER_INIT
        D4DTCH_MCU_USER_INIT
      #endif

      SET(D4DTCH_CS);
      OUTPUT(D4DTCH_CS);

      SET(D4DTCH_PENIRQ);
      INPUT(D4DTCH_PENIRQ);

     /* Select the highest baud rate prescaler divisor and the highest baud rate divisor */  //AR
      D4DLCD_SPIBR = 0x34;

      /* SPI Interrupt disable, system enable and master mode selected */ //AR
      D4DLCD_SPIC1 = 0x50;
      D4DLCD_SPIC2 = 0x00;

  }


  static unsigned short Tchscr_TSC2046GetChannel(TSC2046_CTRL_BYTE ctrl_byte)
  {

      unsigned short tmp_res = 0;

      (void)D4DLCD_SPIS; // Clear status reg
      (void)D4DLCD_SPID; // Clear receive status flag

      RESET(D4DTCH_CS);

      // Send data byte
      D4DLCD_SPID = (unsigned char) ctrl_byte.all;

      while (!D4DLCD_SPIS_SPTEF) {};
      while (!D4DLCD_SPIS_SPRF) {};
      (void)D4DLCD_SPID; // Clear receive status flag

      D4DLCD_SPID = 0;

      while (!D4DLCD_SPIS_SPTEF) {};
      while (!D4DLCD_SPIS_SPRF) {};

      // Send data byte
      tmp_res = (unsigned short)(D4DLCD_SPID << 8);
      D4DLCD_SPID = 0;

      while (!D4DLCD_SPIS_SPTEF) {};
      while (!D4DLCD_SPIS_SPRF) {};
      // Send data byte
      tmp_res |= D4DLCD_SPID;


      SET(D4DTCH_CS);

    return (unsigned short)(tmp_res >> 3);
  }

  static unsigned char TchScr_GetFilteredAxis(TSC2046_CTRL_BYTE ctrl_byte, unsigned short *Res)
  {
    #define AVERAGE_BUFF_LEN 5

    unsigned short wSample[AVERAGE_BUFF_LEN];
    unsigned short wRes, wSum;
    unsigned short wADCReading;
    unsigned char tmp, tmp_res;
    int cnt;

    (void)Tchscr_TSC2046GetChannel(ctrl_byte);

    for(tmp = 0; tmp < AVERAGE_BUFF_LEN;tmp++) {

      wSample[tmp] = 0;

    }
    wSum = 0;
    cnt = 0;

    while (cnt < 33)
    {
        cnt++;
        // Read ADC value
        wADCReading = Tchscr_TSC2046GetChannel(ctrl_byte);

        wSum += wADCReading;
        wSum -= wSample[AVERAGE_BUFF_LEN - 1];
        for(tmp = (AVERAGE_BUFF_LEN - 1); tmp > 0 ;tmp--)
        {
          wSample[tmp] = wSample[tmp - 1];
        }

        wSample[0] = wADCReading;

        wRes = (unsigned short)(wSum / AVERAGE_BUFF_LEN);

        if(wADCReading < D4DTCH_Y_TOUCH_MIN)
          return 0;

        if(cnt > AVERAGE_BUFF_LEN) {
          tmp_res = 0;
          for(tmp = 0; tmp < AVERAGE_BUFF_LEN ;tmp++)
          {
            if((wRes > (wSample[tmp] - D4DTCH_SAMPLE_MARGIN)) && (wRes < (wSample[tmp] + D4DTCH_SAMPLE_MARGIN)))
            {
              tmp_res++;
            }
          }
          if(tmp_res >= (AVERAGE_BUFF_LEN - 2)) {
            *Res = wRes;
            return 1;
          }
        }
    }
    return 0;
  }

  //
  //-----------------------------------------------------------------------------
  // FUNCTION:    LCD_GetTouchScreenPositionRaw
  // SCOPE:       Global
  // DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y
  //              coordinates if screen touched
  // PARAMETERS:  unsigned int *TouchPositionX    Pointer to X coordinate
  //              unsigned int *TouchPositionY    Pointer to Y ccordinate
  // RETURNS:     0   no screen touch
  //              1   screen touch,
  //-----------------------------------------------------------------------------
  //
  unsigned char D4DTCH_GetPositionRaw (unsigned int *TouchPositionX,
                                         unsigned int *TouchPositionY)
  {
      // Declare and initialise local variables
      unsigned short tmpRes;
      TSC2046_CTRL_BYTE ctrl_byte;

      if(GET(D4DTCH_PENIRQ))     // Check if touchscreen is touched
        return 0;

      ctrl_byte.all = TSC_2046_CTRL_BYTE_DIF12_DEFAULT;

      ctrl_byte.MergedBits.A = TSC2046_CHNL_DIF_Y;
      if(!TchScr_GetFilteredAxis(ctrl_byte, &tmpRes))
      {
        return  0;
      }

      if(tmpRes > D4DTCH_Y_TOUCH_MIN) {

        #if D4DTCH_AXIS_ORIENT_Y == D4DTCH_AXIS_ORIENT_INVERTED
          tmpRes = (unsigned short)(D4DTCH_FULL_SCALE - tmpRes);
        #endif

        *TouchPositionY = tmpRes;
        ctrl_byte.MergedBits.A = TSC2046_CHNL_DIF_X;
        if(!TchScr_GetFilteredAxis(ctrl_byte, &tmpRes)) {
          return  0;
        }

        if(tmpRes > D4DTCH_X_TOUCH_MIN) {

          #if D4DTCH_AXIS_ORIENT_X == D4DTCH_AXIS_ORIENT_INVERTED
            tmpRes = (unsigned short)(D4DTCH_FULL_SCALE - tmpRes);
          #endif

          *TouchPositionX = tmpRes;
          return 1;
        }
      }
      return 0;
  }

#endif

#endif
