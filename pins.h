/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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


#include <Arduino.h>

#ifndef PINS_H_
#define PINS_H_

#define beerSensorPin    A5 // OneWire 1
#define fridgeSensorPin  A4 // OneWire 2

#define coolingPin	6
#define heatingPin	5
#define doorPin		4
#define alarmPin	3
#define LCD_LATCH_PIN 10

// If you change the interrupt pins, you will also have to review the interrupt vectors of the rotary encoder
#define ROTARY_SWITCH 7 // INT6
#define ROTARY_A 8 // PCINT4
#define ROTARY_B 9 // PCINT5

// you can use the internal pull-up resistors instead of external ones for the doorPin and the rotary encoder pins
#define USE_INTERNAL_PULL_UP_RESISTORS false

#endif /* PINS_H_ */