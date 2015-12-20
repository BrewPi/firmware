#pragma once

#include "Values.h"
#include "StreamUtil.h"
#include "TemperatureFormats.h"

enum ControllerStateEnum
{
	idle = 0,
	heating = 1,
	cooling = 2	
};

/**
 * Here the references to the actuators and values are fixed as part of the object config.
 * This doesn't seem flexible enough.
 */
class BangBangController : public Value
{
	
public:
	uint8_t state;
	uint8_t hysteresis;
	Value* sensor;
	Value* setpoint;
	Actuator* heater;
	Actuator* cooler;
	
	BangBangController(uint8_t _hysteresis, Value* _sensor, Value* _setpoint, Actuator* _heater, Actuator* _cooler) 
	: state(idle), hysteresis(_hysteresis), sensor(_sensor), setpoint(_setpoint), heater(_heater), cooler(_cooler) {}
	
	/**
	 * Output format:
	 * output state: 0
	 */
	void readTo(DataOut& out) {
		out.write(state);
	}	
		
	uint8_t streamSize() { return 1; }
		
	void update() {
		int16_t offset = read2BytesFrom(sensor) - read2BytesFrom(setpoint);		
		int16_t h = hysteresis;
		switch (state)
		{
			case idle: 
				state = (offset<-h) ? heating : (offset>h) ? cooling : idle;
				break;
			case heating:
				if (offset>h)
					state = idle;
				break;
			case cooling:
				if (offset<-h)
					state = idle;
		}
		updateActuators();
	}
	
	void updateActuators()
	{
		heater->setActive(state==heating);
		cooler->setActive(state==cooling);		
	}
		
	/**
	 * Configuration:
	 * hysteresis	(1 byte) - temperature in 1/16 of a degree. Up to 16C.
	 * id_chain - temp sensor id - read provides 2 bytes of temp data
	 * id_chain - setpoint object - provides 2 bytes for setpoint
	 * id_chain - cooler - cooler actuator
	 * id_chain - heater - heater actuator
	 * ? alarm temp (hi/low) - could be a separate object reading from the temp sensor
	 */
	static Object* create(ObjectDefinition& def) {
		BangBangController* result = new_object(BangBangController(
			def.in->next(), 
			(Value*)lookupUserObject(*def.in),
			(Value*)lookupUserObject(*def.in), 
			(Actuator*)lookupUserObject(*def.in), 
			(Actuator*)lookupUserObject(*def.in)));
		return result;
	}
};

// persistent config
class ControllerConfig
{
	bool enabled;
	fixed4_4 hysteresis;		// 
	// etc. etc.. imagine lots of values here
	// uint8_t minCoolTime	
};

class ControllerConfigEepromValue : public EepromValue
{
	
public:


	uint8_t enabled() {
		return eepromAccess.readByte(eeprom_offset());
	}

	/**
	 * Here we directly access the config block in eeprom.
	 * We could equally have copied this to a ControllerConfig data member, but that takes more RAM.
	 */
	uint8_t hysteresis() {
		return eepromAccess.readByte(eeprom_offset()+1);
	}
	
};

struct ControllerState
{
	ControllerStateEnum state;
		
};

// non-persistent state
class ControllerStateValue : public ExternalValue
{
	ControllerState value;
	
public:	
	ControllerStateValue() 
	: ExternalValue(&value, sizeof(value)) {		
	}

	// more advanced controllers will have more here, e.g. p, i, d, tempDiff values	
	friend class BangBangController2;
};

struct ControllerAgents {
	Actuator* heater;		// slot 0
	Actuator* cooler;		// slot 1 
	Value* setpoint;		// slot 2
	Value* current;			// slot 3
};

/**
 * Another take on a simple controller implemented as a container. 
 * The caller when instantiating this should first instantiate with enabled flag in the controller
 * state set to false. Then instantiate the actuators and values to appropriate slots in container 0.
 * Finally, set the enabled byte to 1.  This will ensure that the controller only operates while it is
 * correctly configured.
 *
 * When reinstantiating from eeprom, all objects are instantiated before update is called so this is
 * always safe. 
 */
class BangBangController2 : public Container
{
	// fixed but open container for externally instantiated stuff Actuators, Values
	// fixed pre-instantiated container for values
	ControllerAgents agents;
	FixedContainer agentsContainer = FixedContainer(sizeof(agents)/sizeof(Object*), (Object**)&agents);	
	ControllerConfigEepromValue config;
	ControllerStateValue controllerState;
	
public:
	void rehydrated(eptr_t address) {
		config.rehydrated(address);
	}

	Object* item(container_id id) {
		switch (id) {
			case 0: return &agentsContainer;
			case 1: return &config;
			case 2: return &controllerState;
		}
		return NULL;
	}	
	
	void update() {			
		if (config.enabled()) {
			int16_t offset = read2BytesFrom(agents.current) - read2BytesFrom(agents.setpoint);
			int16_t h = config.hysteresis();
			ControllerStateEnum state = controllerState.value.state;
			switch (state)
			{
				case idle:
					state = (offset<-h) ? heating : (offset>h) ? cooling : idle;
					break;
				case heating:
					if (offset>h)
						state = idle;
					break;
				case cooling:
					if (offset<-h)
						state = idle;
			}
			controllerState.value.state = state;
			agents.heater->setActive(state==heating);
			agents.cooler->setActive(state==cooling);
		}
	}

	
	/**
	 * Definition block is the ControllerConfig block.
	 */
	static Object* create(ObjectDefinition& defn) {
		return new BangBangController2();
	}
};
