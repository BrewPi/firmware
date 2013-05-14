/*
 * ActuatorAutoOff.h
 *
 * Created: 14/05/2013 05:28:58
 *  Author: mat
 */ 

#pragma once

#include "Actuator.h"
#include "Ticks.h"

class AutoOffActuator : public Actuator {
	
public:
	AutoOffActuator(uint16_t timeout, Actuator* target) {
		this->timeout = timeout;
		this->target = target;
	}
	
	void setActive(bool active)
	{
		target->setActive(active);
		if (active)
			lastActiveTime = ticks.seconds();
	}
	
	void update() {
		if (ticks.timeSince(lastActiveTime)>=timeout)
			setActive(false);
	}

private:
	uint16_t lastActiveTime;
	uint16_t timeout;
	Actuator* target;
};