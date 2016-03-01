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

#include "Static.h"
#include "DataStream.h"

class Commands;

/**
 * The primary communications interface.
 */
class Comms {

	cb_static DataOut& hexOut;
	cb_static bool prevConnected;
	cb_static bool reset;

public:

#if !CONTROLBOX_STATIC
private:
	Commands* commands_ptr;


public:
	Comms(DataOut& hexOut_) : hexOut(hexOut_), prevConnected(false), reset(false) {}

	void setCommands(Commands& commands)
	{
		commands_ptr = &commands;
	}
#endif

	inline void connectionStarted(DataOut& out);

	inline void handleCommand(DataIn& in, DataOut& out);

	cb_static void init();

	/*
	 * Read and process from the commms link.
	 */
	cb_static void receive();

	cb_static void resetOnCommandComplete();

	/**
	 * Output stream. Used to write data after command processing.
	 */
	cb_static DataOut& dataOut() { return hexOut; }
};

#if CONTROLBOX_STATIC
	/**
	 * The global instance. Allows the same calling code to be used for static and non-static methods.
	 */
	extern Comms comms;
#endif
