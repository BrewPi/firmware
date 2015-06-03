#pragma once

#include "Values.h"
//#include "LcdDriver.h"
#include "DataStreamEeprom.h"
#include "TemperatureFormats.h"

class DisplayValue : public Value
{
	//LcdDriver driver;
	uint8_t buffer[4][20];
	
public:
	DisplayValue()
	{
//		driver.init();
//		driver.begin(20, 4);
//		driver.clear();
	}

	void writeMaskedFrom(DataIn& in, DataIn& mask) {
		readPlatformEndianMaskedBytes(buffer, 80, in, mask);		
	}
	
	void readTo(DataOut& out) {
		out.writeBuffer(buffer, 80);
	}
	
	uint8_t streamSize() {
		return 80;
	}
	
	static Object* create(ObjectDefinition& def) {
		return new_object(DisplayValue);
	}
};

uint8_t format_temp(uint8_t* data, char* _buf, uint8_t idx) {	
	// todo - make data in little endian format so it can be directly cast to a int on arduino?
	int temp = fixedToTenths((fixed7_9(data[0])<<8) | data[1]);
	bool neg = temp<0;
	temp = abs(temp);	
	
	char* buf = _buf + 6;		// 5 + 1	
	do {
		*--buf = (temp % 10)+'0';
		temp /= 10;
	} while (temp);
	while (buf>buf)
		*--buf = ' ';
	if (neg)
		*_buf = '-';
	_buf[4] = _buf[3];
	_buf[3] = '.';
	return idx+5;
}


class DisplayTemplate : public RehydratedAwareObject
{
	eptr_t address;
	char buf[80];		// if this becomes multi-instance make this a static pointer to dynamically allocated memory
	
	uint8_t formatValue(Value* value, uint8_t format, char* buf, uint8_t idx) {
		// todo - maintain an array of formatters, indexed by idx
		uint8_t read[4];		// assume maximum size of 4 bytes
		BufferDataOut out(read, 4);
		value->readTo(out);
		switch ((format & 0xF0)>>4) {
			case 0: // 2 byte fixed7_9 value, with offset. written to 5 spaces, right aligned.
				return format_temp(read, buf, idx);				
		}		
	}
	
public:
	void rehydrated(eptr_t address) {
		this->address = address;
	}
			
	void update() {		
		EepromDataIn in;
		// make DataIn from eeprom region		
		in.reset(eeprom_offset(), streamSize());
		
		// read target object (from id_chain in eeprom)
		Value* target = (Value*)lookupUserObject(in);
		Value* source;
		for (uint8_t i=0; i<80; ) {
			char c = in.next();
			switch (c) {
				case 0: i=80; break;
				case 10: 
					for (int x=i%20; x<20; x++) {	// clear to end of line
						buf[i++] = ' ';
					}
					break;
				case 27: 
					c = in.next();					// formatting
					source = (Value*)lookupUserObject(in);	// object data to format
					i = formatValue(source, c, buf, i);
					break;
				default:
					buf[i++] = c;
			}
		}
		
		// read until buffer full or newline		
		//		if 0x1B (escape), read next byte for formatting
		//		then fetch target Object* (read id_chain)		
		
		// TODO - ideally this would work with multiple beers, but not enough space, so
		// some kind of alternation is needed. This makes the template more complex.
		// thinking probably simplest to drive the display from the connector.
	}
	
	/**
	 * params:
	 *   id to write to
	 *   template: text given line by line with each line terminated by a null byte.
	 *		within each line the escape code (27) is used to start a placeholder.
	 *			the first byte of the placeholder is FFFFWWWW 
	 *				where WWWW encodes the width of the field (0-15)
	 *				and FFFF is the formatting type of the field.
	 *			subsequent bytes are the id_chain of the object to render
	 */
	static Object* create(ObjectDefinition& def) {
		return new_object(DisplayTemplate());
	}
};