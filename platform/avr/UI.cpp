#include "Brewpi.h"
#include "UI.h"
#include "RotaryEncoder.h"
#include "Display.h"
#include "Buzzer.h"
#include "Menu.h"
#include "Actuator.h"

DisplayType realDisplay;
DisplayType DISPLAY_REF display = realDisplay;


uint8_t UI::init() {
#if BREWPI_BUZZER
	buzzer.init();
	buzzer.beep(2, 500);
#endif
	display.init();
	rotaryEncoder.init();
	return 0;
}

uint32_t UI::showStartupPage()
{
    return 0;
}
        
/**
 * Show the main controller page. 
 */
void UI::showControllerPage()
{
	display.printStationaryText();
	display.printState();
}

extern ValueActuator alarm;
void UI::ticks() {
#if BREWPI_BUZZER
	buzzer.setActive(alarm.isActive() && !buzzer.isActive());
#endif

#if BREWPI_MENU
	if(rotaryEncoder.pushed()){
		rotaryEncoder.resetPushed();
		menu.pickSettingToChange();
	}
#endif
    
}

void UI::update() {

    // update the lcd for the chamber being displayed
    display.printState();
    display.printAllTemperatures();
    display.printMode();
    display.updateBacklight();		

}

bool UI::inStartup() { return false; }