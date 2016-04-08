/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "DataStream.h"

/**
 * The primary communications interface.
 */
class Comms {

	static DataOut& hexOut;

public:
	static void init();

	/*
	 * Read and process from the commms link.
	 */
	static void receive();

	static void resetOnCommandComplete();

	/**
	 * Output stream. Used to write data after command processing.
	 */
	static DataOut& dataOut() { return hexOut; }
};

