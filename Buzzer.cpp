/*
 * Buzzer.cpp
 *
 * Created: 7-2-2013 0:22:37
 *  Author: Elco
 */ 

#include "FastDigitalPin.h"
#include "Ticks.h"
#include "pins.h"
#include "Buzzer.h"
#include <util/delay.h>


#if (alarmPin != 3)
	#error "Check PWM settings when you want to use a different pin for the alarm"
#endif

#if defined(USBCON)
// Arduino Leonardo, the buzzer is on OC0B, but Timer0 is already in use for micros() and millis()
// Do not change anything about the timer, just use the output mode
#define BEEP_ON() bitSet(TCCR0A,COM0B1);
#define BEEP_OFF() bitClear(TCCR0A,COM0B1);

void Buzzer::init(void){
	// set up square wave PWM for buzzer
	fastPinMode(alarmPin,OUTPUT);
}

#else
// Arduino Uno, the buzzer is on OC2B
#define BEEP_ON() bitSet(TCCR2A,COM2B1);
#define BEEP_OFF() bitClear(TCCR2A,COM2B1);
	
void Buzzer::init(void){
	// set up square wave PWM for buzzer
#ifndef __OPTIMIZE__
	pinMode(alarmPin, OUTPUT);
#else		
	pinMode(alarmPin,OUTPUT);
#endif	
	// Arduino UNO, buzzer is on OC2B
	TCCR2A = (1<<COM2B1) | (1<<WGM20);
	TCCR2B = (1<<WGM22) | (1<<CS21) | (1<<CS20); // prescaler = 32
	OCR2A = 125; // timer top. This value adjusts the frequency.
	OCR2B = 62;
}

#endif


void Buzzer::beep(uint8_t numBeeps, uint16_t duration){
	for(uint8_t beepCount = 0; beepCount<numBeeps; beepCount++){
		BEEP_ON();
		wait.millis(duration);
		BEEP_OFF();
		if(beepCount < numBeeps - 1){
			wait.millis(duration); // not the last beep
		}		
	}		
}	
	
Buzzer buzzer;