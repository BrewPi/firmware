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
* @file      d4dlcdhw_swspi_16b.h
*
* @author     Petr Gargulak
*
* @version   0.0.3.0
*
* @date      Mar-1-2012
*
* @brief     D4D driver Software SPI header file
*
*******************************************************************************/

#ifndef __D4DRVHW_SWSPI_16B_H
#define __D4DRVHW_SWSPI_16B_H

#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_swspi_16b_ID)


  /******************************************************************************
  * Includes
  ******************************************************************************/\
#include "d4dlcdhw_swspi_16b_cfg.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  #define SET(x)           (x##_PORT) |= (1 << (x))
  #define RESET(x)         (x##_PORT) &= ~(1 << (x))

  #define OUTPUT(x)        (x##_DDR) |= (1 << (x))
  #define INPUT(x)         (x##_DDR) &= ~(1 << (x))

  /******************************************************************************
  * Constants
  ******************************************************************************/

  #ifndef D4DLCD_INIT_CS
    #ifndef D4DLCD_CS
    	#error "The Macro D4DLCD_CS must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_CS OUTPUT(D4DLCD_CS);
    #endif
  #endif

  #ifndef D4DLCD_ASSERT_CS
  	#ifndef D4DLCD_CS
    	#error "The Macro D4DLCD_CS must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_CS RESET(D4DLCD_CS);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_CS
    #ifndef D4DLCD_CS
    	#error "The Macro D4DLCD_CS must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_CS SET(D4DLCD_CS);
    #endif
  #endif

  #ifndef D4DLCD_INIT_DC
    #ifndef D4DLCD_DC
    	#error "The Macro D4DLCD_DC must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_DC OUTPUT(D4DLCD_DC);
    #endif
  #endif

  #ifndef D4DLCD_ASSERT_DC
    #ifndef D4DLCD_DC
    	#error "The Macro D4DLCD_DC must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_DC RESET(D4DLCD_DC);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_DC
    #ifndef D4DLCD_DC
    	#error "The Macro D4DLCD_DC must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_DC SET(D4DLCD_DC);
    #endif
  #endif


  #ifndef D4DLCD_READ_CMD_SUPPORT
    #define D4DLCD_READ_CMD_SUPPORT D4D_FALSE
  #endif

  #ifndef D4DLCD_INIT_SPICLK
    #ifndef D4DLCD_SPICLK
    	#error "The Macro D4DLCD_SPICLK must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_SPICLK OUTPUT(D4DLCD_SPICLK);
    #endif
  #endif

  #ifndef D4DLCD_ASSERT_SPICLK
    #ifndef D4DLCD_SPICLK
    	#error "The Macro D4DLCD_SPICLK must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_SPICLK RESET(D4DLCD_SPICLK);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_SPICLK
    #ifndef D4DLCD_SPICLK
    	#error "The Macro D4DLCD_SPICLK must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_SPICLK SET(D4DLCD_SPICLK);
    #endif
  #endif

  #ifndef D4DLCD_INIT_SPIDATA
    #ifndef D4DLCD_SPIDATA
    	#error "The Macro D4DLCD_SPIDATA must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_SPIDATA OUTPUT(D4DLCD_SPIDATA);
    #endif
  #endif

  #ifndef D4DLCD_ASSERT_SPIDATA
    #ifndef D4DLCD_SPIDATA
    	#error "The Macro D4DLCD_SPIDATA must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_SPIDATA RESET(D4DLCD_SPIDATA);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_SPIDATA
    #ifndef D4DLCD_SPIDATA
    	#error "The Macro D4DLCD_SPIDATA must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_SPIDATA SET(D4DLCD_SPIDATA);
    #endif
  #endif



  #ifdef D4DLCD_BACKLIGHT

    #ifndef D4DLCD_INIT_BACKLIGHT
    	#define D4DLCD_INIT_BACKLIGHT OUTPUT(D4DLCD_BACKLIGHT);
    #endif

    #ifndef D4DLCD_ASSERT_BACKLIGHT
    	#define D4DLCD_ASSERT_BACKLIGHT RESET(D4DLCD_BACKLIGHT);
    #endif

    #ifndef D4DLCD_DEASSERT_BACKLIGHT
      #define D4DLCD_DEASSERT_BACKLIGHT SET(D4DLCD_BACKLIGHT);
    #endif

  #endif

  #ifndef D4DLCD_INIT_RESET
    #ifndef D4DLCD_RESET
    	#error "The Macro D4DLCD_RESET must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_RESET OUTPUT(D4DLCD_RESET);
   #endif
  #endif

  #ifndef D4DLCD_ASSERT_RESET
    #ifndef D4DLCD_RESET
    	#error "The Macro D4DLCD_RESET must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_RESET RESET(D4DLCD_RESET);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_RESET
    #ifndef D4DLCD_RESET
    	#error "The Macro D4DLCD_RESET must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_RESET SET(D4DLCD_RESET);
    #endif
  #endif


  /******************************************************************************
  * Types
  ******************************************************************************/

  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Global variables
  ******************************************************************************/

  /******************************************************************************
  * Global functions
  ******************************************************************************/

#endif
#endif /* __D4DRVHW_SWSPI_16B_H */










