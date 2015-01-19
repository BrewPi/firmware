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
* @file      d4dlcd_LS020.h
*
* @author     Petr Gargulak
*
* @version   0.0.10.0
*
* @date      Jun-4-2010
*
* @brief     D4D driver - ls020 lcd driver function header file
*
*******************************************************************************/

#ifndef __D4DLCD_LS020_H
#define __D4DLCD_LS020_H

  #if (D4D_MK_STR(D4D_LLD_LCD) == d4dlcd_ls020_ID)

    /******************************************************************************
    * Includes
    ******************************************************************************/
    #include "d4dlcd_ls020_cfg.h"

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

		#ifdef D4D_LLD_LCD_HW
		  extern const D4DLCDHW_FUNCTIONS D4D_LLD_LCD_HW;
		#endif

    /******************************************************************************
    * Global functions
    ******************************************************************************/

  #endif
#endif /* __D4DLCD_LS020_H */










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
* @file      d4dlcd_d3000TFT2_1.h
*
* @author     Petr Gargulak
*
* @version   0.0.8.0
*
* @date      Nov-4-2009
*
* @brief     D4D driver for d3000 2.1" LCD header file
*
*******************************************************************************/

#ifndef __D4DLCD_D3000TFT2_1_H
#define __D4DLCD_D3000TFT2_1_H

#if (D4D_LLD_LCD == D4D_LLD_LCD_D3000TFT2_1)

  /******************************************************************************
  * Includes
  ******************************************************************************/
  #include "d4dlcd_d3000TFT2_1_cfg.h"
  /******************************************************************************
  * Constants
  ******************************************************************************/

  #define D4DLCD_COMM_HW_NONE 0
  #define D4DLCD_COMM_HW_SPI 1
  #define D4DLCD_COMM_HW_QSPI 2
  #define D4DLCD_COMM_HW_DSPI 3
  #define D4DLCD_COMM_HW_SWSPI 4
  #define D4DLCD_COMM_HW_PAR_GPIO 10
  #define D4DLCD_COMM_HW_PAR_FLEX 11



  #define D4DLCD_PIXEL_COUNT (D4D_SCREEN_SIZE_SHORTER_SIDE * D4D_SCREEN_SIZE_LONGER_SIDE)


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


  #ifndef D4DLCD_INIT_SPICLK
    #ifndef D4DLCD_SPICLK
    	#error "The Macro D4DLCD_SPICLK must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_SPICLK OUTPUT(D4DLCD_SPICLK);
    #endif
  #endif

  #ifndef D4DLCD_ASSERT_SPICLK
    #ifndef D4DLCD_SPICLK
    	#error "The Macro D4DLCD_SPICLK must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_SPICLK RESET(D4DLCD_SPICLK);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_SPICLK
    #ifndef D4DLCD_SPICLK
    	#error "The Macro D4DLCD_SPICLK must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_SPICLK SET(D4DLCD_SPICLK);
    #endif
  #endif

  #ifndef D4DLCD_INIT_SPIDATA
    #ifndef D4DLCD_SPIDATA
    	#error "The Macro D4DLCD_SPIDATA must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_INIT_SPIDATA OUTPUT(D4DLCD_SPIDATA);
    #endif
  #endif

  #ifndef D4DLCD_ASSERT_SPIDATA
    #ifndef D4DLCD_SPIDATA
    	#error "The Macro D4DLCD_SPIDATA must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_ASSERT_SPIDATA RESET(D4DLCD_SPIDATA);
    #endif
  #endif

  #ifndef D4DLCD_DEASSERT_SPIDATA
    #ifndef D4DLCD_SPIDATA
    	#error "The Macro D4DLCD_SPIDATA must be set in LCD low level driver configuration file."
    #else
    	#define D4DLCD_DEASSERT_SPIDATA SET(D4DLCD_SPIDATA);
    #endif
  #endif

  #ifdef D4DLCD_COMM_HW

    #if D4DLCD_COMM_HW == D4DLCD_COMM_HW_SPI

      #if D4DLCD_COMM_HW_ID == 0
        #define D4DLCD_SPIBR 				SPIBR
        #define D4DLCD_SPIC1 				SPIC1
        #define D4DLCD_SPIC2 				SPIC2
        #define D4DLCD_SPID 				SPID
        #define D4DLCD_SPIS_SPTEF 	SPIS_SPTEF
      #elif D4DLCD_COMM_HW_ID == 1
        #define D4DLCD_SPIBR 				SPI1BR
        #define D4DLCD_SPIC1 				SPI1C1
        #define D4DLCD_SPIC2 				SPI1C2
        #define D4DLCD_SPID 				SPI1D
        #define D4DLCD_SPIS_SPTEF 	SPI1S_SPTEF
      #elif D4DLCD_COMM_HW_ID == 2
        #define D4DLCD_SPIBR 				SPI2BR
        #define D4DLCD_SPIC1 				SPI2C1
        #define D4DLCD_SPIC2 				SPI2C2
        #define D4DLCD_SPID 				SPI2D
        #define D4DLCD_SPIS_SPTEF 	SPI2S_SPTEF
      #elif D4DLCD_COMM_HW_ID == 3
        #define D4DLCD_SPIBR 				SPI3BR
        #define D4DLCD_SPIC1 				SPI3C1
        #define D4DLCD_SPIC2 				SPI3C2
        #define D4DLCD_SPID 				SPI3D
        #define D4DLCD_SPIS_SPTEF 	SPI3S_SPTEF
      #endif

      #define LCD_READ_CMD_SUPPORT 0

    #elif D4DLCD_COMM_HW == D4DLCD_COMM_HW_QSPI

    	#if D4DLCD_COMM_HW_ID == 0
				#define D4DLCD_QSPI_QIR 				MCF_QSPI_QIR
				#define D4DLCD_QSPI_QIR_WCEF 		MCF_QSPI_QIR_WCEF
				#define D4DLCD_QSPI_QIR_ABRT 		MCF_QSPI_QIR_ABRT
				#define D4DLCD_QSPI_QIR_SPIF 		MCF_QSPI_QIR_SPIF
				#define D4DLCD_QSPI_QDLYR 			MCF_QSPI_QDLYR
				#define D4DLCD_QSPI_QDLYR_SPE 	MCF_QSPI_QDLYR_SPE
				#define D4DLCD_QSPI_QWR 				MCF_QSPI_QWR
				#define D4DLCD_QSPI_QWR_HALT 		MCF_QSPI_QWR_HALT
				#define D4DLCD_QSPI_QWR_CSIV 		MCF_QSPI_QWR_CSIV
				#define D4DLCD_QSPI_QWR_NEWQP 	MCF_QSPI_QWR_NEWQP
				#define D4DLCD_QSPI_QWR_ENDQP 	MCF_QSPI_QWR_ENDQP
				#define D4DLCD_QSPI_QMR 				MCF_QSPI_QMR
				#define D4DLCD_QSPI_QMR_MSTR 		MCF_QSPI_QMR_MSTR
				#define D4DLCD_QSPI_QAR 				MCF_QSPI_QAR
				#define D4DLCD_QSPI_QAR_CMD 		MCF_QSPI_QAR_CMD
				#define D4DLCD_QSPI_QAR_TRANS 	MCF_QSPI_QAR_TRANS
				#define D4DLCD_QSPI_QDR 				MCF_QSPI_QDR
				#define D4DLCD_QSPI_QDR_BITSE 	MCF_QSPI_QDR_BITSE
	    #elif D4DLCD_COMM_HW_ID == 1
	    	#define D4DLCD_QSPI_QIR 				MCF_QSPI1_QIR
				#define D4DLCD_QSPI_QIR_WCEF 		MCF_QSPI1_QIR_WCEF
				#define D4DLCD_QSPI_QIR_ABRT 		MCF_QSPI1_QIR_ABRT
				#define D4DLCD_QSPI_QIR_SPIF 		MCF_QSPI1_QIR_SPIF
				#define D4DLCD_QSPI_QDLYR 			MCF_QSPI1_QDLYR
				#define D4DLCD_QSPI_QDLYR_SPE 	MCF_QSPI1_QDLYR_SPE
				#define D4DLCD_QSPI_QWR 				MCF_QSPI1_QWR
				#define D4DLCD_QSPI_QWR_HALT 		MCF_QSPI1_QWR_HALT
				#define D4DLCD_QSPI_QWR_CSIV 		MCF_QSPI1_QWR_CSIV
				#define D4DLCD_QSPI_QWR_NEWQP 	MCF_QSPI1_QWR_NEWQP
				#define D4DLCD_QSPI_QWR_ENDQP 	MCF_QSPI1_QWR_ENDQP
				#define D4DLCD_QSPI_QMR 				MCF_QSPI1_QMR
				#define D4DLCD_QSPI_QMR_MSTR 		MCF_QSPI1_QMR_MSTR
				#define D4DLCD_QSPI_QAR 				MCF_QSPI1_QAR
				#define D4DLCD_QSPI_QAR_CMD 		MCF_QSPI1_QAR_CMD
				#define D4DLCD_QSPI_QAR_TRANS 	MCF_QSPI1_QAR_TRANS
				#define D4DLCD_QSPI_QDR 				MCF_QSPI1_QDR
				#define D4DLCD_QSPI_QDR_BITSE 	MCF_QSPI1_QDR_BITSE
	    #elif D4DLCD_COMM_HW_ID == 2
	    	#define D4DLCD_QSPI_QIR 				MCF_QSPI2_QIR
				#define D4DLCD_QSPI_QIR_WCEF 		MCF_QSPI2_QIR_WCEF
				#define D4DLCD_QSPI_QIR_ABRT 		MCF_QSPI2_QIR_ABRT
				#define D4DLCD_QSPI_QIR_SPIF 		MCF_QSPI2_QIR_SPIF
				#define D4DLCD_QSPI_QDLYR 			MCF_QSPI2_QDLYR
				#define D4DLCD_QSPI_QDLYR_SPE 	MCF_QSPI2_QDLYR_SPE
				#define D4DLCD_QSPI_QWR 				MCF_QSPI2_QWR
				#define D4DLCD_QSPI_QWR_HALT 		MCF_QSPI2_QWR_HALT
				#define D4DLCD_QSPI_QWR_CSIV 		MCF_QSPI2_QWR_CSIV
				#define D4DLCD_QSPI_QWR_NEWQP 	MCF_QSPI2_QWR_NEWQP
				#define D4DLCD_QSPI_QWR_ENDQP 	MCF_QSPI2_QWR_ENDQP
				#define D4DLCD_QSPI_QMR 				MCF_QSPI2_QMR
				#define D4DLCD_QSPI_QMR_MSTR 		MCF_QSPI2_QMR_MSTR
				#define D4DLCD_QSPI_QAR 				MCF_QSPI2_QAR
				#define D4DLCD_QSPI_QAR_CMD 		MCF_QSPI2_QAR_CMD
				#define D4DLCD_QSPI_QAR_TRANS 	MCF_QSPI2_QAR_TRANS
				#define D4DLCD_QSPI_QDR 				MCF_QSPI2_QDR
				#define D4DLCD_QSPI_QDR_BITSE 	MCF_QSPI2_QDR_BITSE
	    #endif

    #elif D4DLCD_COMM_HW == D4DLCD_COMM_HW_PAR_GPIO
      #define LCD_READ_CMD_SUPPORT 1


      #ifndef D4DLCD_INIT_RD
	      #ifndef D4DLCD_RD
		    	#error "The Macro D4DLCD_RD must be set in LCD low level driver configuration file."
		    #else
		      #define D4DLCD_INIT_RD OUTPUT(D4DLCD_RD);
		    #endif
      #endif

      #ifndef D4DLCD_ASSERT_RD
        #ifndef D4DLCD_RD
		    	#error "The Macro D4DLCD_RD must be set in LCD low level driver configuration file."
		    #else
		    	#define D4DLCD_ASSERT_RD RESET(D4DLCD_RD);
		    #endif
      #endif

      #ifndef D4DLCD_DEASSERT_RD
        #ifndef D4DLCD_RD
		    	#error "The Macro D4DLCD_RD must be set in LCD low level driver configuration file."
		    #else
		    	#define D4DLCD_DEASSERT_RD SET(D4DLCD_RD);
		    #endif
      #endif


      #ifndef D4DLCD_INIT_WR
        #ifndef D4DLCD_WR
		    	#error "The Macro D4DLCD_WR must be set in LCD low level driver configuration file."
		    #else
		    	#define D4DLCD_INIT_WR OUTPUT(D4DLCD_WR);
		   #endif
      #endif

      #ifndef D4DLCD_ASSERT_WR
        #ifndef D4DLCD_WR
		    	#error "The Macro D4DLCD_WR must be set in LCD low level driver configuration file."
		    #else
		    	#define D4DLCD_ASSERT_WR RESET(D4DLCD_WR);
		    #endif
      #endif

      #ifndef D4DLCD_DEASSERT_WR
        #ifndef D4DLCD_WR
		    	#error "The Macro D4DLCD_WR must be set in LCD low level driver configuration file."
		    #else
		    	#define D4DLCD_DEASSERT_WR SET(D4DLCD_WR);
		    #endif
      #endif


      #ifndef D4DLCD_INIT_DATA
        #ifndef D4DLCD_DATA
		    	#error "The Macro D4DLCD_DATA must be set in LCD low level driver configuration file."
		    #else
		    	#define D4DLCD_INIT_DATA OUTPUT_ALL(D4DLCD_DATA);
		    #endif
      #endif

    #else

      #define LCD_READ_CMD_SUPPORT 0

    #endif
  #else

    #error "The Macro D4DLCD_COMM_HW must be set in LCD low level configuration file."

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
#endif /* __D4DLCD_D3000TFT2_1_H */

#endif
