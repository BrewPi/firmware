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
* @file      d4dlcdhw_sp_spark_8b_cfg.h.example
*
* @author    Elco Jacobs
*
* @version   0.0.1.0
*
* @date      Feb 2015
*
* @brief     D4D low level driver SPI_SPARK_8B hardware level LCD header configuration file
*
*******************************************************************************/

#ifndef __D4DLCD_SPI_SPARK_8B_CFG_H
#define __D4DLCD_SPI_SPARK_8B_CFG_H

  /******************************************************************************
  * includes
  ******************************************************************************/

  //#include "derivative.h"       /* include peripheral declarations and more for S08 and CV1 */
  //#include "support_common.h" /* include peripheral declarations and more for CV2 */


  /******************************************************************************
  * Constants
  ******************************************************************************/

// Uncomment this line for 16 bit SPI peripheral version to run it in 8 bit mode
//  #define SPI_VERSION_16B 1

//#define D4DLCD_DISPLAY_MCU_USER_INIT;



  /*******************************************
  * Signals definition
  *******************************************/
// PB3, D4
  #define D4DLCD_CS              D4         // CS
//#define D4DLCD_CS_PORT         TODO      // CS
//#define D4DLCD_CS_DDR          TODO     // CS

// PA15, D5
  #define D4DLCD_DC              D5         // DC
//  #define D4DLCD_DC_PORT         TODO      // DC
//  #define D4DLCD_DC_DDR          TODO     // DC

// No HW reset, use software reset
//  #define D4DLCD_RESET           255       // RESET
//  #define D4DLCD_RESET_PORT      PTCD      // RESET
//  #define D4DLCD_RESET_DDR       PTCDD     // RESET

  //#define D4DLCD_BACKLIGHT       0         // BACKLIGHT
  //#define D4DLCD_BACKLIGHT_PORT  PTCD      // BACKLIGHT
  //#define D4DLCD_BACKLIGHT_DDR   PTCDD     // BACKLIGHT


  /*********** Power macros - for modification uncomment and modify ***********/

#if PLATFORM_ID==0
  #define D4DLCD_INIT_CS pinMode(D4DLCD_CS, OUTPUT)
  #define D4DLCD_ASSERT_CS PIN_MAP[D4DLCD_CS].gpio_peripheral->BRR = PIN_MAP[D4DLCD_CS].gpio_pin;
  #define D4DLCD_DEASSERT_CS PIN_MAP[D4DLCD_CS].gpio_peripheral->BSRR = PIN_MAP[D4DLCD_CS].gpio_pin;
  #define D4DLCD_INIT_DC pinMode(D4DLCD_DC, OUTPUT)
  #define D4DLCD_ASSERT_DC PIN_MAP[D4DLCD_DC].gpio_peripheral->BRR = PIN_MAP[D4DLCD_DC].gpio_pin;
  #define D4DLCD_DEASSERT_DC PIN_MAP[D4DLCD_DC].gpio_peripheral->BSRR = PIN_MAP[D4DLCD_DC].gpio_pin;

#elif PLATFORM_ID==6
  #define D4DLCD_INIT_CS pinMode(D4DLCD_CS, OUTPUT)
  #define D4DLCD_DEASSERT_CS HAL_Pin_Map()[D4DLCD_CS].gpio_peripheral->BSRRL = HAL_Pin_Map()[D4DLCD_CS].gpio_pin;
  #define D4DLCD_ASSERT_CS HAL_Pin_Map()[D4DLCD_CS].gpio_peripheral->BSRRH = HAL_Pin_Map()[D4DLCD_CS].gpio_pin;
  #define D4DLCD_INIT_DC pinMode(D4DLCD_DC, OUTPUT)
  #define D4DLCD_DEASSERT_DC HAL_Pin_Map()[D4DLCD_DC].gpio_peripheral->BSRRL = HAL_Pin_Map()[D4DLCD_DC].gpio_pin;
  #define D4DLCD_ASSERT_DC HAL_Pin_Map()[D4DLCD_DC].gpio_peripheral->BSRRH = HAL_Pin_Map()[D4DLCD_DC].gpio_pin;


#elif PLATFORM_ID==666
#include "pinmap_hal.h"
#define D4DLCD_INIT_CS pinMode(D4DLCD_CS, OUTPUT)
  #define D4DLCD_DEASSERT_CS digitalWrite(D4DLCD_CS, LOW)
  #define D4DLCD_ASSERT_CS digitalWrite(D4DLCD_CS, HIGH)
  #define D4DLCD_INIT_DC pinMode(D4DLCD_DC, OUTPUT)
  #define D4DLCD_DEASSERT_DC digitalWrite(D4DLCD_DC, LOW)
  #define D4DLCD_ASSERT_DC digitalWrite(D4DLCD_DC, HIGH)
#else
#error Unknown Platform
#endif



#if defined(D4DLCD_RESET)
  #define D4DLCD_INIT_RESET pinMode(D4DLCD_RESET, OUTPUT)
  #define D4DLCD_ASSERT_RESET digitalWrite(D4DLCD_RESET, LOW)
  #define D4DLCD_DEASSERT_RESET digitalWrite(D4DLCD_RESET, HIGH)
#else
  #define D4DLCD_INIT_RESET 
  #define D4DLCD_ASSERT_RESET D4DLCDHW_SendCmdWord_Spi_Spark_8b(0x01)
  #define D4DLCD_DEASSERT_RESET 
#endif



#endif /* __D4DLCD_SPI_SPARK_8B_CFG_H */










