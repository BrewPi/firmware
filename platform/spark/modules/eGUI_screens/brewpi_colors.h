/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
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

#ifndef BREWPI_COLORS_H
#define	BREWPI_COLORS_H

#ifdef	__cplusplus
extern "C" {
#endif


#define HEATING_COLOR D4D_COLOR_RGB(0xb4, 0x00, 0x00)
#define COOLING_COLOR D4D_COLOR_RGB(0x00, 0x4b, 0xb4)
#define IDLE_COLOR D4D_COLOR_RGB(0x55, 0x55, 0x55)
#define WAITING_TO_HEAT_COLOR D4D_COLOR_RGB(0xb4, 0x68, 0x68)
#define WAITING_TO_COOL_COLOR D4D_COLOR_RGB(0x68, 0x98, 0xb4)
#define STATE_OFF_COLOR D4D_COLOR_RGB(0,0,0)
#define WAITING_FOR_PEAK_DETECT_COLOR IDLE_COLOR
#define DOOR_OPEN_COLOR D4D_COLOR_RGB(64,64,0)
#define COOLING_MIN_TIME_COLOR COOLING_COLOR
#define HEATING_MIN_TIME_COLOR HEATING_COLOR 

#define NAME_BG_COLOR D4D_COLOR_RGB(64, 64, 64)
#define BEER_BG_COLOR D4D_COLOR_RGB(0xb3, 0x98, 0x00) // HSV 51,70,70
#define BEER_FG_COLOR D4D_COLOR_RGB(0xff, 0xf7, 0xcc) // HSV 51,20,100
#define FRIDGE_BG_COLOR D4D_COLOR_RGB(0x36, 0x93, 0xb3) // HSV 195,70,70
#define FRIDGE_FG_COLOR D4D_COLOR_RGB(0xcc, 0xf2, 0xff) // HSV 195,20,100
#define ROOM_BG_COLOR D4D_COLOR_RGB(0x73, 0x7c, 0x80) // HSV 195,10,50
#define ROOM_FG_COLOR D4D_COLOR_RGB(0xe6, 0xf9, 0xff) // HSV 195,10,100

#define REGULAR_TEXT_COLOR D4D_COLOR_RGB(255,255,255)
#define SECONDARY_TEXT_COLOR D4D_COLOR_RGB(160,160,160)
#define LOW_TEXT_COLOR D4D_COLOR_RGB(128,128,128)

#define INITIAL_BLOCK_COLOR D4D_COLOR_RGB(32,32,32)


#define MODE_OFF_COLOR D4D_COLOR_RGB(0,0,0)
#define MODE_TEST_COLOR D4D_COLOR_RGB(0,0,64)
#define MODE_BEER_COLOR BEER_BG_COLOR
#define MODE_FRIDGE_COLOR FRIDGE_BG_COLOR
#define MODE_PROFILE_COLOR BEER_BG_COLOR


#ifdef	__cplusplus
}
#endif

#endif	/* BREWPI_COLORS_H */

