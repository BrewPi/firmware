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
* @file      d4dtch_resistive_mqx.c
*
* @author     Petr Gargulak
*
* @version   0.0.7.0
*
* @date      Jun-29-2012
*
* @brief     D4D driver - resistive touch screen driver source c file
*
******************************************************************************/
#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"

#define d4dtch_resistive_mqx_ID 1

#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_resistive_mqx_ID)

#include <mqx.h>
#include <fio.h>
#include <tchres.h>
#include "low_level_drivers\touch_screen\touch_screen_drivers\resistive_mqx\d4dtch_resistive_mqx.h"

/******************************************************************************
* Macros
******************************************************************************/

/**************************************************************//*!
*
* Local functions
*
******************************************************************/
static unsigned char D4DTCH_Init_Tchres_Mqx(void);
static unsigned char D4DTCH_DeInit_Tchres_Mqx(void);
static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Tchres_Mqx(void);
static unsigned char D4DTCH_GetPositionRaw_Tchres_Mqx (unsigned short *TouchPositionX,
                                      unsigned short *TouchPositionY);

/**************************************************************//*!
*
* Global variables
*
******************************************************************/
const D4DTCH_FUNCTIONS d4dtch_resistive_mqx =
{
   D4DTCH_Init_Tchres_Mqx,
   D4DTCH_GetPositionRaw_Tchres_Mqx,
   D4DTCH_GetRawLimits_Tchres_Mqx,
   D4DTCH_DeInit_Tchres_Mqx
};

/**************************************************************//*!
*
* Local variables
*
******************************************************************/
static FILE_PTR tchscr_dev;

/**************************************************************//*!
*
* Initialize
*
******************************************************************/
static unsigned char D4DTCH_Init_Tchres_Mqx(void)
{
   tchscr_dev = fopen("tchscr:", NULL);
   if( tchscr_dev == NULL )
    	return 0;
   else
   		return 1;
}

/**************************************************************//*!
*
* Deinitialize
*
******************************************************************/
static unsigned char D4DTCH_DeInit_Tchres_Mqx(void)
{
   if( tchscr_dev )
   {
      fclose( tchscr_dev );
      tchscr_dev = NULL;
   }
   return 1;
}

/**************************************************************//*!
*
* Initialize
*
******************************************************************/
static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Tchres_Mqx(void)
{
   static D4D_TOUCHSCREEN_LIMITS touchscreen_limits;
   TCHRES_ADC_LIMITS_STRUCT limit_struct;

   if( tchscr_dev )
   {
      _io_ioctl(tchscr_dev, IO_IOCTL_TCHSCR_GET_RAW_LIMITS, &limit_struct);

//       *(&touchscreen_limits) = *(D4D_TOUCHSCREEN_LIMITS *)(&limit_struct);

      touchscreen_limits.rawDataScale = (unsigned short)limit_struct.FULL_SCALE;
      touchscreen_limits.touchMinX = (unsigned short)limit_struct.X_TOUCH_MIN;
      touchscreen_limits.touchMinY = (unsigned short)limit_struct.Y_TOUCH_MIN;
      touchscreen_limits.touchOffMaxX = (unsigned short)limit_struct.X_TOUCH_MAX;
      touchscreen_limits.touchOffMaxY = (unsigned short)limit_struct.Y_TOUCH_MAX;

      return( &touchscreen_limits );
   }
   return( NULL );
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
static unsigned char D4DTCH_GetPositionRaw_Tchres_Mqx (unsigned short *TouchPositionX,
                                      unsigned short *TouchPositionY)
{
   TCHRES_POSITION_STRUCT  position;

   if( tchscr_dev == NULL )
   {
      return 0;
   }
   if (ioctl(tchscr_dev, IO_IOCTL_TCHSCR_GET_POSITION_RAW, &position ) == TCHRES_OK)
   {
      *TouchPositionX = (unsigned short)position.X;
      *TouchPositionY = (unsigned short)position.Y;
      return 1;
   }
   else
   {
      return 0;
   }
}

#endif
