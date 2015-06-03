/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This Atmel Studio 6 project automatically includes all needed Arduino source files, you just have to point it to the right directories.
 * To compile this project on your computer, you will have to set an environment variable to find your local Arduino installation.
 * Set the variable ARDUINO_HOME to point to your local Arduino path, without trailing slash, e.g. 'D:\arduino-1.01'. Instructions on the wiki here:
 * http://wiki.brewpi.com/index.php/Setting_up_the_brewpi-avr_Project
 * 'ArduinoFunctions.cpp' includes all the source files from Arduino that are used. You might have to edit it if you are not using a Leonardo.
 * That is all that is needed! No hassle with makefiles and compiling libraries.
 */
#include "Brewpi.h"
#include "Platform.h"
#include "PiLinkHandlers.h"

// setup and loop are in brewpi_config so they can be reused across projects
extern void setup(void);
extern void loop (void);


void handleReset(bool exit) 
{ 
    if (!exit)
        return;
    // resetting using the watchdog timer (which is a full reset of all registers) 
    // might not be compatible with old Arduino bootloaders. jumping to 0 is safer.
    asm volatile ("  jmp 0");
}

void flashFirmware()
{
    // a no-op. This is not used on this platform.
}

__attribute__((OS_main)) int main(void);

int main(void)
{
	init();

	#if defined(USBCON)
		USBDevice.attach();
	#endif
	
	setup();
	
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
	return 0;
}

// catch bad interrupts here, uncomment while only when debugging
ISR(BADISR_vect){
	while (1);
}

