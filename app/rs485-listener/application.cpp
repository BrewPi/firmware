#include "application.h"
#include <inttypes.h>
#include "EepromTypes.h"
#include "EepromAccessImpl.h"
#include "Platform.h"
#include "Board.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
SerialLogHandler traceLog(LOG_LEVEL_TRACE);

EepromAccess eepromAccess;

HAL_USB_USART_Config acquireSerialBuffer()
{
  HAL_USB_USART_Config conf = {0};

  // The usable buffer size will be 128
  static uint8_t serial_rx_buffer[257];
  static uint8_t serial_tx_buffer[257];

  conf.rx_buffer = serial_rx_buffer;
  conf.tx_buffer = serial_tx_buffer;
  conf.rx_buffer_size = 257;
  conf.tx_buffer_size = 257;

  return conf;
}

HAL_USB_USART_Config acquireUSBSerial1Buffer()
{
  HAL_USB_USART_Config conf = {0};

  // The usable buffer size will be 128
  static uint8_t serial_rx_buffer[257];
  static uint8_t serial_tx_buffer[257];

  conf.rx_buffer = serial_rx_buffer;
  conf.tx_buffer = serial_tx_buffer;
  conf.rx_buffer_size = 257;
  conf.tx_buffer_size = 257;

  return conf;
}

void setup() {
	boardInit();
	platform_init();
    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);

    acquireSerialBuffer();
    Serial.begin(230400);
    Serial1.begin(230400, SERIAL_8N1);

    // take control of the LED
	RGB.control(true);
	RGB.color(0,255,0);
}

// return time that has passed since timeStamp, take overflow into account
system_tick_t timeSince(system_tick_t previousTime) {
    system_tick_t currentTime = millis();
    if(currentTime>=previousTime){
        return currentTime - previousTime;
    }
    else{
        // overflow has occurred
        return (currentTime + 1440000) - (previousTime +1440000); // add a day to both for calculation
    }
}


void loop() {
	static system_tick_t lastReceive = millis();

	if(Serial.available()){
		Serial.read(); // discard
	}

    if(Serial1.available() > 0) {
    	uint8_t b = Serial1.read();
    	if(Serial.isConnected()){
    		Serial.write(b);
    	}
    	lastReceive = millis();
    }
    // red, green, blue, 0-255.
    // the following sets the RGB LED to white:

    if(timeSince(lastReceive) < 100){
    	RGB.color(0,255,255);
    }
    else{
    	RGB.color(0,255,0);
    }
    // Particle.process();
}
