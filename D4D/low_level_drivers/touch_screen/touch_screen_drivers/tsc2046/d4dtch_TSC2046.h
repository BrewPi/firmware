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
* @file      d4dtch_TSC2046.h
*
* @author     Petr Gargulak
*
* @version   0.0.9.0
*
* @date      May-7-2010
*
* @brief     D4D driver - tsc2046 touch screen driver function header file
*
*******************************************************************************/

#ifndef __D4DTCH_TSC2046_H
#define __D4DTCH_TSC2046_H

  #if (D4D_MK_STR(D4D_LLD_TCH) == d4dtch_tsc2046_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dtch_tsc2046_cfg.h"

    /******************************************************************************
    * Constants
    ******************************************************************************/

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
#endif /* __D4DTCH_TSC2046_H */











#if 0  // will be implemented ASAP, Petr Gargulak

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
* @file      d4dtch_TSC2046.h
*
* @author     Petr Gargulak
*
* @version   0.0.8.0
*
* @date      Apr-30-2010
*
* @brief     D4D driver - TSC2046 touch screen driver function header file
*
*******************************************************************************/

/******************************************************************************
* Driver under development
******************************************************************************/


#ifndef __D4DTCH_TSC2046_H
#define __D4DTCH_TSC2046_H

#if (D4D_LLD_TCH == D4D_LLD_TCHSCR_TSC2046)

  /******************************************************************************
  * Includes
  ******************************************************************************/
  #include "d4dtch_TSC2046_cfg.h"
  /******************************************************************************
  * Constants
  ******************************************************************************/

  #define D4DTCH_AXIS_ORIENT_NORMAL 0
  #define D4DTCH_AXIS_ORIENT_INVERTED 1


  // define Channels
  #define TSC2046_CHNL_DIF_Y 1
  #define TSC2046_CHNL_DIF_X 5


  #define TSC_2046_CTRL_BYTE_DIF12_DEFAULT 0x80

    #if D4DTCH_SERIAL_OUTPUT_SPI_IX == 1
      #define D4DLCD_SPIBR SPI1BR
      #define D4DLCD_SPIC1 SPI1C1
      #define D4DLCD_SPIC2 SPI1C2
      #define D4DLCD_SPID SPI1D
      #define D4DLCD_SPIS SPI1S
      #define D4DLCD_SPIS_SPTEF SPI1S_SPTEF
      #define D4DLCD_SPIS_SPRF  SPI1S_SPRF
    #elif D4DTCH_SERIAL_OUTPUT_SPI_IX == 2
      #define D4DLCD_SPIBR SPI2BR
      #define D4DLCD_SPIC1 SPI2C1
      #define D4DLCD_SPIC2 SPI2C2
      #define D4DLCD_SPID SPI2D
      #define D4DLCD_SPIS SPI2S
      #define D4DLCD_SPIS_SPTEF SPI2S_SPTEF
      #define D4DLCD_SPIS_SPRF  SPI2S_SPRF
    #endif

  /******************************************************************************
  * Types
  ******************************************************************************/
  typedef union{
    unsigned int all;
    struct{
      unsigned PD0 : 1;
      unsigned PD1 : 1;
      unsigned SER_DFR : 1;
      unsigned MODE : 1;
      unsigned A0 : 1;
      unsigned A1 : 1;
      unsigned A2 : 1;
      unsigned S : 1;
    }Bits;
    struct{
      unsigned PD : 2;
      unsigned SER_DFR : 1;
      unsigned MODE : 1;
      unsigned A : 3;
      unsigned S : 1;
    }MergedBits;
  }TSC2046_CTRL_BYTE;


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
#endif /* __D4DTCH_TSC2046_H */

#endif








