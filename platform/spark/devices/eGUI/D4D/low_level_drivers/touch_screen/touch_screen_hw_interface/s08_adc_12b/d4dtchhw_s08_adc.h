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
* @file      d4dtchhw_s08_adc.h
*
* @author     Petr Gargulak
*
* @version   0.0.3.0
*
* @date      Jul-27-2010
*
* @brief     D4D driver - low level driver of ADC and gpio for resistive touch screen
*            header file
*
*******************************************************************************/

#ifndef __D4DTCHHW_S08_ADC_H
#define __D4DTCHHW_S08_ADC_H

  #if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_s08_adc_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtchhw_s08_adc_cfg.h"

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
      	#define D4DTCH_RESET_X_PLUS RESET(D4DTCH_X_PLUS);
      #endif
    #endif

    #ifndef D4DTCH_SET_X_PLUS
      #ifndef D4DTCH_X_PLUS
      	#error "The Macro D4DTCH_X_PLUS must be set in Touch Screen low level driver configuration file."
      #else
      	#define D4DTCH_SET_X_PLUS SET(D4DTCH_X_PLUS);
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
      	#define D4DTCH_RESET_X_MINUS RESET(D4DTCH_X_MINUS);
      #endif
    #endif

    #ifndef D4DTCH_SET_X_MINUS
      #ifndef D4DTCH_X_MINUS
      	#error "The Macro D4DTCH_X_MINUS must be set in Touch Screen low level driver configuration file."
      #else
      	#define D4DTCH_SET_X_MINUS SET(D4DTCH_X_MINUS);
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
      	#define D4DTCH_RESET_Y_PLUS RESET(D4DTCH_Y_PLUS);
      #endif
    #endif

    #ifndef D4DTCH_SET_Y_PLUS
      #ifndef D4DTCH_Y_PLUS
      	#error "The Macro D4DTCH_Y_PLUS must be set in Touch Screen low level driver configuration file."
      #else
      	#define D4DTCH_SET_Y_PLUS SET(D4DTCH_Y_PLUS);
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
      	#define D4DTCH_RESET_Y_MINUS RESET(D4DTCH_Y_MINUS);
      #endif
    #endif

    #ifndef D4DTCH_SET_Y_MINUS
      #ifndef D4DTCH_Y_MINUS
      	#error "The Macro D4DTCH_Y_MINUS must be set in Touch Screen low level driver configuration file."
      #else
      	#define D4DTCH_SET_Y_MINUS SET(D4DTCH_Y_MINUS);
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


    #ifndef D4D_TCHHW_ADCH
      #define D4D_TCHHW_ADCH           ADCSC1_ADCH
    #endif

    #ifndef D4D_TCHHW_ADCCFG
      #define D4D_TCHHW_ADCCFG         ADCCFG
    #endif

    #ifndef D4D_TCHHW_ADCR
      #define D4D_TCHHW_ADCR           ADCR
    #endif

    #ifndef D4D_TCHHW_ADCSC1_COCO
      #define D4D_TCHHW_ADCSC1_COCO           ADCSC1_COCO
    #endif

    #ifndef D4D_TCHHW_ADCSC2_ADACT
      #define D4D_TCHHW_ADCSC2_ADACT           ADCSC2_ADACT
    #endif

    #ifndef D4D_TCHHW_ADC_INIT
      #define D4D_TCHHW_ADC_INIT           D4D_TCHHW_ADCCFG = 0x04;
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
      #define D4DTCH_X_TOUCH_OFFMAX              (D4DTCH_X_TOUCH_MIN * 4 / 2)
    #endif

    #ifndef D4DTCH_Y_TOUCH_OFFMAX
      #define D4DTCH_Y_TOUCH_OFFMAX              (D4DTCH_Y_TOUCH_MIN * 4 / 2)
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
#endif /* __D4DTCHHW_S08_ADC_H */










