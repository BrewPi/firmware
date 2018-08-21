/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of Controlbox
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

#include "Ticks.h"
#include "Object.h"
#include "DataStream.h"

namespace cbox {

template<class T>
class TicksObject : public Object
{
	T & ticks;

public:
	TicksObject(T & _ticks) : ticks(_ticks){}

	virtual CboxError streamTo(DataOut& out) const override final {
		ticks_seconds_t secondsSinceUtc = ticks.getNow();
		out.put(secondsSinceUtc);
		return CboxError::no_error;
	}

	virtual CboxError streamFrom(DataIn& in) override final {
	    ticks_seconds_t secondsSinceUtc;

		if(!in.get(secondsSinceUtc)){
		    return CboxError::input_stream_read_error;
		}
        ticks.setNow(secondsSinceUtc);
		return CboxError::no_error;
	}

	virtual obj_type_t typeId() const override final {
		// use function overloading and templates to manage type IDs in a central place (TypeRegistry)
		return 11;
	}
};

} // end namespace cbox


