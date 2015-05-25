#pragma once

#include "Values.h"
#include "Actuator.h"

/**
 * 
 */
class LogicActuator : public Value
{
	/**
	 * The number of actuators that must return true for this one to return true.
	 * 0 - this functions as a NAND gate
	 * 1 - functions as a OR gate
	 * 2 - functions as a AND gate
	 */
	uint8_t count;
	
	Value* v1; Value* v2;

private:

	uint8_t active(Value* v) {
		uint8_t data = 0;
		BufferDataOut out(&data, 1);
		if (v!=NULL)
			v->readTo(out);
		return data;
	}
	
public:	
	LogicActuator(uint8_t _count, Value* _v1, Value* _v2)
	: count(_count), v1(_v1), v2(_v2) {}

	/**
	 * The value is 0 if not active, or non-zero if active.
	 */
	void writeFrom(DataIn& out) {		
		// doesn't support overrides - what to do here?
		// - change base Actautor interface to ActuatorOutput (supports query state not setting state)
		// - allow value to be written: 0 off, 1 on, -1 logic
	}
	
	void readTo(DataOut& out) {
		out.write(isActive());		
	}
	
	// todo - if there are other simple truth values ensure this class is refactored to avoid code-duplication
	uint8_t streamSize() { return 1; }
	
	bool isActive() {
		return active(v1) + active(v2) == count;
	}	
	
		
	/**
	 * data is a sequence of 
	 */
	static Object* create(ObjectDefinition& def)
	{
		LogicActuator* result = new LogicActuator(
			def.in->next(),
			(Value*)lookupUserObject(*def.in),
			(Value*)lookupUserObject(*def.in)
		);
		return result;
	}
};


#ifdef ARDUINO

class DigitalPinActuator : public Actuator
{
private:
	bool invert;
	uint8_t pin;
	bool active;
public:
	
	DigitalPinActuator(uint8_t pin, bool invert) {
		this->invert = invert;
		this->pin = pin;
		setActive(false);
		pinMode(pin, OUTPUT);
	}
	
	void readTo(DataOut& out) {
		out.write(isActive());
	}
	
	void writeMaskedFrom(DataIn& in, DataIn& mask) {
		setActive(in.next() & mask.next());
	}
	
	uint8_t streamSize() {
		return 1;
	}
	
	inline void setActive(bool active) {
		this->active = active;
		digitalWrite(pin, active^invert ? HIGH : LOW);
	}
	
	bool isActive() { return active; }
		
	/*
	 * Params: 1 - byte
	 *			IPPPPPPP
	 *			I = invert (0 not inverted, 1 inverted)
	 *			P = bits for pin number.
	 */
	static Object* create(ObjectDefinition& def) {
		uint8_t v = def.in->next();
		return new_object(DigitalPinActuator(v&0x7f, v&0x80));
	}
	
};



#endif