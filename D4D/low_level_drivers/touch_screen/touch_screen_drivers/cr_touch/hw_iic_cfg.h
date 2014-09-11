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
* @file      hw_iic_cfg.h
* @author    Pavel Michlicek, Pavel Krenek
* @version   1.0.6.0
* @date      Oct-2-2013
* @brief     HardI2C configuration
*
******************************************************************************/

#ifndef _HW_I2C_CFG_H
#define _HW_I2C_CFG_H


/******************************************************************************
* Includes
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


#include "low_level_drivers\touch_screen\touch_screen_drivers\cr_touch\d4dtch_cr_touch.h"

// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtchhw_crtouch_iicBareMetal_ID 1




// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW_CRTOUCH) == d4dtchhw_crtouch_iicBareMetal_ID)

  /******************************************************************************
  * Constants
  ******************************************************************************/


  #define D4D_I2C_DISABLE_COMPILATION 1
  #define D4D_I2C_MCU_TYPE D4D_I2C_MCU_TYPE_MK
/*
    #define D4D_I2C_BASE      I2C0_BASE_PTR

    #define D4D_I2C_FREQ_REG   0x17        // 400Khz for 100MHz

    #define D4D_I2C_FREQ_BUS   50000000u


    #define D4D_I2C_SDA_PCR   PORTE_PCR18
    #define D4D_I2C_SCL_PCR   PORTE_PCR19

    #define D4D_I2C_SCL   19
    #define D4D_I2C_SCL_PORT  GPIOE_PDOR   // Data output register
    #define D4D_I2C_SCL_DDR   GPIOE_PDDR   // Output enable register

    #define D4D_I2C_SDA   18
    #define D4D_I2C_SDA_PORT  GPIOE_PDOR   // Data output register
    #define D4D_I2C_SDA_DDR   GPIOE_PDDR   // Output enable register
  */
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Types
  ******************************************************************************/

  /******************************************************************************
  * Global functions
  ******************************************************************************/
  // Init Denit functions
  extern Byte HwIIC_Init(void);
  extern Byte HwIIC_DeInit(void);
  // Address related functions
  extern void HwIIC_SetAddress(Byte address);
  // Multi Byte read Write functions - 8 bit device address mode
  extern Byte HwIIC_MultiReadBytes(Byte regAdr, Byte *pData, Byte cnt);
  extern Byte HwIIC_MultiWriteBytes(Byte regAdr, Byte *pData, Byte cnt);

  //Help functions
  extern sLWord HwIIC_SetBaudRate(LWord baudRate);


  /******************************************************************************
  * Global variables
  ******************************************************************************/

  const D4DTCHHWCRTOUCH_FUNCTIONS d4dtchhw_crtouch_iicBareMetal =
  {
    HwIIC_Init,
    HwIIC_DeInit,
    HwIIC_SetAddress,
    HwIIC_SetBaudRate,
    HwIIC_MultiReadBytes,
    HwIIC_MultiWriteBytes,
    0
  };
  #else
    #define D4D_I2C_DISABLE_COMPILATION 0
  #endif

#endif

#endif