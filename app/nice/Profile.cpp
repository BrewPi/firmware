/*
 * Profile.cpp
 *
 * Created: 22/03/2014 22:24:49
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "Profile.h"
#include "TemperatureFormats.h"
#include "ValueTicks.h"

ticks_millis_t ProfileConfig::lastTick;

typedef uint16_t fixed0_16;
typedef uint32_t fixed16_16;

uint16_t ProfileConfig::calculateSetpoint(uint8_t step, uint8_t maxStep, uint16_t current) 
{
	uint16_t duration = stepDuration(step);
	uint16_t setpoint = stepSetpoint(step);
	ProfileInterpolation interpolation = this->profileInterpolation();
	if (step==maxStep || duration==0 || interpolation==none)
	{		
		// setpoint is as is
	}
	else
	{
		// todo - duration granularity is one minute
		
		// fixed point 0.16
		fixed0_16 t2 = ((fixed16_16)current << 16) / duration;

		fixed7_9 sp1 = setpoint;
		fixed7_9 sp2 = stepSetpoint(step+1);
#if PROFILE_SMOOTHING		
		if (interpolation == smooth) {
			// smoothstep (x*x*(3-2*x))
			t2 = (fixed0_16)(((((fixed16_16)t2) * (fixed16_16)t2) >> 16) * ((3UL<<16) - ((fixed16_16) t2 << 1)) >> 16);
		} 
		else if (interpolation == smoother) {
			// smootherstep (x*x*x*(x*(x*6-15)+10))
			t2 = ((((t2 * ((fixed16_16)t2 * 6UL - (15UL<<16))) >> 16) + (10UL<<16)) * (((((fixed16_16)t2 * t2) >> 16) * t2) >> 16)) >> 16;
		}
#endif		
		fixed0_16 t1 = (1UL<<16)-t2;
		if (interpolation!=none)
			setpoint = ((fixed23_9)sp1 * t1 + (fixed23_9)sp2 * t2) >> 16;				
	}
	return setpoint;
}
	
profile_value_t ProfileConfig::updateSetpoint(profile_value_t setpoint)
{	
	ticks_millis_t currTick = ticks.millis();
	if (currTick-lastTick>60000) {
		lastTick = currTick;
		
		uint8_t step = currentStep();
		uint8_t maxStep = stepCount()-1;		
		uint16_t current = currentProfileTime();
		if (current >= stepDuration(step)) {	// end of current step
			if (step==maxStep || stepDuration(step)==0) {		// at least step
				setpoint = stepSetpoint(step);
			}
			else {
				setCurrentProfileTime(0);
				setCurrentStep(step+1);
				setpoint = calculateSetpoint(step, maxStep, current);
			}
		}
		else {
			setpoint = calculateSetpoint(step, maxStep, current);
		}
	}
	return setpoint;
}

#if 0 
// obsolete - for passing the value to another object
void ProfileConfig::setSetpoint(uint16_t setpoint) {	
	
	EepromDataIn id_chain;
	id_chain.reset(eeprom_offset()+4+(stepCount()<<1), MAX_CONTAINER_DEPTH);
	Object* obj = lookupUserObject(id_chain);
		
	if (isWritable(obj)) {
		Value* v = (Value*)obj;
		if (v->streamSize()==2) {
			uint8_t buf[2];
			buf[0] = setpoint >> 8;			// ensure in appropriate order (not architecture dependent)
			buf[1] = setpoint & 0xFF;
			BufferDataIn buffer(buf);
			DefaultMask mask;
			v->writeMaskedFrom(buffer, mask);
		}
	}
}
#endif