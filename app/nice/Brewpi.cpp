/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Brewpi.h"
#include "Ticks.h"
#include "Comms.h"
#include "Commands.h"
#include "Values.h"
#include "ValuesEeprom.h"
#include "ValuesProgmem.h"
#include "GenericContainer.h"
#include "ValueModels.h"
#include "ValueTicks.h"
#include "ValueActuator.h"
#include "SystemProfile.h"
#include "Profile.h"
#include "BangBangController.h"
#include "ValueDisplay.h"

#if defined(ARDUINO) || defined(SPARK)
#include "OneWireBus.h"
#include "OneWireTempSensor.h"
#endif

#if BREWPI_SIMULATE && 0                // disable simulator for time being
	#include "Simulator.h"
#endif

// global class objects static and defined in class cpp and h files

// instantiate and configure the sensors, actuators and controllers we want to use

EepromAccess eepromAccess;

void setup(void);
void loop (void);

Container* createRootContainer()
{
	DynamicContainer* d = new DynamicContainer();    
    return d;
}

#if 0
class BuildInfoValues : public FactoryContainer {
	private:	
	
	public:
	BuildInfoValues() {}
			
	container_id size() { return 1; }
	virtual Object* item(container_id id) { return new_object(ProgmemStringValue((PSTR(BUILD_NAME)))); }
};
#endif

bool logValuesFlag = false;

const uint8_t loadProfileDelay = 10;	// seconds

class GlobalSettings {
	uint8_t settings[10];
		
	Object* externalValueHandler(container_id id) {
		if (id==-1) return (Object*)10;	// size
		if (id>9) return NULL;
		return new ExternalValue(settings+id, 1);
	}
};

void setup()
{    	
	SystemProfile::initialize();
	
	Comms::init();
			
#if 0
	uint8_t start = ticks.seconds();
	while (ticks.timeSince(start)<loadProfileDelay) {
		Comms::receive();
	}
#endif			
	SystemProfile::activateDefaultProfile();
}

bool prepareCallback(Object* o, void* data, container_id* id, bool enter) {
	if (enter) {				
		uint32_t& waitUntil = *(uint32_t*)data;
		prepare_t millisToWait = o->prepare();
		if (millisToWait)
			waitUntil = ticks.millis()+millisToWait;
	}
	return false;	// continue enumerating
}

/**
 * Updates each object in the container hierarchy.
 */
bool updateCallback(Object* o, void* data, container_id* id, bool enter) {
	if (enter)
		o->update();
	return false;
}

/**
 * Logs all values in the system. 
 */
void logValues(container_id* ids)
{
	DataOut& out = Comms::dataOut();
	out.write(CMD_LOG_VALUES_AUTO);
	logValuesImpl(ids, out);
	out.close();
}

void process() {
	container_id ids[MAX_CONTAINER_DEPTH];
	
    prepare_t d = 0;
    Container* root = SystemProfile::rootContainer();
    if (root)
        d = root->prepare();
#if BREWPI_VIRTUAL			
    d = min(prepare_t(1000), d);	// just for testing to stop busy waiting
#endif
    uint32_t end = ticks.millis()+d;
    while (ticks.millis()<end) {
        Comms::receive();
#if BREWPI_VIRTUAL              // avoid busy waiting on a desktop PC since this hogs the cpu
        wait.millis(10);
#endif        
    }        
        
    Container* root2 = SystemProfile::rootContainer();	
        // root may have been changed by commands, so original prepare may not be valid
        // should watch out for newly created objects, since these will then also need preparing
	if (root==root2 && root) {
        root->update();
		
        // todo - should brewpi always log, or only log when requested?
        if (logValuesFlag)
        {
            logValuesFlag = false;
            logValues(ids);
        }
    }
}

/*
 * Lifecycle for components:
 *
 * - prepare: start of a new control loop and determine how long any asynchronous operations will take. 
 * - update: fetch data from the environment, read sensor values, compute settings etc..
 */
void brewpiLoop(void)
{		
	process();
	Comms::receive();
}

void loop() {       
	#if 0 && BREWPI_SIMULATE
	simulateLoop();
	#else
	brewpiLoop();
	#endif
}

/**
 * ARDUINO_OBJECT - used to declare object factories that are only suitable on 
 * an arduino-compatible device
 */
#if defined(ARDUINO) || defined(SPARK)
#define ARDUINO_OBJECT(x) x
#else
#define ARDUINO_OBJECT(x) nullFactory
#endif	


#if BREWPI_LCD && 0
#define DISPLAY_OBJECT(x) x
#else
#define DISPLAY_OBJECT(x) nullFactory
#endif

/**
 * Include objects that have not been tested.
 */
#define BREWPI_EXPERIMENTAL 1

#if BREWPI_EXPERIMENTAL
#define EXPERIMENTAL(x) x
#else
#define EXPERIMENTAL(x) nullFactory
#endif

ObjectFactory createObjectHandlers[] = {
	nullFactory,                                            // type 0
	ARDUINO_OBJECT(OneWireBus::create),						// type 1
	ARDUINO_OBJECT(OneWireTempSensor::create),       		// type 2
	CurrentTicksValue::create,								// type 3
	DynamicContainer::create,								// type 4
	EepromValue::create,									// type 5
	Profile::create,										// type 6
	EXPERIMENTAL(LogicActuator::create),					// type 7
	EXPERIMENTAL(BangBangController2::create),				// type 8
	PersistChangeValue::create,								// type 9
	DISPLAY_OBJECT(DisplayValue::create),					// type A
	DISPLAY_OBJECT(DisplayTemplate::create),				// type B
//	ARDUINO_OBJECT(DigitalPinActuator::create),				// type C
	IndirectValue::create,									// type D
	NULL
	
	// When defining a new object type, add the handler above the last NULL value (it's just there to make
	// editing the code easier).
	// The Object definition passed to the create handler contains the stream and the block length.
	// it's critical that the create code reads len bytes from the stream so that the data is 
	// spooled to eeprom to the persisted object definition.
};

/**
 * The application supplied object factory.
 * Fetches the object type from the stream and looks this up against an array of object factories.
 */
Object* createApplicationObject(ObjectDefinition& def, bool dryRun)
{
	uint8_t type = def.type;
	if (dryRun || type>=sizeof(createObjectHandlers)/sizeof(createObjectHandlers[0]))
		type = 0;		// null object creator. Ensures stream is properly consumed even for invalid type values.
	
	Object* result = createObjectHandlers[type](def);	
	return result;
}
