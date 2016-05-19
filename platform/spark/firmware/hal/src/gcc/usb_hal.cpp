/**
 ******************************************************************************
 * @file    usb_device.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    27-Sept-2014
 * @brief   USB Virtual COM Port and HID device HAL
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_hal.h"
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#ifndef _WIN32
#include <sys/poll.h>
#include <termios.h>
#else
#include <windows.h>
#include <conio.h>
#endif

uint32_t last_baudRate;
#ifdef WIN32
HANDLE input_handle;
bool is_pipe;
fd_set rfds;
#endif

/*******************************************************************************
 * Function Name  : USB_USART_Init
 * Description    : Start USB-USART protocol.
 * Input          : baudRate.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Init(uint32_t baudRate)
{
    last_baudRate = baudRate;

#ifdef WIN32
 DWORD dw;
  input_handle = GetStdHandle(STD_INPUT_HANDLE);
  is_pipe = !GetConsoleMode(input_handle, &dw);
  /* Watch stdin (fd 0) to see when it has input. */
  FD_ZERO(&rfds);
  FD_SET(0, &rfds);
#endif
}

/*******************************************************************************
 * Function Name  : USB_USART_Available_Data.
 * Description    : Return the length of available data received from USB.
 * Input          : None.
 * Return         : Non zero when data is available.
 *******************************************************************************/
uint8_t USB_USART_Available_Data(void)
{
#ifdef WIN32
    struct timeval timeout;
    timeout.tv_sec = 0; // do not wait for input
    timeout.tv_usec = 0;
    int ret = select(1, &rfds, NULL, NULL, &timeout);
#else
  fd_set in;
  FD_ZERO(&in);
  FD_SET(STDIN_FILENO, &in);
  struct timeval tv = {0};

  int retval = select(1, &in, NULL, NULL, &tv);
  if (retval <= 0)
	  return 0;
  return retval;
#endif
/*// for some reason it stops working when we return -1
 * if(ret <= 0){
         return 0;
    }
    else{
    }
*/
    return ret;
}

int32_t last = -1;

/*******************************************************************************
 * Function Name  : USB_USART_Receive_Data.
 * Description    : Return data sent by USB Host.
 * Input          : None
 * Return         : Data.
 *******************************************************************************/
int32_t USB_USART_Receive_Data(uint8_t peek)
{
    if (last<0 && USB_USART_Available_Data()) {
        uint8_t data = 0;
        if (read(0, &data, 1))
            last = data;
    }
    int32_t result = last;
    if (!peek)
        last = -1;      // consume the data
    return result;
}

/*******************************************************************************
 * Function Name  : USB_USART_Available_Data_For_Write.
 * Description    : Return the length of available space in TX buffer
 * Input          : None.
 * Return         : Length.
 *******************************************************************************/
int32_t USB_USART_Available_Data_For_Write(void)
{
  return 1;
}

/*******************************************************************************
 * Function Name  : USB_USART_Send_Data.
 * Description    : Send Data from USB_USART to USB Host.
 * Input          : Data.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Send_Data(uint8_t Data)
{
    std::cout.write((const char*)&Data, 1);
}

/*******************************************************************************
 * Function Name  : USB_USART_Flush_Data.
 * Description    : Flushes TX buffer
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void USB_USART_Flush_Data(void)
{
}

#ifdef USB_HID_ENABLE
/*******************************************************************************
 * Function Name : USB_HID_Send_Report.
 * Description   : Send HID Report Info to Host.
 * Input         : pHIDReport and reportSize.
 * Output        : None.
 * Return value  : None.
 *******************************************************************************/
void USB_HID_Send_Report(void *pHIDReport, size_t reportSize)
{
}
#endif



unsigned int USB_USART_Baud_Rate(void)
{
    return last_baudRate;
}

void USB_USART_LineCoding_BitRate_Handler(void (*handler)(uint32_t bitRate))
{
}

int32_t USB_USART_Flush_Output(unsigned timeout, void* reserved)
{
    return 0;
}