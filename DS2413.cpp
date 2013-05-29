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

#include "Brewpi.h"
#include "OneWire.h"

#include "DS2413.h"

uint8_t DS2413::channelReadAll()
{
	byte result = accessRead();
	return result<0 ? 0xFF : ((result&0x8)>>2 | (result&2)>>1);
}
	
void DS2413::channelWriteAll(uint8_t values)
{
	accessWrite(values);
}

