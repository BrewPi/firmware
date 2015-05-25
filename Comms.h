#pragma once

#include "DataStream.h"

/*
 * Comms.h
 *
 * Created: 05/02/2014 22:17:46
 *  Author: mat
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

