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

void setup() {
	boardInit();
	platform_init();
    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);

    acquireSerialBuffer();
    Serial.begin(256000);
    Serial1.begin(256000, SERIAL_8N1);

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
	static uint8_t buffer[512];
	static int buffer_write_idx = 0;
	static int buffer_read_idx = 0;

	int available = Serial1.available();
    if(available > 0){
    	if(buffer_write_idx + available < 511){
    		char * write_loc = (char*) &buffer[buffer_write_idx];
    		buffer_write_idx += Serial1.readBytes(write_loc, available);
    	}
    	lastReceive = millis();
    }

    if(Serial.isConnected()){
		while(Serial.available()){
			Serial.read(); // discard
		}
		while(true){
			int writeAvailable = Serial.availableForWrite();
			int bufferAvailable = buffer_write_idx - buffer_read_idx;
			int writeBytes = writeAvailable < bufferAvailable ? writeAvailable : bufferAvailable;
			if(writeBytes <=0){
				break;
			}
			buffer_read_idx += Serial.write(&buffer[buffer_read_idx], writeBytes);
		}
    }
    if(buffer_read_idx == buffer_write_idx){
    	buffer_read_idx = 0;
    	buffer_write_idx = 0;
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
