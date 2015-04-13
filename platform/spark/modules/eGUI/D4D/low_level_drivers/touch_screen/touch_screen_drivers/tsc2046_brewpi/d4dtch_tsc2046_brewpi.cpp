/**************************************************************************
 * Copyright 2015 by Elco Jacobs, BrewPi
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
* @file      d4dtch_tsc2046_brewpi.c
*
* @author     Elco Jacobs
*
* @version   0.0.1.0
*
* @date      Feb 2015
*
* @brief     D4D driver - TSC2046 on Spark Core SPI touch screen driver source c file
*
******************************************************************************/

extern "C" {
#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR"
}

#include "application.h" // spark includes
#include "BrewPiTouch.h"

// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtch_tsc2046_brewpi_ID 1


// compilation enable preprocessor condition
// the string d4dtch_tsc2046_brewpi_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_brewpi_ID)

// include of low level driver header file
// it will be included into whole project only in case that this driver is selected in main D4D configuration file
#include "low_level_drivers/touch_screen/touch_screen_drivers/tsc2046_brewpi/d4dtch_tsc2046_brewpi.h"


/******************************************************************************
 * Macros
 ******************************************************************************/

/******************************************************************************
 * Internal function prototypes
 ******************************************************************************/

static unsigned char D4DTCH_Init_Tsc2046_brewpi(void);
static unsigned char D4DTCH_DeInit_Tsc2046_brewpi(void);
static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Tsc2046_brewpi(void);
static unsigned char D4DTCH_GetPositionRaw_Tsc2046_brewpi(unsigned short *TouchPositionX,
        unsigned short *TouchPositionY);

/**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

// the main structure that contains low level driver api functions
// the name of this structure is used for recognizing of configured low level driver of whole D4D
// so this name has to be used in main configuration header file of D4D driver to enable this driver
const D4DTCH_FUNCTIONS d4dtch_tsc2046_brewpi = {
    D4DTCH_Init_Tsc2046_brewpi,
    D4DTCH_GetPositionRaw_Tsc2046_brewpi,
    D4DTCH_GetRawLimits_Tsc2046_brewpi,
    D4DTCH_DeInit_Tsc2046_brewpi
};

const D4D_TOUCHSCREEN_LIMITS d4dtchhw_tsc2046_brewpi_limits ={
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

BrewPiTouch touch(D4DTCH_CS, D4DTCH_IRQ);

/**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCH_Init_Tsc2046_brewpi
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------

static unsigned char D4DTCH_Init_Tsc2046_brewpi(void) {
#ifdef D4DTCH_MCU_USER_INIT
    D4DTCH_MCU_USER_INIT
#endif
            // assume SPI is already initialized, just initialize CS pin and interrupt

            touch.init();
    return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCH_DeInit_Tsc2046_brewpi
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for deinitialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------

static unsigned char D4DTCH_DeInit_Tsc2046_brewpi(void) {
    return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCH_GetRawLimits_Tsc2046_brewpi
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function returns pointer on touch screen raw limits
//              structure.
// PARAMETERS:  none
//
// RETURNS:     pointer on touch screen raw limit structure
//
//-----------------------------------------------------------------------------

static D4D_TOUCHSCREEN_LIMITS* D4DTCH_GetRawLimits_Tsc2046_brewpi(void) {
    return (D4D_TOUCHSCREEN_LIMITS*)&d4dtchhw_tsc2046_brewpi_limits;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCH_GetPositionRaw_Tsc2046_brewpi
// SCOPE:       Low Level Driver API function
// DESCRIPTION: Reads  touch screen and returns raw uncompensated X, Y
//              coordinates if screen touched
// PARAMETERS:  unsigned short *TouchPositionX    Pointer to X coordinate
//              unsigned short *TouchPositionY    Pointer to Y ccordinate
// RETURNS:     0   no screen touch
//              1   screen touch,
//-----------------------------------------------------------------------------

static unsigned char D4DTCH_GetPositionRaw_Tsc2046_brewpi(unsigned short *TouchPositionX,
    unsigned short *TouchPositionY) {
    if(touch.update()){
        *TouchPositionX = touch.getXRaw();
        *TouchPositionY = touch.getYRaw();
        return 1;
    }   
    return 0;
}

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_brewpi_ID)