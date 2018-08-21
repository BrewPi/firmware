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

#include "Object.h"
#include "DataStream.h"
#include "Profiles.pb.h"
#include "Block.h"
#include "Container.h"

// provides a protobuf interface to change the profile setting of a container
class ProfilesBlock : public cbox::Object
{
	cbox::ObjectContainer & container;

public:
	ProfilesBlock(cbox::ObjectContainer & _container) : container(_container){}


	virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final {
		blox_Profiles newData;
		cbox::CboxError result = streamProtoFrom(dataIn, &newData, blox_Profiles_fields, blox_Profiles_size);
		if(result == cbox::CboxError::no_error){
			container.setActiveProfiles(newData.active);
		}
		return result;
	}

	virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final {
		blox_Profiles message;
		message.active = container.getActiveProfiles();
		return streamProtoTo(out, &message, blox_Profiles_fields, blox_Profiles_size);
	}

	virtual cbox::obj_type_t typeId() const override final {
		return resolveTypeId(this);
	}
};



