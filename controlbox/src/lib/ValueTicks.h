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

#include "Values.h"
#include "Ticks.h"
#include "StreamUtil.h"
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
class ScaledTicksValue : public WritableValue
{
	ticks_millis_t logicalStart;
	ticks_millis_t timerStart;
	uint16_t scale;

#if !CONTROLBOX_STATIC
	Ticks& baseticks;
#endif

public:
	ScaledTicksValue(cb_nonstatic_decl(Ticks& base)) : logicalStart(0), timerStart(0), scale(1) cb_nonstatic_decl(,baseticks(base)) {}

	ticks_millis_t millis() {
		uint32_t now_offset = baseticks.millis()-timerStart;
		return logicalStart + (now_offset*scale);
	}

	ticks_millis_t millis(ticks_millis_t& currentTime) {
		uint32_t now_offset = currentTime-timerStart;
		return logicalStart + (now_offset*scale);
	}

	ticks_seconds_t seconds() { return millis()/1000; }

	virtual void readTo(DataOut& out) override {
		ticks_millis_t time = baseticks.millis();
		time = millis(time);
		writePlatformEndianBytes(&time, sizeof(time), out);
		writePlatformEndianBytes(&scale, sizeof(scale), out);
	}

	virtual void writeFrom(DataIn& in) override {
		// write the start and scale
		ticks_millis_t time = baseticks.millis();
		logicalStart = millis(time);
		timerStart = time;
		readPlatformEndianBytes(&logicalStart, sizeof(logicalStart), in);
		readPlatformEndianBytes(&scale, sizeof(scale), in);
	}

	/**
	 * Stream written/read is
	 *   4-bytes	uint32	The number of milliseconds passed since poweron.
	 *   2-bytes	uint16	The time scaling. a 16-bit integer.
	 */
	virtual uint8_t readStreamSize() override { return 6; }

	// return time that has passed since timeStamp, take overflow into account
	ticks_seconds_t timeSinceSeconds(ticks_seconds_t previousTime) {
		ticks_seconds_t currentTime = seconds();
		return timeSince(currentTime, previousTime);
	}

	static Object* create(ObjectDefinition& /*defn*/) {
		return nullptr;
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
extern cbox::ScaledTicksValue ticks;


namespace cbox {

/**
 * Fetches the current millisecond count from the {@code Ticks} static instance.
 * TODO: now that ScaledTicksValue is part of the system container, we don't really need this?
 * It was mainly for testing, so can be replaced.
 */
class CurrentTicksValue : public Value
{

public:
	virtual void readTo(DataOut& out) override
	{
		ticks_millis_t millis = ticks.millis();
		writePlatformEndianBytes(&millis, 4, out);
	}

	virtual uint8_t readStreamSize() override
	{
		return sizeof(ticks_millis_t);
	}

	static Object* create(ObjectDefinition& /*def*/) {
		// no parameters required
		return new_object(CurrentTicksValue());
	}

	virtual obj_type_t typeID() override {
		// use function overloading and templates to manage type IDs in a central place (TypeRegistry)
		return resolveTypeID(this);
	}
};



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
