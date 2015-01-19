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
* @file      d4dlcdhw_px_dcu_fb.c
*
* @author    Jiri Kotzian
*
* @version   0.0.3.0
*
* @date      Aug-9-2012
*
* @brief     D4D driver - px_dcu_fb hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_px_dcu_fb_ID 1

// copilation enable preprocessor condition
// the string d4dtch_template_fb_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_px_dcu_fb_ID)

// include of low level driver heaser file
// it will be included into wole project only in case that this driver is selected in main D4D configuration file
#include "d4dlcdhw_px_dcu_fb.h"
/******************************************************************************
 * Macros
 ******************************************************************************/

/******************************************************************************
 * Internal function prototypes
 ******************************************************************************/
static void TWR_PXD10_SIUL_setup(void);
static void EDT_setup(void);
static void TWR_LCD_RGB_setup(void);

static unsigned char D4DLCDHW_Init_PxDcuFb(void);
static unsigned char D4DLCDHW_DeInit_PxDcuFb(void);
static void D4DLCDHW_WriteData_PxDcuFb(unsigned long addr, D4D_COLOR value);
static D4D_COLOR D4DLCDHW_ReadData_PxDcuFb(unsigned long addr);
static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_PxDcuFb(void);
static unsigned char D4DLCDHW_PinCtl_PxDcuFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_PxDcuFb(D4DLCD_FLUSH_MODE mode);

/**************************************************************//*!
 *
 * Global variables
 *
 ******************************************************************/

// the main structure that contains low level driver api functions
// the name fo this structure is used for recognizing of configured low level driver of whole D4D
// so this name has to be used in main configuration header file of D4D driver to enable this driver
const D4DLCDHWFB_FUNCTIONS d4dlcdhw_px_dcu_fb =
{
	D4DLCDHW_Init_PxDcuFb,
	D4DLCDHW_WriteData_PxDcuFb,
	D4DLCDHW_ReadData_PxDcuFb,
	D4DLCDHW_GetFbDescriptor_PxDcuFb,
	D4DLCDHW_PinCtl_PxDcuFb,
    D4DLCD_FlushBuffer_PxDcuFb,
	D4DLCDHW_DeInit_PxDcuFb
};
/**************************************************************//*!
 *
 * Local variables
 *
 ******************************************************************/
static const D4DLCD_FRAMEBUFF_DESC d4dlcdhw_px_dcu_desc =
	{
	 	D4DLCDHWFB_START_ADDRESS,
		D4DLCDHWFB_X_MAX,
		D4DLCDHWFB_Y_MAX,
		D4DLCDHWFB_BPP_BYTE
	};

/**************************************************************//*!
 *
 * Functions bodies
 *
 ******************************************************************/

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_Init_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for initialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
uint32_t* clut_memory_test = (uint32_t*) 0xffe7e000;

static unsigned char D4DLCDHW_Init_PxDcuFb(void)
{
    uint8_t i;
    uint32_t j;
    DCU.DCU_MODE.R = 0x00000000; //Ensure DCU is disabled
    DCU.BGND.R = D4DLCD_BACKGROUND_COLOR; //Background color 0x00RRGGBB, 0x000000 = black
    // Configure layers to initial settings
    for (i = 0; i < 16; i++)
    {
        DCU.LAYER[i].CTRLDESCL1.B.HEIGHT = 128; //Height of layer
        DCU.LAYER[i].CTRLDESCL1.B.WIDTH = 96; //Width of layer
        DCU.LAYER[i].CTRLDESCL2.B.POSX = 0; //Place this distance from left
        DCU.LAYER[i].CTRLDESCL2.B.POSY = 0; //Place this distance from top
        //DCU.LAYER[i].CTRLDESCL3.R = NULL;         //Address of image
        DCU.LAYER[i].CTRLDESCL4.B.LUOFFS = 0; //offset for CLUT
        DCU.LAYER[i].CTRLDESCL4.B.BPP = 4; //4 16bpp rgb565 // 3 8bpp //2 4bpp

        //Layer blending and enable
        DCU.LAYER[i].CTRLDESCL4.B.TRANS = 0x80; //50% transparent
        DCU.LAYER[i].CTRLDESCL4.B.BB = 1; //Chroma keying
        DCU.LAYER[i].CTRLDESCL4.B.AB = 0; //Blend chroma pixels

        //Chroma setting
        DCU.LAYER[i].CTRLDESCL5.B.CKMAX_R = 0; //Max red chroma
        DCU.LAYER[i].CTRLDESCL6.B.CKMIN_R = 0; //Min red chroma
        DCU.LAYER[i].CTRLDESCL5.B.CKMAX_G = 0; //Max green chroma
        DCU.LAYER[i].CTRLDESCL6.B.CKMIN_G = 0; //Min green chroma
        DCU.LAYER[i].CTRLDESCL5.B.CKMAX_B = 0; //Max blue chroma
        DCU.LAYER[i].CTRLDESCL6.B.CKMIN_B = 0; //Min blue chroma
        //Tile settings
        DCU.LAYER[i].CTRLDESCL7.B.TILE_VER_SIZE = 0;
        DCU.LAYER[i].CTRLDESCL7.B.TILE_HOR_SIZE = 0;
        //Enable layer
        DCU.LAYER[i].CTRLDESCL4.B.EN = 0; //Turn on the layer
    }

    // generate look color table for 8bpp  0bRRR GGG BB  -> 0xRRGGBB
    for (j = 0; j < 256; j++)
    {
        DCU.CLUT[j] = ((j & 0b11100000) << 16) | ((j & 0b00011100) << 11) | ((j & 0b00000011) << 6);
        //DCU.CLUT[j] = j;
        //*(clut_memory_test+j) = j;
    }

    // selected layer for eGUI

    i = 0;
    DCU.LAYER[i].CTRLDESCL1.B.HEIGHT = D4DLCDHWFB_Y_MAX; //Height of layer
    DCU.LAYER[i].CTRLDESCL1.B.WIDTH = D4DLCDHWFB_X_MAX; //Width of layer
    DCU.LAYER[i].CTRLDESCL2.B.POSX = D4DLCDHWFB_X_OFFSET; //Place this distance from left
    DCU.LAYER[i].CTRLDESCL2.B.POSY = D4DLCDHWFB_Y_OFFSET; //Place this distance from top
    DCU.LAYER[i].CTRLDESCL3.R = (uint32_t) D4DLCDHWFB_START_ADDRESS; //Address of image
    DCU.LAYER[i].CTRLDESCL4.B.LUOFFS = 0; //4bpp LUTs
    DCU.LAYER[i].CTRLDESCL4.B.DATA_SEL = 1; // 1 = CLUT mode

    // b0110 = 32 bpp (ARGB8888)
    // b0100 = 16 bpp (RGB565)
    // b1011 = 16 bpp (ARGB1555)
    // b1100 = 16 bpp (ARGB4444)
    // b0011 = 8 bpp

#if D4DLCDHWFB_BPP_BYTE == 4
    DCU.LAYER[i].CTRLDESCL4.B.BPP = 6; // b0110 = 32 bpp (ARGB8888)
#elif D4DLCDHWFB_BPP_BYTE == 2
    DCU.LAYER[i].CTRLDESCL4.B.BPP = 4; // b0100 = 16 bpp (RGB565)
#elif D4DLCDHWFB_BPP_BYTE == 1
    DCU.LAYER[i].CTRLDESCL4.B.BPP = 3; // b0011 = 8 bpp (RGB332)
#else
    DCU.LAYER[i].CTRLDESCL4.B.BPP = 4; // b0100 = 16 bpp (RGB565)
#endif

    //Layer blending and enable
    DCU.LAYER[i].CTRLDESCL4.B.TRANS = 0x80; //0% transparent
    DCU.LAYER[i].CTRLDESCL4.B.BB = 1; //Chroma keying
    DCU.LAYER[i].CTRLDESCL4.B.AB = 1; //Blend chroma pixels

    //Chroma setting
    DCU.LAYER[i].CTRLDESCL5.B.CKMAX_R = 0; //Max red chroma
    DCU.LAYER[i].CTRLDESCL6.B.CKMIN_R = 0; //Min red chroma
    DCU.LAYER[i].CTRLDESCL5.B.CKMAX_G = 0; //Max green chroma
    DCU.LAYER[i].CTRLDESCL6.B.CKMIN_G = 0; //Min green chroma
    DCU.LAYER[i].CTRLDESCL5.B.CKMAX_B = 0; //Max blue chroma
    DCU.LAYER[i].CTRLDESCL6.B.CKMIN_B = 0; //Min blue chroma
    //Tile settings
    DCU.LAYER[i].CTRLDESCL7.B.TILE_VER_SIZE = 0;
    DCU.LAYER[i].CTRLDESCL7.B.TILE_HOR_SIZE = 0;
    //Enable layer
    DCU.LAYER[i].CTRLDESCL4.B.EN = 1; //Turn on the layer

    DCU.DCU_MODE.B.DCU_MODE = 1; // Standard Mode
    DCU.DCU_MODE.B.RASTER_EN = 1; //Enable Raster signals


    //EDT_setup();
    TWR_PXD10_SIUL_setup();
    TWR_LCD_RGB_setup();
    return 1;

}

//----------------------------------------------------------------------------
static void TWR_PXD10_SIUL_setup(void)
{
    //DCU pins
    SIU.PCR[0].R = 0x0600; //R0
    SIU.PCR[1].R = 0x0600;
    SIU.PCR[2].R = 0x0600;
    SIU.PCR[3].R = 0x0600;
    SIU.PCR[4].R = 0x0600;
    SIU.PCR[5].R = 0x0600;
    SIU.PCR[6].R = 0x0600;
    SIU.PCR[7].R = 0x0600;
    SIU.PCR[8].R = 0x0600; //G0
    SIU.PCR[9].R = 0x0600;
    SIU.PCR[10].R = 0x0600;
    SIU.PCR[11].R = 0x0600;
    SIU.PCR[12].R = 0x0600;
    SIU.PCR[13].R = 0x0600;
    SIU.PCR[14].R = 0x0604;
    SIU.PCR[15].R = 0x0600;
    SIU.PCR[86].R = 0x0604; //B0
    SIU.PCR[87].R = 0x0600;
    SIU.PCR[88].R = 0x0604;
    SIU.PCR[89].R = 0x0600;
    SIU.PCR[90].R = 0x0604;
    SIU.PCR[91].R = 0x0600;
    SIU.PCR[92].R = 0x0604;
    SIU.PCR[93].R = 0x0600;
    SIU.PCR[94].R = 0x0604; //V_SYNC
    SIU.PCR[95].R = 0x0600; //H_SYNC
    SIU.PCR[96].R = 0x0003; //DE   :  noDE 0x0003   DE 0x0403
    SIU.PCR[97].R = 0x0600; //P_CLK
}

//----------------------------------------------------------------------------
static void TWR_LCD_RGB_setup(void)
{
    //rev B
    DCU.DISP_SIZE.B.DELTA_X = 30; // X = 30 * 16 = 480
    DCU.HSYN_PARA.B.BP_H = 2; // Horizontal back porch
    DCU.HSYN_PARA.B.PW_H = 41; // Horizontal sync pulse
    DCU.HSYN_PARA.B.FP_H = 20; // Horizontal front porch

    DCU.DISP_SIZE.B.DELTA_Y = 272; // Y = 272
    DCU.VSYN_PARA.B.BP_V = 2; // Vertical back porch
    DCU.VSYN_PARA.B.PW_V = 10; // Vertical sync pulse
    DCU.VSYN_PARA.B.FP_V = 2; // Vertical front porch

    DCU.DIV_RATIO.R = (DCU_PCLK / PCLK_MAX); //Clock divider based on DCU clock frequency
    DCU.SYN_POL.R = 0x00000003; // Synchro neg.
}

//----------------------------------------------------------------------------
static void EDT_setup(void)
{
    DCU.DISP_SIZE.B.DELTA_Y = 240; // Y = 240
    DCU.DISP_SIZE.B.DELTA_X = 20; // X = 20 * 16 = 320
    DCU.HSYN_PARA.B.BP_H = 34; // Horizontal back porch
    DCU.HSYN_PARA.B.PW_H = 34; // Horizontal sync pulse
    DCU.HSYN_PARA.B.FP_H = 20; // Horizontal front porch

    DCU.VSYN_PARA.B.BP_V = 9; // Vertical back porch
    DCU.VSYN_PARA.B.PW_V = 9; // Vertical sync pulse
    DCU.VSYN_PARA.B.FP_V = 4; // Vertical front porch

    DCU.DIV_RATIO.R = (DCU_PCLK / PCLK_MAX) - 1; //Clock divider based on DCU clock frequency
    DCU.SYN_POL.R = 0x00000043; // Synchro neg.
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_DeInit_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function is used for deinitialization of this low level driver
//
// PARAMETERS:  none
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_DeInit_PxDcuFb(void)
{
    return 1;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_SendDataWord_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function send the one 16 bit variable into LCD
//
// PARAMETERS:  unsigned long addr  		address to write data
//							unsigned short value    variable to send
//
// RETURNS:     result: 1 - Success
//                      0 - Failed
//-----------------------------------------------------------------------------

static void D4DLCDHW_WriteData_PxDcuFb(unsigned long addr, D4D_COLOR value)
{

#if TWR_PXD10_REV ==  TWR_PXD10_REV_B
    *((D4D_COLOR*) addr) = value;
#else
    D4D_COLOR value_b, value_r;

#if D4DLCDHWFB_BPP_BYTE == 2  // b0100 = 16 bpp (RGB565)
    D4D_COLOR value_b, value_r;
    value_b = value & 0b0000000000011111;
    value_r = (value & 0b1111100000000000) >> 11;
    *((D4D_COLOR*)addr) = (value & 0b0000011111100000) | (value_b <<11) | value_r;

#elif D4DLCDHWFB_BPP_BYTE == 1
    value_b = (value & 0b00000011) << 6; // need 3 bits only
    value_r = (value & 0b11000000) >> 6; // need 3 bits
    *((D4D_COLOR*)addr) = (value & 0b00011100) | value_b | value_r;

#endif  //END BPP_BYTE
#endif  //END REV
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_ReadDataWord_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
//
// PARAMETERS:  unsigned long addr  		address to read data
//
// RETURNS:     unsigned short - the readed value
//
//-----------------------------------------------------------------------------
static D4D_COLOR D4DLCDHW_ReadData_PxDcuFb(unsigned long addr)
{
    return *((D4D_COLOR*) addr);
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_GetFbDescriptor_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: The function return the pointer on filled frame buffer descriptor
//
// PARAMETERS:  none
//
// RETURNS:     D4DLCD_FRAMEBUFF_DESC* - pointer on frame buffer descriptor
//
//-----------------------------------------------------------------------------
static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_PxDcuFb(void)
{
    return (D4DLCD_FRAMEBUFF_DESC*) &d4dlcdhw_px_dcu_desc;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCDHW_PinCtl_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: allows control GPIO pins for LCD conrol purposes
//
// PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
//              D4DHW_PIN_STATE setState - Pin action
// RETURNS:     for Get action retuns the pin value
//-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_PinCtl_PxDcuFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
{
    (void) pinId;
    (void) setState;
    return 0;
}

//-----------------------------------------------------------------------------
// FUNCTION:    D4DLCD_FlushBuffer_PxDcuFb
// SCOPE:       Low Level Driver API function
// DESCRIPTION: For buffered low level interfaces is used to inform
//              driver the complete object is drawed and pending pixels should be flushed
//
// PARAMETERS:  none
//
// RETURNS:     none
//-----------------------------------------------------------------------------
  static void D4DLCD_FlushBuffer_PxDcuFb(D4DLCD_FLUSH_MODE mode)
{
	D4D_UNUSED(mode);
}

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_px_dcu_fb_ID)
