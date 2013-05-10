/*
 * Time.cpp
 *
 * Created: 21/02/2013 02:18:29
 *  Author: mat
 */ 

#include "brewpi_avr.h"
#include "Time.h"

long ArduinoTime::millis() 
{ 
	return ::millis(); 
}

long ArduinoTime::micros() 
{ 
	return ::micros(); 
}

ScaledTime::ScaledTime(Time& source)
: source(source)
{	
}	

long ScaledTime::millis()
{
	return scaleTime(source.millis());
}

long ScaledTime::micros()
{
	return scaleTime(source.micros());
}

long ScaledTime::scaleTime(long l)
{
	return long(float(l)*scale);
}	

