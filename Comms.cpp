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

#include "Platform.h"
#include "Comms.h"
#include "Commands.h"
#include "Version.h"
#include "Ticks.h"
#include "Integration.h"

// Rename Serial to piStream, to abstract it for later platform independence

#if NICE_EMULATE
class MockSerial : public Stream
{
	public:
	void print(char c) {}
	void print(const char* c) {}
	void printNewLine() {}
	void println() {}
	int read() { return -1; }
	int available() { return -1; }
	void begin(unsigned long) {}
	size_t write(uint8_t w) { return 1; }
	int peek() { return -1; }
	void flush() { };
	operator bool() { return true; }
};

static MockSerial comms;

#elif !defined(ARDUINO) && !defined(SPARK)
StdIO comms;
#else
#define comms Serial
	#define NICE_COMMS_USE_FLUSH 0
// for serial, flush waits until the output has been flushed. The flush is there just to ensure the output is not
// buffered, which it never is with serial.
#endif

#ifndef NICE_COMMS_USE_FLUSH
#define NICE_COMMS_USE_FLUSH 1
#endif

void Comms::init() {
    comms.begin(57600);
}

class CommsIn : public DataIn
{
	bool hasNext() { return comms; }			// hasNext true if stream is still open.
	uint8_t next() { return comms.read(); }
	uint8_t peek() { return comms.peek(); }
};

class CommsOut : public DataOut
{
public:
	bool write(uint8_t data) { comms.write(data); return true; }
	void flush() {
	#if NICE_COMMS_USE_FLUSH		// only flush for those stream types that require it
		comms.flush();
	#endif
	}
};

// low-level binary in/out streams
CommsIn commsIn;
CommsOut commsOut;

/*
 * Filters out non-significant text - comment markers, whitespace, unrecognized characters.
 * The stream automatically closes on newline.
 */
class TextIn : public DataIn {
	DataIn*	_in;
	uint8_t data;
	bool hasData;
	int8_t commentLevel;	// -1 indicates end of stream

	void fetchNextData(bool optional);

public:
	TextIn(DataIn& in)
	: _in(&in), data(0), hasData(0), commentLevel(0) {}

	bool hasNext()
	{
		fetchNextData(true);
		return hasData;
	}

	uint8_t next()
	{
		fetchNextData(false);
		hasData = false;
		return data;
	}

	uint8_t peek()
	{
		fetchNextData(true);
		return data;
	}
};

/**
 * Fetches the next significant data byte from the stream.
 * Sets hasData and data.
 */
void TextIn::fetchNextData(bool optional) {

	while (commentLevel>=0 && !hasData && _in->hasNext()) {
		data = 0xFF;
		uint8_t d = _in->next();
		if (d=='[') {
                    commentLevel++;
                }
		else if (d==']') {
                    commentLevel--;
                }
		else if (d=='\n' || d=='\r') {
                    commentLevel = -1; data = 0;
                }
		else if (!commentLevel && isHexadecimalDigit(d)) {
			hasData = true;
			data = d;
		}
	}
}

/**
 * Converts a hex digit to the corresponding binary value.
 */
uint8_t h2d(unsigned char hex)
{
	if (hex > '9')
		hex -= 7; // 'A' is 0x41, 'a' is 0x61. -7 =  0x3A, 0x5A
	return (hex & 0xf);
}

unsigned char d2h(uint8_t bin)
{
	return bin+(bin>9 ? 'A'-10 : '0');
}

/*
 * Converts pairs of hex digit characters into the corresponding binary value.
 */
class HexTextToBinaryIn : public DataIn
{
	DataIn* _text;	// store as pointer to avoid non-POD warnings
	uint8_t char1;	// Text character for upper nibble
	uint8_t char2;	// Text character for lower nibble

	void fetchNextByte();

public:
	HexTextToBinaryIn(DataIn& text) : _text(&text), char1(0), char2(0) {}

	bool hasNext() {
		fetchNextByte();
		return char2;
	}

	uint8_t peek() {
		fetchNextByte();
		return (h2d(char1)<<4) | h2d(char2);
	}

	uint8_t next()  {
		uint8_t r = peek();
		char1 = 0; char2 = 0;
		return r;
	}
};

/**
 * Fetches the next byte from the stream.
 */
void HexTextToBinaryIn::fetchNextByte()
{
	if (char1)		// already have data
		return;

	DataIn& _text = *this->_text;
	if (!_text.hasNext())
		return;

	if (!char1) {
		char1 = _text.next();
	}

	if (!_text.hasNext())
		return;

	if (!char2) {
		char2 = _text.next();
	}
}

class BinaryToHexTextOut : public DataOut {

	DataOut* _out;

public:

	BinaryToHexTextOut(DataOut& out) : _out(&out) {}

	/**
	 * Annotations are written as is to the stream, surrouned by annotation marks.
	 */
	void writeAnnotation(const char* data) {
		_out->write('[');
		_out->writeBuffer(data, strlen(data));
		_out->write(']');
	}

	/**
	 * Data is written as hex-encoded
	 */
	bool write(uint8_t data) {
		_out->write(d2h((data&0xF0)>>4));
		_out->write(d2h((data&0xF)));
		_out->write(' ');
		return true;
	}

	/**
	 * Rather than closing the global stream, write a newline to signify the end of this command.
	 */
	void close() {
		_out->write('\n');
	}
};

BinaryToHexTextOut hexOut(commsOut);
DataOut& Comms::hexOut = ::hexOut;
bool prevConnected = false;

bool reset = false;
void Comms::resetOnCommandComplete() {
	reset = true;
}

void Comms::receive() {

	static uint16_t connections = 0;
	// ensure that the Uno prints the version string on startup
	// or that the leonardo prints it with each disconnect/connection made
	bool b = comms;
	if (b != prevConnected) {
		printVersion(commsOut);
                commsOut.flush();
		prevConnected = b;
		if (b)
			connections++;
	}
#if 0
	digitalWrite(13, prevConnected ? LOW : HIGH);

	// this is to keep the port busy so that disconnects are detected immediately
	static byte count = 0;
	if ((count = (count+1)&0x3F))
		comms.write(' ');
	else {
		comms.write('[');
		comms.print(connections);
		comms.write(']');
		comms.println();
	}
#endif


	if (!prevConnected || reset)	// reset received, don't process any more commands
		return;

	while (comms.available()>0) {                           // there is some data ready to be processed											// form this point on, the system will block waiting for a complete command or newline.
		TextIn textIn(commsIn);
		HexTextToBinaryIn hexIn(textIn);
		if (hexIn.hasNext()) {				// ignore blank newlines, annotations etc..
			handleCommand(hexIn, hexOut);
                        while (hexIn.hasNext())	{			// todo - log a message about unconsumed data?
                                hexIn.next();
                        }
                }
		hexOut.close();
		commsOut.flush();
	}
	if (reset) {
		handleReset(true);					// do the hard reset
	}
}

