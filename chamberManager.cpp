/*
 * chamberManager.cpp
 *
 * Created: 15/03/2013 02:23:11
 *  Author: mat
 */ 

#include "TempControl.h"
#include "chamber.h"

Chamber::Chamber(TempSensor& _fridgeSensor, TempSensor& _beerSensor, Actuator& _cooler, Actuator& _heater, Actuator& _light, Sensor<boolean>& _door) : 
		TempControlState(_fridgeSensor, _beerSensor, _cooler, _heater, _light, _door) 
{
	
}


chamber_id ChamberManager::switchChamber(chamber_id id)
{
	if (id==current)
		return id;
	DEBUG_MSG(PSTR("switch from chamber %d to %d"), current, id);
	chamber_id prev = current;
	getChamber(current).retract();
	getChamber(id).apply();
	current = id;
	return prev;
}
	
void ChamberManager::initChamber(chamber_id id) {
	current = id;
	Chamber& chamber = getChamber(id);
	chamber.init();
	tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM
	tempControl.init();
	tempControl.updatePID();
	tempControl.updateState();
	getChamber(id).retract();
}




