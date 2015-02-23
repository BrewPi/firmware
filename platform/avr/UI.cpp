#include "Brewpi.h"
#include "UI.h"
#include "RotaryEncoder.h"
#include "Buzzer.h"
#include "Menu.h"

uint8_t UI::init() {
#if BREWPI_BUZZER
	buzzer.init();
	buzzer.beep(2, 500);
#endif
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
    // a no-op - we only have the controller page
}


void UI::update() {
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
