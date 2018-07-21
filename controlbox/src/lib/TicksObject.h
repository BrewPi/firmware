/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "Object.h"
#include "Ticks.h"
#include "Static.h"

namespace cbox {

// return time that has passed since timeStamp, take overflow into account
inline ticks_seconds_t timeSince(ticks_seconds_t currentTime, ticks_seconds_t previousTime){
	if(currentTime>=previousTime) {
		return currentTime - previousTime;
	}
	else{
		// overflow has occurred
		return (currentTime + 1440) - (previousTime +1440); // add a day to both for calculation
	}
}

/*
 * Allows time to run normally, or allows external code to manipulate time for testing and simulation.
 * The original ticks instance has been renamed baseticks, and this provides the current (real) time
 * for the platform, which is used by this class to compute the scaled time.
 *
 * The current time and scale are not persisted to eeprom.
 */
template<class T>
class ScaledTicksValue<T> : public WritableObject
{
	ticks_millis_t logicalStart;
	ticks_millis_t timerStart;
	uint16_t scale;
	T base;

public:
	ScaledTicksValue(T& _base) : logicalStart(0), timerStart(0), scale(1), base(base){}

	ticks_millis_t millis() {
		uint32_t now_offset = base.millis()-timerStart;
		return logicalStart + (now_offset*scale);
	}

	ticks_millis_t millis(ticks_millis_t& currentTime) {
		uint32_t now_offset = currentTime-timerStart;
		return logicalStart + (now_offset*scale);
	}

	ticks_seconds_t seconds() { return millis()/1000; }

	virtual Object::StreamToResult readTo(DataOut& out) override {
		ticks_millis_t timeVal = millis(base.millis());
		out.put(timeVal);
		out.put(scale);
		return Object::StreamToResult::success;
	}

	virtual void writeFrom(DataIn& in) override {
		ticks_millis_t timeVal = base.millis();
		ticks_millis_t newLogicalStart;
		uint16_t newScale;
		bool success = in.get(newLogicalStart);
		success &= in.get(newScale);

		if(!success){
		    return Object::StreamFromResult::stream_error;
		}

		logicalStart = newLogicalStart; // store what the scaled time was at the time of write
		timerStart = base.millis(); // store the base time at the time of write
		scale = newScale;
	}

	virtual uint8_t readStreamSize() override { return sizeof(logicalStart) + sizeof(scale); }

	// return time that has passed since timeStamp, take overflow into account
	ticks_seconds_t timeSinceSeconds(ticks_seconds_t previousTime) {
		ticks_seconds_t currentTime = seconds();
		return timeSince(currentTime, previousTime);
	}

	static std::shared_ptr<Object> create(DataIn & defn){
		return nullptr; // not creatable
	}

	virtual obj_type_t typeID() override {
		// use function overloading and templates to manage type IDs in a central place (TypeRegistry)
		return resolveTypeID(this);
	}
};

} // end namespace cbox

/**
 * Time is critical to so many components that this is provided as a system-level service outside of the cbox namespace
 * The SystemProfile maintains this instance and persists changes to eeprom.
 */
extern cbox::ScaledTicksValue<TICKS> ticks(baseticks);


namespace cbox {

/**
 * Remembers the time at the start of the current cycle.
 */
class CurrentCycleTicksValue : public CurrentTicksValue
{
	ticks_millis_t cycle_ticks;
public:

	virtual void update() override final {
		cycle_ticks = ticks.millis();
	}

	virtual void readTo(DataOut& out) override final {
		out.writeBuffer(&cycle_ticks, sizeof(cycle_ticks));
	}
};

} // end namespace cbox
