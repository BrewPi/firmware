/*
 * Buzzer.h
 *
 * Created: 7-2-2013 0:24:58
 *  Author: Elco
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

class Buzzer
{
	public:
Buzzer(){};
	~Buzzer(){};
	
	void init(void);
	void beep(uint8_t numBeeps, uint16_t duration);
	
	private:
};

extern Buzzer buzzer;



#endif /* BUZZER_H_ */