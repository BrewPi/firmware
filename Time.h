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
	virtual long millis();
	virtual long micros();
};
	
class ScaledTime : public Time
{
	private:
	float scale;
	Time& source;
	protected:
	long scaleTime(long l);
	
	public:
	ScaledTime(Time& source);
	
	virtual long millis();
	virtual long micros();
};
	


#endif /* TIME_H_ */