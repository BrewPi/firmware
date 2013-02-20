/*
 * Time.h
 *
 * Created: 20/02/2013 19:57:48
 *  Author: mat
 */ 


#ifndef TIME_H_
#define TIME_H_

class Time
{
	public:
	virtual long millis()=0;
	virtual long micros()=0;
};

class ArduinoTime:public Time
{
	public:
	virtual long millis() { return ::millis(); }
	virtual long micros() { return ::micros(); }
}	
	};


#endif /* TIME_H_ */