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
* @file      d4dtch_mcf52277_asp.h
*
* @author     Petr Gargulak
*
* @version   0.0.2.0
*
* @date      Sep-1-2010
*
* @brief     D4D driver - mcf52277_asp touch screen driver function header file
*
*******************************************************************************/

#ifndef __D4DTCH_MDF52277_ASP_H
#define __D4DTCH_MDF52277_ASP_H

  #if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_mcf52277_asp_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtch_mcf52277_asp_cfg.h"

    /******************************************************************************
    * Constants
    ******************************************************************************/

    /******************************************************************************
    * Types
    ******************************************************************************/

    /******************************************************************************
    * Macros
    ******************************************************************************/

    #ifndef D4DTCH_MIRROVED_X
    	#define D4DTCH_MIRROVED_X 0
    #endif

    #ifndef D4DTCH_MIRROVED_Y
    	#define D4DTCH_MIRROVED_Y 0
    #endif

    #ifndef D4DTCH_SWAP_AXES
    	#define D4DTCH_SWAP_AXES 0
    #endif


    #ifndef D4DTCH_ASYNCH_EVENTS
    	#define D4DTCH_ASYNCH_EVENTS 0
    #endif

    #ifndef D4DTCHHW_VECTOR_RAM
      #define D4DTCHHW_VECTOR_RAM __VECTOR_RAM
    #endif

     /****** Algorithm constants/macros *****/
    #ifndef D4DTCH_FULL_SCALE
      #define D4DTCH_FULL_SCALE           0x0FFF
    #endif

    #ifndef D4DTCH_X_TOUCH_MIN
      #define D4DTCH_X_TOUCH_MIN              (D4DTCH_FULL_SCALE / 10)
    #endif

    #ifndef D4DTCH_Y_TOUCH_MIN
      #define D4DTCH_Y_TOUCH_MIN              (D4DTCH_FULL_SCALE / 10)
    #endif

    #ifndef D4DTCH_X_TOUCH_OFFMAX
      #define D4DTCH_X_TOUCH_OFFMAX              (D4DTCH_X_TOUCH_MIN * 4)
    #endif

    #ifndef D4DTCH_Y_TOUCH_OFFMAX
      #define D4DTCH_Y_TOUCH_OFFMAX              (D4DTCH_Y_TOUCH_MIN * 4)
    #endif

    #ifndef D4DTCH_SAMPLE_MARGIN
      #define D4DTCH_SAMPLE_MARGIN            (D4DTCH_FULL_SCALE / 256)
    #endif

    /******************************************************************************
    * Global variables
    ******************************************************************************/

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DTCH_MDF52277_ASP_H */










