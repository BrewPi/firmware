/*
 * Simulator.cpp
 *
 * Created: 14/05/2013 06:41:16
 *  Author: mat
 */ 

#include "Simulator.h"

#include "display.h"
#include "piLink.h"

#if BREWPI_SIMULATE

Simulator simulator;

static fixed7_9 funFactor = 0;	// paused
static unsigned long lastUpdate = 0;
uint8_t printTempInterval = 5;

void setRunFactor(fixed7_9 factor)
{
	funFactor = factor>>9;		// for now whole values only
	lastUpdate = ::millis();
}

void updateSimulationTicks()
{
#if BREWPI_EMULATE
	// in the avr simulator (we call emulator to try to distinguish), ticks take forever. 1 second takes many minutes if 
	// emulating waiting for millis() to increment.
	ticks.incMillis(1000);
#else
	if (funFactor) {
		unsigned long now = ::millis();
		int interval = 1000/funFactor;
		if (interval>0) {
			if ((now-lastUpdate)>=uint16_t(interval)) {
				lastUpdate += interval;
				ticks.incMillis(1000);
			}
		}
		else
		{
			lastUpdate = now;
			ticks.incMillis(1000);
		}
	}
#endif
}

void simulateLoop(void)
{
	static unsigned long lastUpdate = 0;
	
	// only needed if we want the arduino to be self running. Useful for manual testing, but not so much with an
	// external driver.
	updateSimulationTicks();
	
	if(ticks.millis() - lastUpdate >= (1000)) { //update settings every second
		lastUpdate = ticks.millis();
		
		tempControl.updateTemperatures();
		tempControl.detectPeaks();
		tempControl.updatePID();
		tempControl.updateState();
		tempControl.updateOutputs();

		#if !BREWPI_EMULATE			// simulation on actual hardware
		static byte updateCount = 0;
		if (printTempInterval && (++updateCount%printTempInterval)==0) {
			piLink.printTemperatures();
			updateCount = 0;
		}
		static unsigned long lastDisplayUpdate = 0;  // update the display every second
		if ((::millis()-lastDisplayUpdate)>=1000 && (lastDisplayUpdate+=1000))
		#endif
		{
			// update the lcd for the chamber being displayed
			display.printState();
			display.printAllTemperatures();
			display.printMode();
			display.updateBacklight();
		}
		
		simulator.step();
	}
	#if !BREWPI_EMULATE
	static unsigned long lastCheckSerial = 0;
	if ((::millis()-lastCheckSerial)>=1000 && (lastCheckSerial=::millis()>0))	// only listen if 1s passed since last time
	#endif
	//listen for incoming serial connections while waiting to update
	piLink.receive();

}

#include "TempSensorExternal.h"

const char SimulatorBeerTemp[] PROGMEM = "b";
const char SimulatorBeerConnected[] PROGMEM = "bc";
const char SimulatorBeerVolume[] PROGMEM = "bv";
const char SimulatorCoolPower[] PROGMEM = "c";
const char SimulatorDoorState[] PROGMEM = "d";
const char SimulatorFridgeTemp[] PROGMEM = "f";
const char SimulatorFridgeConnected[] PROGMEM = "fc";
const char SimulatorFridgeVolume[] PROGMEM = "fv";
const char SimulatorHeatPower[] PROGMEM = "h";
const char SimulatorPrintInterval[] PROGMEM = "i";
const char SimulatorNoise[] PROGMEM = "n";
const char SimulatorCoeffBeer[] PROGMEM = "kb";
const char SimulatorCoeffRoom[] PROGMEM = "ke";
const char SimulatorRoomTempMin[] PROGMEM = "rmi";
const char SimulatorRoomTempMax[] PROGMEM = "rmx";
const char SimulatorBeerDensity[] PROGMEM = "sg";
const char SimulatorTime[] PROGMEM = "t";

void setTicks(ExternalTicks& externalTicks, const char* val, int multiplier=1000) {		
	
	if (val==NULL || *val==0) {
		externalTicks.incMillis(1000);
	}
	else {
		if (*val=='=')
			externalTicks.setMillis(atol(val+1)*multiplier);
		else
			externalTicks.incMillis(atol(val+1)*multiplier);
	}
	
	logDeveloper("New ticks %lu"), externalTicks.millis();
}


/* How often the temperature is output, in simulated seconds.
 * 0 is never.
 * 1 is once per second.
 * 5 is once every 5 seconds etc..
 */
extern uint8_t printTempInterval;


void HandleSimulatorConfig(const char* key, const char* val, void* pv)
{
	// this set the system timer, but not the simulator counter
	if (strcmp_P(key, PSTR("s"))==0) {
		setTicks(ticks, val, 1000);
	}
	// these are all doubles - could replace this with a map of string keys to methods
	else if (strcmp_P(key, SimulatorRoomTempMin)==0) {
		simulator.setMinRoomTemp(atof(val));
	}
	else if (strcmp_P(key, SimulatorRoomTempMax)==0) {
		simulator.setMaxRoomTemp(atof(val));
	}
	else if (strcmp_P(key, SimulatorFridgeVolume)==0) {
		simulator.setFridgeVolume(atof(val));
	}
	else if (strcmp_P(key, SimulatorBeerVolume)==0) {
		simulator.setBeerVolume(atof(val));
	}
	else if (strcmp_P(key, SimulatorBeerDensity)==0) {
		simulator.setBeerDensity(atof(val));
	}
	else if (strcmp_P(key, SimulatorFridgeTemp)==0) {
		simulator.setFridgeTemp(atof(val));
	}
	else if (strcmp_P(key, SimulatorBeerTemp)==0) {
		simulator.setBeerTemp(atof(val));
	}
	else if (strcmp_P(key, SimulatorHeatPower)==0) {
		simulator.setHeatPower(atof(val));
	}
	else if (strcmp_P(key, SimulatorCoolPower)==0) {
		simulator.setCoolPower(atof(val));
	}
	else if (strcmp_P(key, SimulatorCoeffRoom)==0) {
		simulator.setRoomCoefficient(atof(val));
	}
	else if (strcmp_P(key, SimulatorCoeffBeer)==0) {
		simulator.setBeerCoefficient(atof(val));
	}
	else if (strcmp_P(key, SimulatorBeerConnected)==0) {
		simulator.setConnected(tempControl.beerSensor, strcmp(val, "0")!=0);
	}
	else if (strcmp_P(key, SimulatorFridgeConnected)==0) {
		simulator.setConnected(tempControl.fridgeSensor, strcmp(val, "0")!=0);
	}		
	else if (strcmp_P(key, SimulatorDoorState)==0) {		// 0 for closed, anything else for open
		simulator.setSwitch(tempControl.door, strcmp(val, "0")!=0);
	}
	else if (strcmp_P(key, PSTR("r"))==0) {
		setRunFactor(stringToFixedPoint(val));
	}
	else if (!strcmp_P(key, SimulatorPrintInterval)) {
		printTempInterval = atol(val);
	}
	else if (!strcmp_P(key, SimulatorNoise)) {
		simulator.setSensorNoise(atof(val));
	}
}

void PiLink::printDouble(double val)
{
	char buf[30];
	val *= 10000;
	long l = val;
	ltoa(l/10000, buf, 10);	// print the whole part
	piLink.print(buf);
	l = l % 10000;
	piLink.print_P(".%05d", l);
}

void PiLink::sendJsonPair(const char* name, double val)
{
	printJsonName(name);
	printDouble(val);
}

void PiLink::printSimulatorSettings()
{
	printResponse('U');	
	sendJsonPair(SimulatorRoomTempMin, simulator.getMinRoomTemp());
	sendJsonPair(SimulatorRoomTempMax, simulator.getMaxRoomTemp());
	sendJsonPair(SimulatorFridgeVolume, simulator.getFridgeVolume());
	sendJsonPair(SimulatorBeerVolume, simulator.getBeerVolume());
	sendJsonPair(SimulatorBeerDensity, simulator.getBeerDensity());
	sendJsonPair(SimulatorFridgeTemp, simulator.getFridgeTemp());
	sendJsonPair(SimulatorBeerTemp, simulator.getBeerTemp());
	sendJsonPair(SimulatorFridgeConnected, simulator.getConnected(tempControl.fridgeSensor) ? "1" : "0");
	sendJsonPair(SimulatorBeerConnected, simulator.getConnected(tempControl.beerSensor) ? "1" : "0");
	sendJsonPair(SimulatorHeatPower, (uint16_t)simulator.getHeatPower());
	sendJsonPair(SimulatorCoolPower, (uint16_t)simulator.getCoolPower());
	sendJsonPair(SimulatorCoeffRoom, simulator.getRoomCoefficient());
 	sendJsonPair(SimulatorCoeffBeer, simulator.getBeerCoefficient());
	sendJsonPair(SimulatorDoorState, simulator.doorState() ? "1" : "0");
	sendJsonPair(SimulatorDoorState, printTempInterval);
  	sendJsonPair(SimulatorNoise, simulator.getSensorNoise());
		
	sendJsonClose();		
}

#endif // brewpi simulate

