/*
 * BacklightController.h
 *
 * Created: 07/03/2013 23:23:44
 *  Author: mat
 */ 


#ifndef BACKLIGHTCONTROLLER_H_
#define BACKLIGHTCONTROLLER_H_

#include "ActivityHandler.h"
#include "Display.h"
#include "Ticks.h"

/* The idle duration after which the LCD backlight will automatically switch off. */
#define BACKLIGHT_AUTO_OFF_PERIOD 600  // 10 minutes


class BacklightController : public ActivityHandler {
public:

	BacklightController() : active(true), lastActivity(0) { }

	void init() {
		rotaryEncoder.setActivityHandler(this);
	}

	void notifyActivity(ActivityType activityType) {
		if (activityType==ROTARY_ENCODER_PUSHED) {
			active = true;
			rotaryEncoder.resetPushed();	
			display.lcd.setBacklightState(true);
				// the result of the push was to wake up the display - consume the push so that
				// other code doesn't react to it.
		}			
	}

	void updateBacklight() {
		if (active) {
			//piLink.debugMessage(PSTR("backlight on"));
			active = false;						// reset flag for this time slot
			lastActivity = ticks.seconds();
		}
		else if ((ticks.seconds()-lastActivity) > BACKLIGHT_AUTO_OFF_PERIOD) {
			//piLink.debugMessage(PSTR("backlight off"));
			display.lcd.setBacklightState(false);
		}
	}		

private:	
	/*
	 * If something was active in the last time slice. It's not necessary to record the exact time of the event - resolution to a few seconds
	 * is good enough.
	 */
	volatile bool active;
	uint16_t lastActivity;	
	
};


#endif /* BACKLIGHTCONTROLLER_H_ */