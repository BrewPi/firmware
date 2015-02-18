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
* @file      d4dtch_tsc2046.h
*
* @author    Elco Jacobs
*
* @version   0.0.1.0
*
* @date      Feb 2015
*
* @brief     D4D driver - tsc2046_brewpi touch screen driver function header file
*
*******************************************************************************/

#ifndef __D4DTCH_TSC2046_BREWPI_H
#define __D4DTCH_TSC2046_BREWPI_H

#if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_brewpi_ID)

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "d4dtch_tsc2046_brewpi_cfg.h"
/******************************************************************************
 * Constants
 ******************************************************************************/

#define D4DTCH_AXIS_ORIENT_NORMAL 0
#define D4DTCH_AXIS_ORIENT_INVERTED 1

/******************************************************************************
 * Types
 ******************************************************************************/

/******************************************************************************
 * Macros
 ******************************************************************************/
#ifndef D4DTCH_FULL_SCALE
#define D4DTCH_FULL_SCALE           2048
#endif

#ifndef D4DTCH_X_TOUCH_MIN
#define D4DTCH_X_TOUCH_MIN           0
#endif

#ifndef D4DTCH_Y_TOUCH_MIN
#define D4DTCH_Y_TOUCH_MIN           0
#endif


#ifndef D4DTCH_X_TOUCH_OFFMAX
#define D4DTCH_X_TOUCH_OFFMAX           (D4DTCH_FULL_SCALE / 8 * 5)
#endif

#ifndef D4DTCH_Y_TOUCH_OFFMAX
#define D4DTCH_Y_TOUCH_OFFMAX           (D4DTCH_FULL_SCALE / 8 * 5)
#endif

/******************************************************************************
 * Global variables
 ******************************************************************************/

/******************************************************************************
 * Global functions
 ******************************************************************************/
#endif /* (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_brewpi_ID) */
#endif /* __D4DTCH_TSC2046_BREWPI_H */








