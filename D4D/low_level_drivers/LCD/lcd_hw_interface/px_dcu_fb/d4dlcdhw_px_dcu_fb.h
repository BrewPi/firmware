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
* @file      d4dlcdhw_px_dcu_fb.h
*
* @author    Jiri Kotzian
*
* @version   0.0.2.0
*
* @date      Jul-31-2012
*
* @brief     D4D driver - px_dcu_fb hardware lcd driver source c file
*
***************************************************************************//*!
*/
#ifndef __D4DLCDHW_PX_DCU_FB_H
#define __D4DLCDHW_PX_DCU_FB_H

#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_px_dcu_fb_ID)

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "d4dlcdhw_px_dcu_fb_cfg.h"

/******************************************************************************
 * Constants
 ******************************************************************************/
#ifndef DCU_PCLK
#error Define input clock of DCU : macro DCU_PCLK
#endif

#ifndef PCLK_MAX
#error Define maximal input clock of display : macro PCLK_MAX
#endif

#ifndef D4DLCDHWFB_START_ADDRESS
#error Define Frame Buffer Start Adress : macro D4DLCDHWFB_START_ADDRESS
#endif

#ifndef D4DLCDHWFB_X_MAX
#define D4DLCDHWFB_X_MAX 320
#endif

#ifndef D4DLCDHWFB_Y_MAX
#define D4DLCDHWFB_Y_MAX 240
#endif

#ifndef D4DLCDHWFB_BPP_BYTE
#define D4DLCDHWFB_BPP_BYTE 2
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
#endif /* __D4DLCDHW_PX_DCU_FB_H */

