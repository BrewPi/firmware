#include "Brewpi.h"
#include "UI.h"
#include "RotaryEncoder.h"
#include "Buzzer.h"
#include "Menu.h"

void UI::init() {
#if BREWPI_BUZZER
	buzzer.init();
	buzzer.beep(2, 500);
#endif
	rotaryEncoder.init();
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
