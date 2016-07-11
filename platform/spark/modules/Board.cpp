/*
 * Copyright 2015 BrewPi/Elco Jacobs/Matthew McGowan.
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

#include "Platform.h"
#include "Board.h"
#include "Ticks.h"

uint8_t getShieldVersion(){
	static uint8_t shield = 255;

	// only auto-detect once
	if(shield == 255){
		// V2 has a pull down resistor, V1 has a pull up resistor on the alarm pin
		// If the pin is low, it is V2
		pinMode(alarmPin, INPUT);
		wait.millis(1); // give time to change
		if(digitalRead(alarmPin)){
			shield = BREWPI_SHIELD_SPARK_V1;
		}
		else{
			shield = BREWPI_SHIELD_SPARK_V2;
		}
	}
	return shield;
}

bool shieldIsV2(){
	return getShieldVersion() == BREWPI_SHIELD_SPARK_V2;
}
