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
* @file      d4dtchhw_px_adc.h
*
* @author    Jiri Kotzian
*
* @version   0.0.2.0
*
* @date      Jul-31-2012
*
* @brief     D4D driver - template hardware touch screen driver header file
*
*******************************************************************************/

#ifndef __D4DTCHHW_PX_ADC_H
#define __D4DTCHWH_PX_ADC_H

#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_px_adc_ID)

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "d4dtchhw_px_adc_cfg.h"

/******************************************************************************
 * Constants
 ******************************************************************************/
#ifndef D4DTCH_ADC
#define D4DTCH_ADC ADC_0
#endif

/******************************************************************************
 * Types
 ******************************************************************************/

/******************************************************************************
 * Macros
 ******************************************************************************/
#define SET(x)       (SIU.GPDO[x].R = 1)
#define RESET(x)     (SIU.GPDO[x].R = 0)

#define OUTPUT(x)    (SIU.PCR[x].R = D4DTCH_SIUL_OUTPUT)
#define INPUT(x)     (SIU.PCR[x].R = D4DTCH_SIUL_INPUT)   //third state
#define ADCPUT(x)    (SIU.PCR[x].R = D4DTCH_SIUL_ADC)

/****** X PLUS signal macros ******/

#ifndef D4DTCH_INIT_X_PLUS
#ifndef D4DTCH_X_PLUS
#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_INIT_X_PLUS OUTPUT(D4DTCH_X_PLUS); RESET(D4DTCH_X_PLUS);
#endif
#endif

#ifndef D4DTCH_RESET_X_PLUS
#ifndef D4DTCH_X_PLUS
#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_RESET_X_PLUS OUTPUT(D4DTCH_X_PLUS); RESET(D4DTCH_X_PLUS);
#endif
#endif

#ifndef D4DTCH_SET_X_PLUS
#ifndef D4DTCH_X_PLUS
#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_SET_X_PLUS OUTPUT(D4DTCH_X_PLUS); SET(D4DTCH_X_PLUS);
#endif
#endif

/****** X MINUS signal macros ******/

#ifndef D4DTCH_INIT_X_MINUS
#ifndef D4DTCH_X_MINUS
#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_INIT_X_MINUS OUTPUT(D4DTCH_X_MINUS); RESET(D4DTCH_X_MINUS);
#endif
#endif

#ifndef D4DTCH_RESET_X_MINUS
#ifndef D4DTCH_X_MINUS
#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_RESET_X_MINUS OUTPUT(D4DTCH_X_MINUS); RESET(D4DTCH_X_MINUS);
#endif
#endif

#ifndef D4DTCH_SET_X_MINUS
#ifndef D4DTCH_X_MINUS
#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_SET_X_MINUS OUTPUT(D4DTCH_X_MINUS); SET(D4DTCH_X_MINUS);
#endif
#endif

#ifndef D4DTCH_X_MINUS_HIGH_Z_ENABLE
#ifndef D4DTCH_X_MINUS
#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_X_MINUS_HIGH_Z_ENABLE INPUT(D4DTCH_X_MINUS);
#endif
#endif

#ifndef D4DTCH_X_MINUS_HIGH_Z_DISABLE
#ifndef D4DTCH_X_MINUS
#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_X_MINUS_HIGH_Z_DISABLE OUTPUT(D4DTCH_X_MINUS);
#endif
#endif

/****** Y PLUS signal macros ******/

#ifndef D4DTCH_INIT_Y_PLUS
#ifndef D4DTCH_Y_PLUS
#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_INIT_Y_PLUS OUTPUT(D4DTCH_Y_PLUS); RESET(D4DTCH_Y_PLUS);
#endif
#endif

#ifndef D4DTCH_RESET_Y_PLUS
#ifndef D4DTCH_Y_PLUS
#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_RESET_Y_PLUS OUTPUT(D4DTCH_Y_PLUS); RESET(D4DTCH_Y_PLUS);
#endif
#endif

#ifndef D4DTCH_SET_Y_PLUS
#ifndef D4DTCH_Y_PLUS
#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_SET_Y_PLUS OUTPUT(D4DTCH_Y_PLUS); SET(D4DTCH_Y_PLUS);
#endif
#endif

/****** Y MINUS signal macros ******/

#ifndef D4DTCH_INIT_Y_MINUS
#ifndef D4DTCH_Y_MINUS
#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_INIT_Y_MINUS OUTPUT(D4DTCH_Y_MINUS); RESET(D4DTCH_Y_MINUS);
#endif
#endif

#ifndef D4DTCH_RESET_Y_MINUS
#ifndef D4DTCH_Y_MINUS
#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_RESET_Y_MINUS OUTPUT(D4DTCH_Y_MINUS); RESET(D4DTCH_Y_MINUS);
#endif
#endif

#ifndef D4DTCH_SET_Y_MINUS
#ifndef D4DTCH_Y_MINUS
#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_SET_Y_MINUS OUTPUT(D4DTCH_Y_MINUS); SET(D4DTCH_Y_MINUS);
#endif
#endif

#ifndef D4DTCH_Y_MINUS_HIGH_Z_ENABLE
#ifndef D4DTCH_Y_MINUS
#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_Y_MINUS_HIGH_Z_ENABLE INPUT(D4DTCH_Y_MINUS);
#endif
#endif

#ifndef D4DTCH_Y_MINUS_HIGH_Z_DISABLE
#ifndef D4DTCH_Y_MINUS
#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file."
#else
#define D4DTCH_Y_MINUS_HIGH_Z_DISABLE OUTPUT(D4DTCH_Y_MINUS);
#endif
#endif

/****** Switching pins between GPIO / ADC func *****/

#ifndef D4DTCH_X_PLUS_ADCH_PIN_ENABLE
#define D4DTCH_X_PLUS_ADCH_PIN_ENABLE  ADCPUT(D4DTCH_X_PLUS);
#endif

#ifndef D4DTCH_X_PLUS_ADCH_PIN_DISABLE
#define D4DTCH_X_PLUS_ADCH_PIN_DISABLE INPUT(D4DTCH_X_PLUS);
#endif

#ifndef D4DTCH_Y_PLUS_ADCH_PIN_ENABLE
#define D4DTCH_Y_PLUS_ADCH_PIN_ENABLE  ADCPUT(D4DTCH_Y_PLUS);
#endif

#ifndef D4DTCH_Y_PLUS_ADCH_PIN_DISABLE
#define D4DTCH_Y_PLUS_ADCH_PIN_DISABLE INPUT(D4DTCH_Y_PLUS);
#endif

/****** Algorithm constants/macros *****/

#ifndef D4DTCH_FULL_SCALE
#define D4DTCH_FULL_SCALE           0x03FF
#endif

#ifndef D4DTCH_X_TOUCH_MIN
#define D4DTCH_X_TOUCH_MIN          (D4DTCH_FULL_SCALE / 10)
#endif

#ifndef D4DTCH_Y_TOUCH_MIN
#define D4DTCH_Y_TOUCH_MIN          (D4DTCH_FULL_SCALE / 10)
#endif

#ifndef D4DTCH_X_TOUCH_OFFMAX
#define D4DTCH_X_TOUCH_OFFMAX       (D4DTCH_X_TOUCH_MIN * 4 / 2)
#endif

#ifndef D4DTCH_Y_TOUCH_OFFMAX
#define D4DTCH_Y_TOUCH_OFFMAX       (D4DTCH_Y_TOUCH_MIN * 4 / 2)
#endif

#ifndef D4DTCH_SAMPLE_MARGIN
#define D4DTCH_SAMPLE_MARGIN        (D4DTCH_FULL_SCALE / 32)
#endif

/******************************************************************************
 * Global variables
 ******************************************************************************/

/******************************************************************************
 * Global functions
 ******************************************************************************/

#endif
#endif /* __D4DTCHHW_TEMPLATE_H */

