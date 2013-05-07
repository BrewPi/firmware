/*
 * FakeSerial.h
 *
 * Created: 19/03/2013 04:39:11
 *  Author: mat
 */ 

#pragma once

#include "FakeStream.h"

class FakeSerial : public FakeStream
{
public:
	void begin(uint16_t baud) { }
	
};