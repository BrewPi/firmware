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
* @file      d4dtchhw_px_adc.c
*
* @author    Jiri Kotzian
*
* @version   0.0.2.0
*
* @date      Jul-31-2012
*
* @brief     D4D driver - template hardware touch screen driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR
// identification string of driver - must be same as name D4DTCHHW_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dtchhw_px_adc_ID 1

// compilation enable preprocessor condition
// the string d4dtch_template_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_px_adc_ID)

// include of low level driver heaser file
// it will be included into wole project only in case that this driver is selected in main D4D configuration file
#include "low_level_drivers/touch_screen/touch_screen_hw_interface/px_adc_10b/d4dtchhw_PX_adc.h"
/******************************************************************************
 * Macros
 ******************************************************************************/

/******************************************************************************
 * Internal function prototypes
 ******************************************************************************/

static unsigned char D4DTCHHW_Init_PX_adc(void);
static unsigned char D4DTCHHW_DeInit_PX_adc(void);
static unsigned short D4DTCHHW_ReadTouchAxis_PX_adc(D4DTCHHW_PINS pinId);
static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_PX_adc(void);
static unsigned char D4DTCHHW_PinCtl_PX_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);

/**************************************************************//*!
 *
 * Global variables
 *
 ******************************************************************/

/**************************************************************//*!
 *
 * Local variables
 *
 ******************************************************************/
// the main structure that contains low level driver api functions
// the name fo this structure is used for recognizing of configured low level driver of whole D4D
// so this name has to be used in main configuration header file of D4D driver to enable this driver
const D4DTCHHW_FUNCTIONS d4dtchhw_px_adc =
{ D4DTCHHW_Init_PX_adc, D4DTCHHW_ReadTouchAxis_PX_adc, D4DTCHHW_GetRawLimits_PX_adc, D4DTCHHW_PinCtl_PX_adc, D4DTCHHW_DeInit_PX_adc };

const D4D_TOUCHSCREEN_LIMITS d4dtchhw_px_adc_limits =
{ D4DTCH_FULL_SCALE, D4DTCH_X_TOUCH_MIN, D4DTCH_Y_TOUCH_MIN, D4DTCH_X_TOUCH_OFFMAX, D4DTCH_Y_TOUCH_OFFMAX };

/**************************************************************//*!
 *
 * Functions bodies
 *
 ******************************************************************/

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCHHW_Init_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this hw low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
static unsigned char D4DTCHHW_Init_PX_adc(void)
{
#ifdef D4DTCH_MCU_USER_INIT
    D4DTCH_MCU_USER_INIT;
#endif

    D4DTCH_ADC.MCR.R = 0x00000000; // Initialize ADC0 for one shot mode, cancel power down
    D4DTCH_ADC.CTR[1].R = 0x00008606; // Conversion times for 32MHz ADClock

    //D4DTCH_INIT_X_PLUS;
    //D4DTCH_INIT_X_MINUS;
    //D4DTCH_INIT_Y_PLUS;
    //D4DTCH_INIT_Y_MINUS;

    return (1);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCHHW_DeInit_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for deinitialization of this hw low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
static unsigned char D4DTCHHW_DeInit_PX_adc(void)
{
    ADC_0.NCMR[1].R = 0x00000000; //ADC chain is empty
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCHHW_ReadTouchAxis_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used to read one RAW nonfiltered value of the
//              touch screen signal
// PARAMETERS:  pinId - the signal to read
//
// RETURNS:     readed value of the specified signal
//
//-----------------------------------------------------------------------------
static unsigned short D4DTCHHW_ReadTouchAxis_PX_adc(D4DTCHHW_PINS pinId)
{
    uint16_t advalue = 0;

    switch (pinId)
    {
    case D4DTCH_X_PLUS_PIN: //XR AN3
        //D4DTCH_ATDCTL5 = D4DTCH_X_PLUS_ADCH;
        //ADC_0.NCMR[1].R = 0x00000001; // 00008 add to ADC chain (should be only one)
        ADC_0.NCMR[D4DTCH_X_PLUS_ADCH_SEQUENCE_REGISTER].R = D4DTCH_X_PLUS_ADCH_SEQUENCE_MASK;

        D4DTCH_ADC.MCR.B.NSTART = 1; // Trigger normal conversions for ADC0
        while (D4DTCH_ADC.MSR.B.NSTART == 1)
        {
        }
        ; // Wait for chain to complete

        advalue = (uint16_t) D4DTCH_ADC.CDR[D4DTCH_X_PLUS_ADCH].B.CDATA; // 35Read first conversion result data register
        break;

    case D4DTCH_Y_PLUS_PIN: //YU AN0
        //D4DTCH_ATDCTL5 = D4DTCH_Y_PLUS_ADCH;
        //ADC_0.NCMR[1].R = 0x00000002; // 00001 add to ADC chain (should be only one)
        ADC_0.NCMR[D4DTCH_Y_PLUS_ADCH_SEQUENCE_REGISTER].R = D4DTCH_Y_PLUS_ADCH_SEQUENCE_MASK;

        D4DTCH_ADC.MCR.B.NSTART = 1; // Trigger normal conversions for ADC0

        while (D4DTCH_ADC.MSR.B.NSTART == 1)
        {
        }
        ; // Wait for chain to complete

        advalue = (uint16_t) D4DTCH_ADC.CDR[D4DTCH_Y_PLUS_ADCH].B.CDATA; // 32Read first conversion result data register
        break;
    }

    return (advalue);

}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCHHW_GetRawLimits_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function returns pointer on touch screen raw limits
//              structure.
// PARAMETERS:  none
//
// RETURNS:     pointer on touch screen raw limit structure
//
//-----------------------------------------------------------------------------
static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_PX_adc(void)
{
    return (D4D_TOUCHSCREEN_LIMITS*) &d4dtchhw_px_adc_limits;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DTCHHW_PinCtl_Template
// SCOPE:       Low Level Driver API function
// DESCRIPTION: allows control GPIO pins for touch screen purposes
//
// PARAMETERS:  D4DTCHHW_PINS pinId - Pin identification
//              D4DHW_PIN_STATE setState - Pin action
// RETURNS:     for Get action retuns the pin value
//-----------------------------------------------------------------------------
static unsigned char D4DTCHHW_PinCtl_PX_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState)
{
    switch (pinId)
    {
    case D4DTCH_X_PLUS_PIN:
        switch (setState)
        {
        case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_X_PLUS);
            //SIU.PCR[D4DTCH_X_PLUS].R = 0x0200; //pin output
            break;
        case D4DHW_PIN_IN:
            INPUT(D4DTCH_X_PLUS);
            //SIU.PCR[D4DTCH_X_PLUS].R = 0x0000; //pin three state
            break;
        case D4DHW_PIN_SET_1:
            D4DTCH_SET_X_PLUS
            //SIU.PCR[D4DTCH_X_PLUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_X_PLUS].R = 1; //set output 1
            break;
        case D4DHW_PIN_SET_0:
            D4DTCH_RESET_X_PLUS
            //SIU.PCR[D4DTCH_X_PLUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_X_PLUS].R = 0; //set output 0
            break;
        case D4DHW_PIN_ADC_ON:
            D4DTCH_X_PLUS_ADCH_PIN_ENABLE;
            //SIU.PCR[D4DTCH_X_PLUS].R = 0x2000; // pin is ADC input

            break;
        case D4DHW_PIN_ADC_OFF:
            D4DTCH_X_PLUS_ADCH_PIN_DISABLE;
            //SIU.PCR[D4DTCH_X_PLUS].R = 0x0000; // pin three state
            break;
        }
        break;
    case D4DTCH_X_MINUS_PIN:
        switch (setState)
        {
        case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_X_MINUS);
            //SIU.PCR[D4DTCH_X_MINUS].R = 0x0200; //pin output
            break;
        case D4DHW_PIN_IN:
            INPUT(D4DTCH_X_MINUS);
            //SIU.PCR[D4DTCH_X_MINUS].R = 0x0000; //pin three state
            break;
        case D4DHW_PIN_SET_1:
            D4DTCH_SET_X_MINUS
            //SIU.PCR[D4DTCH_X_MINUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_X_MINUS].R = 1; //set output 1
            break;
        case D4DHW_PIN_SET_0:
            D4DTCH_RESET_X_MINUS
            //SIU.PCR[D4DTCH_X_MINUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_X_MINUS].R = 0; //set output 0
            break;
        }
        break;
    case D4DTCH_Y_PLUS_PIN:
        switch (setState)
        {
        case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_Y_PLUS);
            //SIU.PCR[D4DTCH_Y_PLUS].R = 0x0200; //pin output
            break;
        case D4DHW_PIN_IN:
            INPUT(D4DTCH_Y_PLUS);
            //SIU.PCR[D4DTCH_Y_PLUS].R = 0x0000; //pin three state
            break;
        case D4DHW_PIN_SET_1:
            D4DTCH_SET_Y_PLUS
            //SIU.PCR[D4DTCH_Y_PLUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_Y_PLUS].R = 1; //set output 1
            break;
        case D4DHW_PIN_SET_0:
            D4DTCH_RESET_Y_PLUS
            //SIU.PCR[D4DTCH_Y_PLUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_Y_PLUS].R = 0; //set output 0
            break;
        case D4DHW_PIN_ADC_ON:
            D4DTCH_Y_PLUS_ADCH_PIN_ENABLE
            //SIU.PCR[D4DTCH_Y_PLUS].R = 0x2000; // pin is ADC input
            break;
        case D4DHW_PIN_ADC_OFF:
            D4DTCH_Y_PLUS_ADCH_PIN_DISABLE
            //SIU.PCR[D4DTCH_Y_PLUS].R = 0x0000; // pin three state
            break;
        }
        break;
    case D4DTCH_Y_MINUS_PIN:
        switch (setState)
        {
        case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_Y_MINUS);
            //SIU.PCR[D4DTCH_Y_MINUS].R = 0x0200; //pin output
            break;
        case D4DHW_PIN_IN:
            INPUT(D4DTCH_Y_MINUS);
            //SIU.PCR[D4DTCH_Y_MINUS].R = 0x0000; //pin three state
            break;
        case D4DHW_PIN_SET_1:
            D4DTCH_SET_Y_MINUS
            //SIU.PCR[D4DTCH_Y_MINUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_Y_MINUS].R = 1; //set output 1
            break;
        case D4DHW_PIN_SET_0:
            D4DTCH_RESET_Y_MINUS
            //SIU.PCR[D4DTCH_Y_MINUS].R = 0x0200; //pin output
            //SIU.GPDO[D4DTCH_Y_MINUS].R = 0; //set output 0
            break;
        }
        break;
    }
    return 0;
}

#endif //(D4D_MK_STR(D4D_LLD_TCH) == d4dtchhw_px_adc_ID)
