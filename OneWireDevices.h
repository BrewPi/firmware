/*
 * OneWireDevices.h
 *
 * Created: 01/05/2013 13:26:49
 *  Author: mat
 */ 

#include "brewpi_avr.h"

void parseBytes(uint8_t* data, const char* s, uint8_t len);
void printBytes(uint8_t* data, uint8_t len, char* buf);
