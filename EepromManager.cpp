/*
 * EepromManager.cpp
 *
 * Created: 23/04/2013 03:15:44
 *  Author: mat
 */ 

#include "EepromManager.h"
#include "TempControl.h"

EepromManager eepromManager;
EepromAccess eepromAccess;

void EepromManager::applySettings(eptr_t pv)
{
	//uint8_t blockSize = eepromAccess.readByte(pv);
	pv++;		// consume block size
	tempControl.loadConstants(pv);
	
}

void EepromManager::storeTempConstantsAndSettings()
{
	// find offset for const ant settings
	eptr_t pv = eptr_t(1/*version byte*/+1 /* chamber block len*/);
	pv += tempControl.storeConstants(pv);
	pv += tempControl.storeSettings(pv);
}

void EepromManager::storeTempSettings()
{
	eptr_t pv = eptr_t(1/*version byte*/+1 /* chamber block len*/);
	pv += tempControl.storeConstants(NULL);
	pv += tempControl.storeSettings(pv);
	
}


