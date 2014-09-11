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
* @file      d4dlcdhw_k70_lcdc.c
*
* @author     Petr Gargulak
*
* @version   0.0.18.0
*
* @date      Oct-2-2013
*
* @brief     D4D driver - k70_lcdc hardware lcd driver source c file
*
******************************************************************************/

#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR


// identification string of driver - must be same as name D4DTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4dlcdhw_k70_lcdc_ID 1


// copilation enable preprocessor condition
// the string d4dtch_k70_lcdc_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_k70_lcdc_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/k70_lcdc/d4dlcdhw_k70_lcdc.h"
  #include "low_level_drivers/LCD/lcd_hw_interface/k70_lcdc/twr_rgb_rev_a/d4dlcdhw_twrrgba.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  #define MOUSE_CURSOR_GRPHCW_PRTY_KEYCLR_MASK    0x00FFFFFF
  #define MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_MASK   0x0F000000
  #define MOUSE_CURSOR_GRPHCW_PRTY_CHNG_MASK      0x80000000
  #define MOUSE_CURSOR_GRPHCW_PRTY_SHOW_MASK      0x40000000

  #define MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_SHIFT  24

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_K70LCDC(void);
  static unsigned char D4DLCDHW_DeInit_K70LCDC(void);
  static void D4DLCDHW_WriteData_K70LCDC(unsigned long addr, D4D_COLOR value);
  static D4D_COLOR D4DLCDHW_ReadData_K70LCDC(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_K70LCDC(void);
  static unsigned char D4DLCDHW_PinCtl_K70LCDC(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCDHW_FlushBuffer_K70LCDC(D4DLCD_FLUSH_MODE mode);

#ifdef D4D_LLD_MOUSE
  static unsigned char D4DMOUSE_Init_K70LCDC(void);
  static void D4DMOUSE_SetCoor_K70LCDC(unsigned short x, unsigned short y);
  static void D4DMOUSE_SetPointerBmp_K70LCDC(void** pPntrData, D4D_INDEX cnt, D4D_COLOR keyColor);
  static void D4DMOUSE_SetPointer_K70LCDC(D4D_INDEX ix, D4D_BOOL show);
  static unsigned char D4DMOUSE_DeInit_K70LCDC(void);
  static D4DMOUSE_DESC* D4DMOUSE_GetDescriptor_K70LCDC(void);
#endif


#if D4DLCDHWFB_DOUBLE_BUFFER
  static void D4DLCDHW_CopyBuffers_K70LCDC(unsigned long second2first);
  void D4DLCDHW_ISR_K70DMA(void);
#endif


  void D4DLCDHW_ISR_K70LCDC(void);




  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  const D4DLCDHWFB_FUNCTIONS d4dlcdhw_k70_lcdc =
  {
    D4DLCDHW_Init_K70LCDC,
    D4DLCDHW_WriteData_K70LCDC,
    D4DLCDHW_ReadData_K70LCDC,
    D4DLCDHW_GetFbDescriptor_K70LCDC,
    D4DLCDHW_PinCtl_K70LCDC,
    D4DLCDHW_FlushBuffer_K70LCDC,
    D4DLCDHW_DeInit_K70LCDC
  };

 #ifdef D4D_LLD_MOUSE
  const D4DMOUSE_FUNCTIONS d4dmouse_k70_lcdc =
  {
    D4DMOUSE_Init_K70LCDC,
    D4DMOUSE_SetCoor_K70LCDC,
    D4DMOUSE_SetPointerBmp_K70LCDC,
    D4DMOUSE_SetPointer_K70LCDC,
    D4DMOUSE_GetDescriptor_K70LCDC,
    D4DMOUSE_DeInit_K70LCDC
  };
#endif
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

    static D4DLCD_FRAMEBUFF_DESC d4dlcdhw_k70_lcdc_desc =
    {
      D4DLCDHWFB_START_ADDRESS,
      D4DLCDHWFB_X_MAX,
      D4DLCDHWFB_Y_MAX,
      D4DLCDHWFB_BPP_BYTE
    };

#ifdef D4D_LLD_MOUSE
    static const D4DMOUSE_DESC d4dmouse_descriptor =
    {
      D4DMOUSE_CURSOR_SIZE_X,
      D4DMOUSE_CURSOR_SIZE_Y,
      D4DMOUSE_CURSOR_BMPFORMAT
    };
#endif

    static volatile sLWord enableWrite = 0;

#ifdef D4D_LLD_MOUSE
    static volatile LWord graphicWindowPrty = (D4D_COLOR_RGB(0x3F, 0x3F, 0x3F) & MOUSE_CURSOR_GRPHCW_PRTY_KEYCLR_MASK);
    static volatile LWord* cursorBmpPtr = NULL;
    static volatile LWord cursorOffsets[D4D_MOUSE_CURSOR_TYPE_CNT];
#endif

#if D4DLCDHWFB_DOUBLE_BUFFER
    static unsigned long fb_start_addr_2nd = D4DLCDHWFB_START_ADDRESS_2ND;
    static volatile signed long fb_draw_2nd = 0;
    static volatile unsigned long fb_switch = 0;
    static volatile unsigned long lastUpdateStart, lastUpdateEnd;
#endif
  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_Init_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
static unsigned char D4DLCDHW_Init_K70LCDC(void)
{
  unsigned long * pTmp;
  unsigned long i;
  /* Enable LCDC and DDR Modules */
  SIM_SCGC3 |= SIM_SCGC3_LCDC_MASK;

#ifdef LCDC_DEBUG
  PORTB_PCR10 = PORT_PCR_MUX(1);
  PORTB_PCR11 = PORT_PCR_MUX(1);
  PORTB_PCR16 = PORT_PCR_MUX(1);
  PORTB_PCR17 = PORT_PCR_MUX(1);
  PORTB_PCR18 = PORT_PCR_MUX(1);

  GPIOB_PDDR |= (1<<10) | (1<<11) | (1<<16) | (1<<17) | (1<<18);
  GPIOB_PCOR |= (1<<10) | (1<<11) | (1<<16) | (1<<17) | (1<<18);
#endif

  if(D4D_LLD_LCD_HW_K70LCDC.D4DLCDHWFBK70LCDC_InitPanel == NULL)
    return 0;


  // Registrer interrupt service routines
  #ifdef D4D_OS_MQX
    if(!_int_install_kernel_isr(INT_LCD, D4DLCDHW_ISR_K70LCDC))
      return 0;
    _bsp_int_init(INT_LCD, 3, 0, TRUE);
  #else
    NVIC_IP(INT_LCD) = 3;
    NVIC_ISER(INT_LCD / 32) |= (1 << (INT_LCD % 32));
    #warning The LCDC K70 driver is is using interrupts, please registry the "D4DLCDHW_ISR_K70LCDC" on interrupt vector of LCDC (113).
  #endif


  #if D4DLCDHWFB_START_ADDRESS == 0
    // Allocate frame buffer memory
    if(d4dlcdhw_k70_lcdc_desc.fb_start_addr)
    {
      D4D_MemFree((void*)d4dlcdhw_k70_lcdc_desc.fb_start_addr);
      d4dlcdhw_k70_lcdc_desc.fb_start_addr = 0;
    }
    d4dlcdhw_k70_lcdc_desc.fb_start_addr = (unsigned long)D4D_MemAlloc(D4DLCDHWFB_FRAMEBUFFER_SIZE);
    if(!d4dlcdhw_k70_lcdc_desc.fb_start_addr)
      return 0;
  #endif

    // Clear the previous contents of buffer
    pTmp = (unsigned long*)d4dlcdhw_k70_lcdc_desc.fb_start_addr;
    for(i=0;i<(D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX);i++)
      *(pTmp++) = 0; // Black color

  #if D4DLCDHWFB_DOUBLE_BUFFER
    #if D4DLCDHWFB_START_ADDRESS_2ND == 0
      // Allocate frame buffer memory
      if(fb_start_addr_2nd)
      {
        D4D_MemFree((void*)fb_start_addr_2nd);
        fb_start_addr_2nd = 0;
      }
      fb_start_addr_2nd = (unsigned long)D4D_MemAlloc(D4DLCDHWFB_FRAMEBUFFER_SIZE);
      if(!fb_start_addr_2nd)
        return 0;
    #endif
      fb_switch = 0;
      fb_draw_2nd = fb_start_addr_2nd - d4dlcdhw_k70_lcdc_desc.fb_start_addr;
      lastUpdateStart = 0xFFFFFFFF;
      lastUpdateEnd = 0;

#if D4DLCDHWFB_DMA_ENABLE
      SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
      SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;
      #if D4DLCDHWFB_DMA_CHANNEL < 16
        SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;
        DMAMUX0_CHCFG(D4DLCDHWFB_DMA_CHANNEL) = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(63);
      #else
        SIM_SCGC6 |= SIM_SCGC6_DMAMUX1_MASK;
        DMAMUX1_CHCFG(D4DLCDHWFB_DMA_CHANNEL - 16) = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(63);
      #endif
      // Copy the buffer

      // set up the channel priority
      *(unsigned char*)((unsigned long)&DMA_DCHPRI3 + (3 - (D4DLCDHWFB_DMA_CHANNEL % 4)) + ((D4DLCDHWFB_DMA_CHANNEL / 4) * 4)) = D4DLCDHWFB_DMA_CHPRI;

      DMA_SOFF(D4DLCDHWFB_DMA_CHANNEL)           = 16;
      DMA_NBYTES_MLNO(D4DLCDHWFB_DMA_CHANNEL)    = 16;
      DMA_DOFF(D4DLCDHWFB_DMA_CHANNEL)           = 16;
      DMA_ATTR(D4DLCDHWFB_DMA_CHANNEL)           = DMA_ATTR_SMOD(0) | DMA_ATTR_SSIZE(4) | DMA_ATTR_DMOD(0) | DMA_ATTR_DSIZE(4);   // no circular addressing S&D, 32 bit S&D
      DMA_SLAST(D4DLCDHWFB_DMA_CHANNEL)          = 0;         // source address will continue X times to allow defined multiply length of buffer // to do
      DMA_DLAST_SGA(D4DLCDHWFB_DMA_CHANNEL)      = 0;                       // no final last adjustment ( does not move )
      DMA_CSR(D4DLCDHWFB_DMA_CHANNEL)            = DMA_CSR_INTMAJOR_MASK;  // major interrupt when done

      // Registrer interrupt service routines
      #ifdef D4D_OS_MQX
        if(!_int_install_kernel_isr(D4DLCDHWFB_DMA_INTVECT, D4DLCDHW_ISR_K70DMA))
        {
          D4DLCDHW_DeInit_K70LCDC();
          return 0;
        }

        _bsp_int_init(D4DLCDHWFB_DMA_INTVECT, 3, 0, TRUE);
      #else
        NVIC_IP(D4DLCDHWFB_DMA_INTVECT) = 3;
        NVIC_ISER(D4DLCDHWFB_DMA_INTVECT / 32) |= (1 << (D4DLCDHWFB_DMA_INTVECT % 32));
        #warning The LCDC K70 driver is is using interrupts, please registry the "D4DLCDHW_ISR_K70DMA" on right interrupt vector for selected DMA channel by D4DLCDHWFB_DMA_CHANNEL.
      #endif

#endif
      // Clear the previous contents of buffer
    pTmp = (unsigned long*)fb_start_addr_2nd;
    for(i=0;i<(D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX);i++)
      *(pTmp++) = 0; // Black color

  #endif

  enableWrite = 1;
#ifdef LCDC_DEBUG
  GPIOB_PSOR |= 1<<10;
#endif

  #if D4DLCDHWFB_MIRROWED == 1
    LCDC_LSSAR = d4dlcdhw_k70_lcdc_desc.fb_start_addr + D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX * D4DLCDHWFB_BPP_BYTE;
  #else
    LCDC_LSSAR = d4dlcdhw_k70_lcdc_desc.fb_start_addr;
  #endif

#ifdef LCDC_DEBUG
  GPIOB_PSOR |= 1<<11;
#endif

  LCDC_LSR =   (D4DLCDHWFB_X_MAX / 16) << 20 | (D4DLCDHWFB_Y_MAX);

  // set LCD virtual page width
  LCDC_LVPWR = LCDC_LVPWR_VPW( D4DLCDHWFB_X_MAX / (4 / D4DLCDHWFB_BPP_BYTE));

  // set LCD cursor positon & settings (turn off)
  LCDC_LCPR = 0;
  LCDC_LCWHB = 0;

  // set LCD panning offset
  LCDC_LPOR = 0;

  // set LCD interrupt configuration
  LCDC_LICR = 0;

  // set LCD interrupt enable
  LCDC_LIER = LCDC_LIER_EOF_EN_MASK;

  //Set background plane DMA to burst mode
  LCDC_LDCR &= ~(LCDC_LDCR_BURST_MASK);

  D4D_LLD_LCD_HW_K70LCDC.D4DLCDHWFBK70LCDC_InitPanel();

  #if D4DLCDHWFB_MIRROWED == 1
    LCDC_LPCR |= LCDC_LPCR_REV_VS_MASK;
  #endif
      /* Enable LCD */
  SIM_MCR|=SIM_MCR_LCDSTART_MASK;


      return 1;
}


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_DeInit_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_DeInit_K70LCDC(void)
  {
    #if D4DLCDHWFB_START_ADDRESS == 0
    if(d4dlcdhw_k70_lcdc_desc.fb_start_addr)
    {
      D4D_MemFree((void*)d4dlcdhw_k70_lcdc_desc.fb_start_addr);
      d4dlcdhw_k70_lcdc_desc.fb_start_addr = 0;
    }
    #endif

    #if D4DLCDHWFB_DOUBLE_BUFFER
      #if D4DLCDHWFB_START_ADDRESS_2ND == 0
      if(fb_start_addr_2nd)
      {
        D4D_MemFree((void*)fb_start_addr_2nd);
        fb_start_addr_2nd = 0;
      }
      #endif
    #endif

    return 1;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_SendDataWord_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD
  //
  // PARAMETERS:  unsigned long addr  		address to write data
  //							unsigned short value    variable to send
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_WriteData_K70LCDC(unsigned long addr, D4D_COLOR value)
  {
    unsigned long data;

    #if D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB888
      data = value;
    #else
      data = D4D_COLOR_RGB888(D4D_COLOR_GET_R(value), D4D_COLOR_GET_G(value), D4D_COLOR_GET_B(value));
    #endif

    while(enableWrite <= 0);

    #if D4DLCDHWFB_DOUBLE_BUFFER
      addr += fb_draw_2nd;



      if(addr > lastUpdateEnd)
      {
        // 16 Byte alligment
        if(addr & 0x0000000f)
          lastUpdateEnd = (addr & 0xFFFFFFF0) + 0x00000010;
        else
          lastUpdateEnd = addr;
      }

      if(addr < lastUpdateStart)
      {
        // 16 Byte alligment
          lastUpdateStart = addr & 0xFFFFFFF0;
      }

    #endif

    *((unsigned long*)addr) = data;
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ReadDataWord_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)
  //
  // PARAMETERS:  unsigned long addr  		address to read data
  //
  // RETURNS:     unsigned short - the readed value
  //
  //-----------------------------------------------------------------------------
  static D4D_COLOR D4DLCDHW_ReadData_K70LCDC(unsigned long addr)
  {
    #if D4DLCDHWFB_DOUBLE_BUFFER
    while(enableWrite <= 0);
    addr += fb_draw_2nd;
    // to do pocat az sa dokonci kopirovani - nebo jeste lepe pokud neni dokoncene kopirovani brat s jineho
    #endif

    #if D4D_COLOR_SYSTEM == D4D_COLOR_SYSTEM_RGB888
        return *((unsigned long*)addr);
    #else
        unsigned long value = *((unsigned long*)addr);
        return D4D_COLOR_RGB(D4D_COLOR888_GET_R(value), D4D_COLOR888_GET_G(value), D4D_COLOR888_GET_B(value));
    #endif
  }


  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_GetFbDescriptor_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function return the pointer on filled frame buffer descriptor
  //
  // PARAMETERS:  none
  //
  // RETURNS:     D4DLCD_FRAMEBUFF_DESC* - pointer on frame buffer descriptor
  //
  //-----------------------------------------------------------------------------
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_K70LCDC(void)
  {
    return (D4DLCD_FRAMEBUFF_DESC*) &d4dlcdhw_k70_lcdc_desc;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_PinCtl_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4DLCDHW_PINS pinId - Pin identification
  //              D4DHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4DLCDHW_PinCtl_K70LCDC(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
    (void)pinId;
    (void)setState;
    return 0;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCD_FlushBuffer_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For buffered low level interfaces is used to inform
  //              driver the complete object is drawed and pending pixels should be flushed
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_FlushBuffer_K70LCDC(D4DLCD_FLUSH_MODE mode)
  {

#if (D4DLCDHWFB_DOUBLE_BUFFER == 0) && (D4DLCDHWFB_CACHE_FLUSH_ENABLE == 0)
    D4D_UNUSED(mode);
#endif

#if D4DLCDHWFB_CACHE_FLUSH_ENABLE || D4DLCDHWFB_DOUBLE_BUFFER
    if(mode == D4DLCD_FLSH_SCR_END || mode == D4DLCD_FLSH_FORCE)
    {
#if D4DLCDHWFB_CACHE_FLUSH_ENABLE
      LMEM_PSCCR |= LMEM_PSCCR_GO_MASK | LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK;
      /* wait until the command completes */
      while (LMEM_PSCCR & LMEM_PSCCR_GO_MASK)
      {};
#endif
#if D4DLCDHWFB_DOUBLE_BUFFER
      enableWrite = 0;
#ifdef LCDC_DEBUG
      GPIOB_PCOR |= 1<<10;
      GPIOB_PTOR |= 1<<16;
#endif

      D4DLCDHW_CopyBuffers_K70LCDC(fb_draw_2nd);
      fb_switch = 1;
#endif
    }
#endif
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_ISR_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: For interrupt handler of LCDC events
  //
  // PARAMETERS:  none (pData)
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------

  void D4DLCDHW_ISR_K70LCDC(void)
  {
#ifdef LCDC_DEBUG
    GPIOB_PTOR |= 1<<18;
#endif
    LWord status = LCDC_LISR;

    if((LCDC_LIER & LCDC_LIER_EOF_EN_MASK) && (status & LCDC_LISR_EOF_MASK))
    {
      // End of drawing screen


#if D4DLCDHWFB_DOUBLE_BUFFER
      if(fb_switch)
      {
        fb_switch = 0;

        // Switch the frame buffer
        if(!fb_draw_2nd)
        {
          fb_draw_2nd = fb_start_addr_2nd - d4dlcdhw_k70_lcdc_desc.fb_start_addr;

          #ifdef LCDC_DEBUG
            GPIOB_PSOR |= 1<<11;
          #endif

          #if D4DLCDHWFB_MIRROWED == 1
            LCDC_LSSAR = d4dlcdhw_k70_lcdc_desc.fb_start_addr + D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX * D4DLCDHWFB_BPP_BYTE;
          #else
            LCDC_LSSAR = d4dlcdhw_k70_lcdc_desc.fb_start_addr;
          #endif
        }
        else
        {
          fb_draw_2nd = 0;
          #ifdef LCDC_DEBUG
            GPIOB_PCOR |= 1<<11;
          #endif

          #if D4DLCDHWFB_MIRROWED == 1
            LCDC_LSSAR = fb_start_addr_2nd + D4DLCDHWFB_X_MAX * D4DLCDHWFB_Y_MAX * D4DLCDHWFB_BPP_BYTE;
          #else
            LCDC_LSSAR = fb_start_addr_2nd;
          #endif
        }
        if(enableWrite < 0)
          enableWrite = 1;
        else
          enableWrite = -1;
      }

#endif

#if (D4DLCDHWFB_DOUBLE_BUFFER == 0) || defined(D4D_LLD_MOUSE)

#if (D4DLCDHWFB_DOUBLE_BUFFER == 0)
      enableWrite = 1;
#endif

      #ifdef LCDC_DEBUG
        GPIOB_PSOR |= 1<<10;
      #endif

      //NOTE: LICR[INTCON] must be 1 to use the BOF interrupt.
      LCDC_LICR = LCDC_LICR_INTCON_MASK /*| LCDC_LICR_INTSYN_MASK*/;
      LCDC_LIER &= ~LCDC_LIER_EOF_EN_MASK;
      LCDC_LIER |= LCDC_LIER_BOF_EN_MASK;

#endif
    }



    else if((LCDC_LIER & LCDC_LIER_BOF_EN_MASK) && (status & LCDC_LISR_BOF_MASK))
    {
#if (D4DLCDHWFB_DOUBLE_BUFFER == 0)
      enableWrite = 0;
#endif
      #ifdef LCDC_DEBUG
        GPIOB_PCOR |= 1<<10;
      #endif

      //NOTE: LICR[INTCON] must be 0 to use the EOF interrupt.
      LCDC_LICR = LCDC_LICR_INTSYN_MASK;
      LCDC_LIER &= ~LCDC_LIER_BOF_EN_MASK;
      LCDC_LIER |= LCDC_LIER_EOF_EN_MASK;
    }


#if defined(D4D_LLD_MOUSE)
    if((LCDC_LIER & LCDC_LIER_GW_EOF_EN_MASK) && (status & LCDC_LISR_GW_EOF_MASK))
    {
      unsigned char alpha = (LCDC_LGWCR & LCDC_LGWCR_GWAV_MASK)>> LCDC_LGWCR_GWAV_SHIFT ;

      // Start of drawing screen
      if(graphicWindowPrty & MOUSE_CURSOR_GRPHCW_PRTY_CHNG_MASK)
      {
        unsigned char r,b,g;
        // Change the cursor pointer
        SIM_MCR &= ~SIM_MCR_LCDSTART_MASK;

        r = D4D_COLOR_GET_R(graphicWindowPrty) & 0x3F;
        g = D4D_COLOR_GET_G(graphicWindowPrty) & 0x3F;
        b = D4D_COLOR_GET_B(graphicWindowPrty) & 0x3F;

        // Enable graphic window
        // Change KeyColor
        //  set LCD graphic window control
        LCDC_LGWCR &= ~(LCDC_LGWCR_GWCKR_MASK | LCDC_LGWCR_GWCKG_MASK | LCDC_LGWCR_GWCKB_MASK);
        LCDC_LGWCR |= LCDC_LGWCR_GWCKR(r) | LCDC_LGWCR_GWCKG(g) | LCDC_LGWCR_GWCKB(b);// color key
        SIM_MCR |= SIM_MCR_LCDSTART_MASK;

        graphicWindowPrty &= ~MOUSE_CURSOR_GRPHCW_PRTY_CHNG_MASK;
      }

      LCDC_LGWSAR = (unsigned long)((char*)cursorBmpPtr + cursorOffsets[(graphicWindowPrty & MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_MASK) >> MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_SHIFT]);
      LCDC_LIER &= ~LCDC_LIER_GW_EOF_EN_MASK;
//      LCDC_LGWCR &= ~LCDC_LGWCR_GWAV_MASK;
//
//      if(graphicWindowPrty & MOUSE_CURSOR_GRPHCW_PRTY_SHOW_MASK)
//      {
//        // make visible cursor
//        if(alpha > 0xF7)
//        {
//          alpha = 0xFF;
//          LCDC_LIER &= ~LCDC_LIER_GW_EOF_EN_MASK;
//        }
//        else
//        {
//          alpha += 8;
//        }
//      }else
//      {
//        // make visible cursor
//        if(alpha < 8)
//        {
//          alpha = 0;
//          LCDC_LIER &= ~LCDC_LIER_GW_EOF_EN_MASK;
//        }
//        else
//        {
//          alpha -= 8;
//        }
//      }
//      LCDC_LGWCR |= LCDC_LGWCR_GWAV(alpha);




    }
#endif

  }

  #if D4DLCDHWFB_DOUBLE_BUFFER

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DLCDHW_CopyBuffers_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function update the last showed buffer with current data
  //
  // PARAMETERS:  none
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DLCDHW_CopyBuffers_K70LCDC(unsigned long second2first)
  {
    LWord* pSource;
    LWord* pDest;
    LWord cnt = lastUpdateEnd;
     cnt -= lastUpdateStart;

    pSource = (LWord*)lastUpdateStart;
    if(second2first)
      pDest = (LWord*)(d4dlcdhw_k70_lcdc_desc.fb_start_addr + (lastUpdateStart - fb_start_addr_2nd));
    else
      pDest = (LWord*)(fb_start_addr_2nd + (lastUpdateStart - d4dlcdhw_k70_lcdc_desc.fb_start_addr));


    #if D4DLCDHWFB_DMA_ENABLE
      DMA_SADDR(D4DLCDHWFB_DMA_CHANNEL) = (LWord)pSource;
      DMA_DADDR(D4DLCDHWFB_DMA_CHANNEL) = (LWord)pDest;

      DMA_CITER_ELINKNO(D4DLCDHWFB_DMA_CHANNEL)  = cnt / 16;
      DMA_BITER_ELINKNO(D4DLCDHWFB_DMA_CHANNEL)  = cnt / 16;

      DMA_ERQ   |=  1<<D4DLCDHWFB_DMA_CHANNEL;
      DMA_SSRT =  D4DLCDHWFB_DMA_CHANNEL;

      if(DMA_ES)
      {
        DMA_CERR = DMA_CERR_CAEI_MASK;
        enableWrite = 1;
      }
    #else
      cnt /= 4;

      while(cnt--)
        *pDest++ = *pSource++;

      if(enableWrite < 0)
        enableWrite = 1;
      else
        enableWrite = -1;

      #ifdef LCDC_DEBUG
        GPIOB_PSOR |= 1<<10;
      #endif

    #endif
    lastUpdateStart = 0xFFFFFFFF;
    lastUpdateEnd = 0;
  }

    #if D4DLCDHWFB_DMA_ENABLE
      //-----------------------------------------------------------------------------
      // FUNCTION:    D4DLCDHW_ISR_K70DMA
      // SCOPE:       Low Level Driver API function
      // DESCRIPTION: For interrupt handler of LCDC events
      //
      // PARAMETERS:  none (pData)
      //
      // RETURNS:     none
      //-----------------------------------------------------------------------------

      void D4DLCDHW_ISR_K70DMA(void)
      {
        #ifdef LCDC_DEBUG
          GPIOB_PTOR |= 1<<17;
        #endif

        if(DMA_INT & (1<<D4DLCDHWFB_DMA_CHANNEL))
        {
          DMA_INT |= (1<<D4DLCDHWFB_DMA_CHANNEL);
          DMA_ERQ &= ~(1<<D4DLCDHWFB_DMA_CHANNEL);
          if(enableWrite < 0)
            enableWrite = 1;
          else
            enableWrite = -1;
          #ifdef LCDC_DEBUG
            GPIOB_PSOR |= 1<<10;
          #endif
        }
      }
    #endif

  #endif

#ifdef D4D_LLD_MOUSE

  /**************************************************************//*!
  *
  * Mouse cursor section
  *
  ******************************************************************/

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DMOUSE_Init_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this mouse low level driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DMOUSE_Init_K70LCDC(void)
  {
    if(!(SIM_SCGC3 & SIM_SCGC3_LCDC_MASK))
      return D4D_FALSE;

    SIM_MCR &= ~SIM_MCR_LCDSTART_MASK;

    // set LCD graphic window start address - at the momnet there is no know any picture data
    LCDC_LGWSAR = 0x20000000;

    // set LCD graphic window size
    LCDC_LGWSR = LCDC_LGWSR_GWW(1) | LCDC_LGWSR_GWH(16);

    //  set LCD graphic window virtual page width
    LCDC_LGWVPWR = 16;

    //  set LCD graphic window panning offset
    LCDC_LGWPOR = 0;

    //  set LCD graphic window position
    LCDC_LGWPR = LCDC_LGWPR_GWXP(0) | LCDC_LGWPR_GWYP(0);

    //Set graphic window DMA to burst mode
    LCDC_LGWDCR &= ~(LCDC_LGWDCR_GWBT_MASK);

    graphicWindowPrty = (D4D_COLOR_RGB(0x3F, 0x3F, 0x3F) & MOUSE_CURSOR_GRPHCW_PRTY_KEYCLR_MASK); // is also used to as flag to change
    cursorBmpPtr = NULL;

    for(int i=0; i < D4D_MOUSE_CURSOR_TYPE_CNT;i++)
      cursorOffsets[i] = 0;


    // Enable graphic window
    // Change KeyColor
    //  set LCD graphic window control
    LCDC_LGWCR = LCDC_LGWCR_GWAV(1) | // alpha-transparent
      LCDC_LGWCR_GWCKE_MASK | // enable color keying
      LCDC_LGWCR_GWE_MASK | // enable graphic window
      //LCDC_LGWCR_GW_RVS_MASK | // reverse vertical scan
      LCDC_LGWCR_GWCKR(0x3F) | // color key
      LCDC_LGWCR_GWCKG(0x3F) |
      LCDC_LGWCR_GWCKB(0x3F);

    SIM_MCR |= SIM_MCR_LCDSTART_MASK;
    return D4D_TRUE;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DMOUSE_SetCoor_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for change/set the coordination of cursor
  //
  // PARAMETERS:  x- coordination in axis X
  //              y- coordination in axis Y
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DMOUSE_SetCoor_K70LCDC(unsigned short x, unsigned short y)
  {
    //  set LCD graphic window position
    LCDC_LGWPR = LCDC_LGWPR_GWXP(x) | LCDC_LGWPR_GWYP(y);
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DMOUSE_SetPointerBmp_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for sets the new set of cursor bitmaps
  //
  // PARAMETERS:  pPntrData- pointer on graphic data (must be in 8-8-8 format)
  //              cnt - count of bitmaps
  //              keyColor - keycolor of these bitmaps
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DMOUSE_SetPointerBmp_K70LCDC(void** pPntrData, D4D_INDEX cnt, D4D_COLOR keyColor)
  {
    int uniqueBmp = 0;
    int filledArray = 0;
    if(!pPntrData)
      return;

    if(cnt > D4D_MOUSE_CURSOR_TYPE_CNT)
      return;

    // Get the count of unigue bitmaps
    for(int i=0; i < cnt;i++)
    {
      int j = i;
      if(pPntrData[i])
      {
        uniqueBmp++;

        while(j)
        {
          j--;
          if(pPntrData[i] == pPntrData[j])
          {
            uniqueBmp--;
            break;
          }
        }
      }
      cursorOffsets[i] = (uniqueBmp - 1) * D4DMOUSE_CURSOR_SIZE_X * D4DMOUSE_CURSOR_SIZE_Y * D4DLCDHWFB_BPP_BYTE;
    }

    // alloc the memory for the unique bitmaps

    if(cursorBmpPtr)
      D4D_MemFree((void*)cursorBmpPtr);

    cursorBmpPtr = D4D_MemAlloc(D4DMOUSE_CURSOR_SIZE_X * D4DMOUSE_CURSOR_SIZE_Y * D4DLCDHWFB_BPP_BYTE * uniqueBmp);

    if(!cursorBmpPtr)
      return;

    // Copy the bitmpa data to RAM (just only unique bitmaps)
    for(int i=0; i < uniqueBmp;i++)
    {
      int j = cursorOffsets[i] / (D4DMOUSE_CURSOR_SIZE_X * D4DMOUSE_CURSOR_SIZE_Y * D4DLCDHWFB_BPP_BYTE);

      if(filledArray <= j)
      {
        char* pDst = ((char*)cursorBmpPtr) + filledArray * (D4DMOUSE_CURSOR_SIZE_X * D4DMOUSE_CURSOR_SIZE_Y * D4DLCDHWFB_BPP_BYTE);
        char* pSrc = (char*)pPntrData[i];

        D4D_MemCopy(pDst, pSrc, (D4DMOUSE_CURSOR_SIZE_X * D4DMOUSE_CURSOR_SIZE_Y * D4DLCDHWFB_BPP_BYTE));
        filledArray++;
      }
    }

    graphicWindowPrty &= ~MOUSE_CURSOR_GRPHCW_PRTY_KEYCLR_MASK;
    graphicWindowPrty = MOUSE_CURSOR_GRPHCW_PRTY_CHNG_MASK | (keyColor & MOUSE_CURSOR_GRPHCW_PRTY_KEYCLR_MASK);
    LCDC_LIER |= LCDC_LIER_GW_EOF_EN_MASK;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DMOUSE_SetPointer_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for switch off/set the cursor bitmap
  //
  // PARAMETERS:  pPntrData- pointer on graphic data (must be in 8-8-8 format)
  //
  // RETURNS:     none
  //-----------------------------------------------------------------------------
  static void D4DMOUSE_SetPointer_K70LCDC(D4D_INDEX ix, D4D_BOOL show)
  {
    LWord bck_graphicWindowPrty = graphicWindowPrty;
    if(show)
    {
      graphicWindowPrty &= ~MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_MASK;
      graphicWindowPrty |= ((ix << MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_SHIFT) & MOUSE_CURSOR_GRPHCW_PRTY_CURSRIX_MASK) | MOUSE_CURSOR_GRPHCW_PRTY_SHOW_MASK;
      LCDC_LGWCR |= LCDC_LGWCR_GWAV(0xFF);
    }else
    {
      if(graphicWindowPrty & MOUSE_CURSOR_GRPHCW_PRTY_SHOW_MASK)
      {
        // switch off
        graphicWindowPrty &= ~MOUSE_CURSOR_GRPHCW_PRTY_SHOW_MASK;
        LCDC_LGWCR &= ~LCDC_LGWCR_GWAV_MASK;
      }
    }

    if(graphicWindowPrty != bck_graphicWindowPrty)
      LCDC_LIER |= LCDC_LIER_GW_EOF_EN_MASK;

  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DMOUSE_GetDescriptor_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function return pointer on Mouse Cursor descriptor
  //
  // PARAMETERS:  none
  //
  // RETURNS:     pointer to Mouse cursor
  //-----------------------------------------------------------------------------
  static D4DMOUSE_DESC* D4DMOUSE_GetDescriptor_K70LCDC(void)
  {
    return (D4DMOUSE_DESC*)&d4dmouse_descriptor;
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4DMOUSE_DeInit_K70LCDC
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinicialization of driver
  //
  // PARAMETERS:  none
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4DMOUSE_DeInit_K70LCDC(void)
  {
    return D4D_FALSE;
  }

#endif


#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_k70_lcdc_ID)
