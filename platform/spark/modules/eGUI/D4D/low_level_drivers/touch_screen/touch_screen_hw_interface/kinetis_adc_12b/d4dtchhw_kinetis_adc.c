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
* @file      d4dtchhw_kinetis_adc.c
*
* @author    b01800
*
* @version   0.0.5.0
*
* @date      Jun-29-2012
*
* @brief     D4D driver - low level driver of ADC and gpio for resistive touch screen
*            source file
*
******************************************************************************/
#include "d4d.h"
#include "common_files/d4d_lldapi.h"
#include "common_files/d4d_private.h"


// define the identification string of this low level driver
#define d4dtchhw_kinetis_adc_ID 1

#if (D4D_MK_STR(D4D_LLD_TCH_HW) == d4dtchhw_kinetis_adc_ID)

  #include "low_level_drivers/touch_screen/touch_screen_hw_interface/kinetis_adc_12b/d4dtchhw_kinetis_adc.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  static unsigned char D4DTCHHW_Init_kinetis_adc(void);
  static unsigned char D4DTCHHW_DeInit_kinetis_adc(void);
  static unsigned short D4DTCHHW_ReadTouchAxis_kinetis_adc(D4DTCHHW_PINS pinId);
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_kinetis_adc(void);
  static unsigned char D4DTCHHW_PinCtl_kinetis_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState);
  static unsigned char Calibrate_ADC(void);



  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  const D4DTCHHW_FUNCTIONS d4dtchhw_kinetis_adc =
  {
    D4DTCHHW_Init_kinetis_adc,
    D4DTCHHW_ReadTouchAxis_kinetis_adc,
    D4DTCHHW_GetRawLimits_kinetis_adc,
    D4DTCHHW_PinCtl_kinetis_adc,
    D4DTCHHW_DeInit_kinetis_adc
  };

  const D4D_TOUCHSCREEN_LIMITS d4dtchhw_kinetis_adc_12b_limits =
  {
    D4DTCH_FULL_SCALE,
    D4DTCH_X_TOUCH_MIN,
    D4DTCH_Y_TOUCH_MIN,
    D4DTCH_X_TOUCH_OFFMAX,
    D4DTCH_Y_TOUCH_OFFMAX
  };

  /**************************************************************//*!
  *
  * Initialize
  *
  ******************************************************************/

  static unsigned char D4DTCHHW_Init_kinetis_adc(void)
  {

    #ifdef D4DTCH_MCU_USER_INIT
      D4DTCH_MCU_USER_INIT;
    #endif

    D4DTCH_INIT_X_PLUS;
    D4DTCH_INIT_X_MINUS;
    D4DTCH_INIT_Y_PLUS;
    D4DTCH_INIT_Y_MINUS;

    return(Calibrate_ADC());
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    Calibrate ADC function
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for Calibration of ADC
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------

  static unsigned char Calibrate_ADC(void)
  {
    short cnt = 1;
    unsigned int caldata=0;

    #if D4DTCH_ADC_ID == 0
      SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // Enable module clock
    #endif

    #if D4DTCH_ADC_ID == 1
      SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK; // Enable module clock
    #endif

    //D4DTCH_INIT_TCH_PADS;

    // Use slow asynchronous clock & Long sample time for precise result
    D4DTCH_ADC_CFG1 = (ADC_CFG1_ADICLK(3)|ADC_CFG1_ADLSMP_MASK);
    D4DTCH_ADC_CFG2 = 0;  // default, longest sample time select.
    D4DTCH_ADC_SC2 = 0;   // default, SW triggered conversion, default reference
    D4DTCH_ADC_SC3 = (0 | ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3)); // Averaging, 32 samples

    // Calibrate
    D4DTCH_ADC_SC3 |= ADC_SC3_CAL_MASK;         // Set CAL
    while((0 == (D4DTCH_ADC_SC1A & ADC_SC1_COCO_MASK))&&(++cnt)){}; // until COCO set + timeout

    if(D4DTCH_ADC_SC3 & ADC_SC3_CALF_MASK)
      return(0); // IF Cal Failed return Error
    else
    {
      // Plus side calibration results
      caldata = (unsigned int)(D4DTCH_ADC_CLP0);
      caldata += (unsigned int)(D4DTCH_ADC_CLP1);
      caldata += (unsigned int)(D4DTCH_ADC_CLP2);
      caldata += (unsigned int)(D4DTCH_ADC_CLP3);
      caldata += (unsigned int)(D4DTCH_ADC_CLP4);
      caldata += (unsigned int)(D4DTCH_ADC_CLPS);
      
      caldata = caldata/2;          // div by 2
      caldata = (caldata | 0x8000); // set MSB bit
      D4DTCH_ADC_PG = caldata;
      // Minus side calibration results
      caldata = (unsigned int)(D4DTCH_ADC_CLM0);
      caldata += (unsigned int)(D4DTCH_ADC_CLM1);
      caldata += (unsigned int)(D4DTCH_ADC_CLM2);
      caldata += (unsigned int)(D4DTCH_ADC_CLM3);
      caldata += (unsigned int)(D4DTCH_ADC_CLM4);
      caldata += (unsigned int)(D4DTCH_ADC_CLMS);
      
      caldata = caldata/2;
      caldata = (caldata | 0x8000);
      D4DTCH_ADC_MG = caldata;

     return(1);// IF Cal OK
    }
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_DeInit_kinetis_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this hw low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCHHW_DeInit_kinetis_adc(void)
  {
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_ReadTouchAxis_kinetis_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used to read one RAW nonfiltered value of the
  //              touch screen signal
  // PARAMETERS:  pinId - the signal to read
  //
  // RETURNS:     readed value of the specified signal
  //
  //-----------------------------------------------------------------------------
  static unsigned short D4DTCHHW_ReadTouchAxis_kinetis_adc(D4DTCHHW_PINS pinId)
  {
     short cnt = 1;
      unsigned short advalue=0;

      // check COCO!
      if(D4DTCH_ADC_SC2 & ADC_SC2_ADACT_MASK) // COCO set
        return 0;

      // Input clock/8 = 96M/8 = 12MHz, 12-bit single-ended, long sample time for precise result
      D4DTCH_ADC_CFG1 = (ADC_CFG1_ADIV(3)|ADC_CFG1_MODE(1)|ADC_CFG1_ADLSMP_MASK);
      D4DTCH_ADC_CFG2 = 0; // default, longest sample time select.
      D4DTCH_ADC_SC2 = 0; // default, SW triggered conversion, default reference
      D4DTCH_ADC_SC3 = (0|ADC_SC3_AVGE_MASK); // HW averaging enabled, 4 samples averaged

      if(pinId == D4DTCH_X_PLUS_PIN)
        //D4DTCH_ATDCTL5 = D4DTCH_X_PLUS_ADCH;
        D4DTCH_ADC_SC1A = (0|ADC_SC1_ADCH(D4DTCH_X_PLUS_ADCH)); // Start new conversion CH10


      else if(pinId == D4DTCH_Y_PLUS_PIN)
        D4DTCH_ADC_SC1A = (0|ADC_SC1_ADCH(D4DTCH_Y_PLUS_ADCH)); // Start new conversion CH10
      else
        return 0;

      // Wait for ADC conversion to complete
      while((0 == (D4DTCH_ADC_SC1A & ADC_SC1_COCO_MASK))&&(++cnt))// until COCO set + timeout
      {
          ;
      }

      // if the measurement ends by timeout return 0 in other case result of ADC conversion
      if(cnt)
        {  //Average of four results
           advalue = (unsigned short)D4DTCH_ADC_RA;
           return(advalue);
        }
      else
        return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_GetRawLimits_kinetis_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function returns pointer on touch screen raw limits
  //              structure.
  // PARAMETERS:  none
  //
  // RETURNS:     pointer on touch screen raw limit structure
  //
  //-----------------------------------------------------------------------------
  static D4D_TOUCHSCREEN_LIMITS* D4DTCHHW_GetRawLimits_kinetis_adc(void)
  {
    return (D4D_TOUCHSCREEN_LIMITS*)&d4dtchhw_kinetis_adc_12b_limits;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DTCHHW_PinCtl_kinetis_adc
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for touch screen purposes
  //
  // PARAMETERS:  D4DTCHHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DTCHHW_PinCtl_kinetis_adc(D4DTCHHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
    switch(pinId)
    {
      case D4DTCH_X_PLUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_X_PLUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_X_PLUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_X_PLUS
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_X_PLUS;
            break;
          case D4DHW_PIN_ADC_ON:
            D4DTCH_X_PLUS_ADCH_PIN_ENABLE;
            break;
          case D4DHW_PIN_ADC_OFF:
            D4DTCH_X_PLUS_ADCH_PIN_DISABLE;
            break;
        }
        break;
      case D4DTCH_X_MINUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_X_MINUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_X_MINUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_X_MINUS
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_X_MINUS;
            break;
        }
        break;
      case D4DTCH_Y_PLUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_Y_PLUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_Y_PLUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_Y_PLUS;
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_Y_PLUS;
            break;
          case D4DHW_PIN_ADC_ON:
            D4DTCH_Y_PLUS_ADCH_PIN_ENABLE;
            break;
          case D4DHW_PIN_ADC_OFF:
            D4DTCH_Y_PLUS_ADCH_PIN_DISABLE;
            break;
        }
        break;
      case D4DTCH_Y_MINUS_PIN:
        switch(setState)
        {
          case D4DHW_PIN_OUT:
            OUTPUT(D4DTCH_Y_MINUS);
            break;
          case D4DHW_PIN_IN:
            INPUT(D4DTCH_Y_MINUS);
            break;
          case D4DHW_PIN_SET_1:
            D4DTCH_SET_Y_MINUS;
            break;
          case D4DHW_PIN_SET_0:
            D4DTCH_RESET_Y_MINUS;
            break;
        }
        break;

    }
    return 0;
  }

#endif
