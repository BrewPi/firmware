#include "Brewpi.h"
#include "UI.h"
#include "Buzzer.h"

void UI::init() {
#if BREWPI_BUZZER
	buzzer.init();
	buzzer.beep(2, 100);
#endif
}

void UI::update() {
#if BREWPI_BUZZER
	buzzer.setActive(alarm.isActive() && !buzzer.isActive());
#endif
}
