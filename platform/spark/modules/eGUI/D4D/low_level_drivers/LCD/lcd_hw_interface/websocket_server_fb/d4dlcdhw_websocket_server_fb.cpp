/**************************************************************************
*
* Copyright 2016 by Matthew McGowan.
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
*/

extern "C" {
#include "d4d.h"            // include of all public items (types, function etc) of D4D driver
#include "common_files/d4d_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "common_files/d4d_private.h"    // include the private header file that contains perprocessor macros as D4D_MK_STR
}
#include "application.h"
#include "WebSocketsServer.h"
#include <vector>

/******************************************************************************
* D4D LCD HW Driver setting  constants
*
*//*! @addtogroup doxd4d_lcdhwfb_const
* @{
*******************************************************************************/

/*! @brief  Identification string of driver - must be same as name D4DLCDHWFB_FUNCTIONS structure + "_ID"
 it is used for enable the code for compilation   */
#define d4dlcdhw_websocket_server_fb_ID 1

 /*! @} End of doxd4d_lcdhwfb_const                                           */

// compilation enable preprocessor condition
// the string d4dtch_websocket_server_fb_ID must be replaced by define created one line up
#if (D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_websocket_server_fb_ID)

  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4D configuration file
  #include "low_level_drivers/LCD/lcd_hw_interface/websocket_server_fb/d4dlcdhw_websocket_server_fb.h"
  /******************************************************************************
  * Macros
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes
  ******************************************************************************/

  static unsigned char D4DLCDHW_Init_WebsocketServerFb(void);
  static unsigned char D4DLCDHW_DeInit_WebsocketServerFb(void);
  static void D4DLCDHW_WriteData_WebsocketServerFb(unsigned long addr, D4D_COLOR value);
  static D4D_COLOR D4DLCDHW_ReadData_WebsocketServerFb(unsigned long addr);
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_WebsocketServerFb(void);
  static unsigned char D4DLCDHW_PinCtl_WebsocketServerFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState);
  static void D4DLCD_FlushBuffer_WebsocketServerFb(D4DLCD_FLUSH_MODE mode);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/

  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4D
  // so this name has to be used in main configuration header file of D4D driver to enable this driver
  extern "C" const D4DLCDHWFB_FUNCTIONS d4dlcdhw_websocket_server_fb __attribute__((used)) =
  {
    D4DLCDHW_Init_WebsocketServerFb,
    D4DLCDHW_WriteData_WebsocketServerFb,
    D4DLCDHW_ReadData_WebsocketServerFb,
	D4DLCDHW_GetFbDescriptor_WebsocketServerFb,
    D4DLCDHW_PinCtl_WebsocketServerFb,
    D4DLCD_FlushBuffer_WebsocketServerFb,
    D4DLCDHW_DeInit_WebsocketServerFb
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

struct __attribute__((packed)) Pixel
{
	uint32_t address_;
	uint32_t color_;

	Pixel() = default;
	~Pixel() = default;

	Pixel(uint32_t address, uint32_t color)
	: address_(address), color_(color)
	{
	}
};

extern "C" void websocket_touch(uint16_t x, uint16_t y);

class DisplayServer;
class DisplayBuffer
{
	static const uint32_t count = D4D_SCREEN_SIZE_LONGER_SIDE*D4D_SCREEN_SIZE_SHORTER_SIDE;
	D4D_COLOR pixels[count];
public:

	inline void set_pixel(uint32_t offset, D4D_COLOR color)
	{
		// addresses are given as offsets
		if (offset<count)
			pixels[offset] = color;
	}

	void push(DisplayServer& server);
};

static_assert(sizeof(D4D_COLOR)==2, "expected D4D_COLOR to be 16-bit");

class NoOpDisplayBuffer
{
	inline void set_pixel(uint32_t offset, D4D_COLOR color) {

	}

	void push(DisplayServer& server) {}
};

#if PLATFORM_ID==3
typedef DisplayBuffer DisplayBufferImpl;
#else
typedef NoOpDisplayBuffer DisplayBufferImpl;
#endif


class DisplayServer
{
	WebSocketsServer server;

	const static int pixel_count = D4D_SCREEN_SIZE_LONGER_SIDE;
	std::vector<Pixel> pixels;
	DisplayBufferImpl buffer;

public:
	DisplayServer(uint16_t port, const String& origin) : server(port, origin) {
		pixels.reserve(pixel_count);
	}

	bool start()
	{
		server.begin();
		server.onEvent(webSocketEvent);
		INFO("started DisplayServer");
		return true;
	}

	void loop()
	{
		server.loop();
	}

	bool stop()
	{
		return true;
	}

	void process_command(const uint8_t* data, size_t length)
	{
		if (length>=1) {
			int cmd = data[0];
			DEBUG("command %d, %d", cmd, length);
			switch (cmd) {
			case 1:
				if (length>=5) {
					uint16_t x = *((uint16_t*)(data+1));
					uint16_t y = *((uint16_t*)(data+3));
					websocket_touch(x, y);
					DEBUG("touch %d, %d", x, y);
				}
				break;
			}
		}
	}

	void handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
	{
		switch (type) {
		case WStype_CONNECTED:
			{
				buffer.push(*this);

				D4D_SCREEN* screen = D4D_GetActiveScreen();
				// invalidate the new screen (global complete redraw, not individual objects)
				// D4D_InvalidateScreen(D4D_GetActiveScreen(), D4D_TRUE);
				if (screen) {
					D4D_InvalidateScreen(screen, D4D_TRUE);
					D4D_RedrawScreen(screen);
				}
			}
			break;
		case WStype_BIN:
			process_command(payload, length);
			break;
		default:
			break;
		}
	}

	void flush()
	{
		uint32_t size = pixels.size();
		if (size) {
			Pixel* data = pixels.data();
			server.broadcastBIN((const uint8_t*)data, sizeof(Pixel)*size);
			pixels.clear();
		}
	}

	void add_pixel(uint32_t offset, D4D_COLOR color)
	{
		if (pixels.size()==pixels.capacity())
			flush();
		pixels.push_back(Pixel(offset, color));
		buffer.set_pixel(offset, color);
	}
};

void DisplayBuffer::push(DisplayServer& server)
{
	for (uint32_t i=0; i<count; i++)
		server.add_pixel(i, pixels[i]);
	server.flush();
}


DisplayServer server(0x1cd0, "*"); // 7376

extern "C" void loop_ws_display() {
	server.loop();
}

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	server.handleEvent(num, type, payload, length);
}


  /**************************************************************************/ /*!
  * @brief   The function is used for initialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should initilize all neccessary things to run template lcd hardware frame buffer interface driver.
  *******************************************************************************/
  static unsigned char D4DLCDHW_Init_WebsocketServerFb(void)
  {
	  return server.start();
  }


  /**************************************************************************/ /*!
  * @brief   The function is used for deinitialization of this low level driver
  * @return  result: 1 - Success; 0 - Failed
  * @note    This should deinitilize all neccessary things to run template lcd hardware frame buffer interface driver.
  *******************************************************************************/
  static unsigned char D4DLCDHW_DeInit_WebsocketServerFb(void)
  {
	  return server.stop();
  }

  /**************************************************************************/ /*!
  * @brief   The function send the one pixel variable into frame buffer
  * @param   addr - the absolute address to frame buffer
  * @param   value - the pixel value
  * @return  none
  * @note    This function writes one pixel on specified address to frame buffer
  *******************************************************************************/
  static void D4DLCDHW_WriteData_WebsocketServerFb(unsigned long addr, D4D_COLOR value)
  {
	  server.add_pixel(uint32_t(addr>>1), value);
  }

//  D4D_COLOR buffer[D4D_SCREEN_SIZE_LONGER_SIDE*D4D_SCREEN_SIZE_SHORTER_SIDE];

  /**************************************************************************/ /*!
  * @brief   The function reads the one pixel variable from frame buffer
  * @param   addr - the absolute address to frame buffer
  * @return  the pixel value
  * @note    This function reads one pixel from specified address in frame buffer
  *******************************************************************************/
  static D4D_COLOR D4DLCDHW_ReadData_WebsocketServerFb(unsigned long addr)
  {
	  return 0;
  }


  /**************************************************************************/ /*!
  * @brief   The function return the pointer on filled frame buffer descriptor
  * @return  pointer to frame buffer descriptor
  * @note    Just to handle pointer to frame buffer descriptor
  *******************************************************************************/
  static D4DLCD_FRAMEBUFF_DESC* D4DLCDHW_GetFbDescriptor_WebsocketServerFb(void)
  {
	  static D4DLCD_FRAMEBUFF_DESC desc;
	  desc.fb_start_addr = 0;
	  desc.lcd_x_max = D4D_SCREEN_SIZE_LONGER_SIDE;
	  desc.lcd_y_max = D4D_SCREEN_SIZE_SHORTER_SIDE;
	  desc.bpp_byte = sizeof(D4D_COLOR);
	  static_assert(sizeof(D4D_COLOR)==2, "expected 16-bits per pixel");
	  return &desc;
  }


  /**************************************************************************/ /*!
  * @brief   The function allows control GPIO pins for LCD conrol purposes
  * @param   pinId - the pin definition
  * @param   setState - the pin action/state definition
  * @return  for Get action retuns the pin value
  * @note    This function controls the additional control pins
  *******************************************************************************/
  static unsigned char D4DLCDHW_PinCtl_WebsocketServerFb(D4DLCDHW_PINS pinId, D4DHW_PIN_STATE setState)
  {
	  return 0;
  }

  /**************************************************************************/ /*!
  * @brief   For buffered low level interfaces is used to inform
  *            driver the complete object is drawed and pending pixels should be flushed
  * @param   mode - mode of Flush
  * @return  none
  * @note    This function just notify the driver that eGUI finish drawing element/object/screen to allow driver handle any kind of caching memmory.
  *******************************************************************************/
  static void D4DLCD_FlushBuffer_WebsocketServerFb(D4DLCD_FLUSH_MODE mode)
  {
	  server.flush();
	  if (mode == D4DLCD_FLSH_FORCE) {
		  server.loop();
	  }
  }

  /*! @} End of doxd4d_tch_func                                               */

#endif //(D4D_MK_STR(D4D_LLD_LCD_HW) == d4dlcdhw_template_fb_ID)
