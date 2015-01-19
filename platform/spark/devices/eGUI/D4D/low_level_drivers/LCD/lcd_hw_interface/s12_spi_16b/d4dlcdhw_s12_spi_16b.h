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
* @file      d4dlcdhw_s12_spi_16b.h
*
* @author     Petr Gargulak
*
* @version   0.0.1.0
*
* @date      May-7-2010
*
* @brief     D4D driver - s12_spi_16b hardware lcd driver function header file
*
*******************************************************************************/

#ifndef __D4DLCDHW_S12SPI16B_H
#define __D4DLCDHW_S12SPI16B_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_s12_spi_16b_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_s12_spi_16b_cfg.h"

    /******************************************************************************
    * Constants
    ******************************************************************************/

    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros
    ******************************************************************************/
    #define SET(x)           (x##_PORT) |= (1 << (x))
    #define RESET(x)         (x##_PORT) &= ~(1 << (x))

    #define OUTPUT(x)        (x##_DDR) |= (1 << (x))
    #define INPUT(x)         (x##_DDR) &= ~(1 << (x))

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


    #if D4DLCD_COMM_HW_ID == 0
      #define D4DLCD_SPIBR 		    SPI0BR
      #define D4DLCD_SPIC1 				SPI0CR1
      #define D4DLCD_SPIC2 				SPI0CR2
      #define D4DLCD_SPID 				SPI0DR
      #define D4DLCD_SPIS_SPTEF 	SPI0SR_SPTEF
      #define D4DLCD_SPIS_SPIF    SPI0SR_SPIF
    #elif D4DLCD_COMM_HW_ID == 1
      #define D4DLCD_SPIBR 		    SPI1BR
      #define D4DLCD_SPIC1 				SPI1CR1
      #define D4DLCD_SPIC2 				SPI1CR2
      #define D4DLCD_SPID 				SPI1DR
      #define D4DLCD_SPIS_SPTEF 	SPI1SR_SPTEF
      #define D4DLCD_SPIS_SPIF    SPI1SR_SPIF
    #elif D4DLCD_COMM_HW_ID == 2
      #define D4DLCD_SPIBR 		    SPI2BR
      #define D4DLCD_SPIC1 				SPI2CR1
      #define D4DLCD_SPIC2 				SPI2CR2
      #define D4DLCD_SPID 				SPI2DR
      #define D4DLCD_SPIS_SPTEF 	SPI2SR_SPTEF
      #define D4DLCD_SPIS_SPIF    SPI2SR_SPIF
    #elif D4DLCD_COMM_HW_ID == 3
      #define D4DLCD_SPIBR 		    SPI3BR
      #define D4DLCD_SPIC1 				SPI3CR1
      #define D4DLCD_SPIC2 				SPI3CR2
      #define D4DLCD_SPID 				SPI3DR
      #define D4DLCD_SPIS_SPTEF 	SPI3SR_SPTEF
      #define D4DLCD_SPIS_SPIF    SPI3SR_SPIF
      //#define D4DLCD_READ_CMD_SUPPORT 0
    #endif

    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DLCDHW_S12SPI16B_H */










