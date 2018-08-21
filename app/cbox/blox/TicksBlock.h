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
#include "Ticks.pb.h"
#include "Block.h"


// provides a protobuf interface to the ticks object
template<typename T>
class TicksBlock : public cbox::Object
{
	T & ticks;

public:
	TicksBlock(T & _ticks) : ticks(_ticks){}


	virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final {
		blox_Ticks newData;
		cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_Ticks_fields, blox_Ticks_size);
		if(result == cbox::CboxError::no_error){
			ticks.setNow(newData.secondsSinceEpoch);
		}
		return result;
	}

	virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final {
		blox_Ticks message;
		message.secondsSinceEpoch = ticks.getNow();
		message.millisSinceBoot = ticks.millis();

		return streamProtoTo(out, &message, blox_Ticks_fields, blox_Ticks_size);
	}

	virtual cbox::obj_type_t typeId() const override final {
		return 262;
	}
};
