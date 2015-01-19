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
* @file      d4dlcdhw_flexbus_8b.h
*
* @author     Petr Gargulak
*
* @version   0.0.1.0
*
* @date      May-5-2010
*
* @brief     D4D driver - flexbus_8b hardware lcd driver function header file
*
*******************************************************************************/

#ifndef __D4DLCDHW_FLEXBUS_8B_H
#define __D4DLCDHW_FLEXBUS_8B_H

  #if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_flexbus_8b_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcdhw_flexbus_8b_cfg.h"


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


    /******************************************************************************
    * Constants
    ******************************************************************************/


      #ifndef D4DLCD_FLEX_BASE_ADDRESS
        #define D4DLCD_FLEX_BASE_ADDRESS  0x00410000
      #endif

      #ifndef D4DLCD_FLEX_DC_ADDRESS
        #define D4DLCD_FLEX_DC_ADDRESS    0x00400000
      #endif

      #ifndef D4DLCD_FLEX_ADRESS_MASK
        #define D4DLCD_FLEX_ADRESS_MASK    0x00010000
      #endif


      #ifndef D4DLCD_FLEX_CS
        #define D4DLCD_FLEX_CS 0
      #else
        #if (D4DLCD_FLEX_CS < 0) || (D4DLCD_FLEX_CS > 1)
          #error The D4DLCD_FLEX_CS value must be set to 0 or 1
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
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DLCDHW_FLEXBUS_8B_H */










