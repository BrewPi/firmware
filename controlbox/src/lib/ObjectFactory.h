
/*
 * Copyright 2018 Elco Jacobs / BrewBlox, based on earlier work of Matthew McGowan
 *
 * This file is part of ControlBox.
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

#include "Object.h"
#include <memory>
#include <array>
#include "DataStream.h"

namespace cbox {

std::unique_ptr<cbox::Object> createApplicationObject(cbox::obj_type_t typeId, cbox::DataIn& in, cbox::CommandError& errorCode);

template<class T>
std::unique_ptr<Object> createObject(){
    return std::make_unique<T>();
}

// An object factory combines the create function with a type ID.
// They can be put in a container that can be walked to find the matching typeId
struct ObjectFactoryEntry {
    obj_type_t typeId;
    std::unique_ptr<Object> (*createFn)();
};

template<int n>
class ObjectFactory {
private:
	const std::array<ObjectFactoryEntry, n> objTypes;

public:

	constexpr ObjectFactory(ObjectFactoryEntry (&userObjectFactoryEntries)[n]) :
		objTypes(userObjectFactoryEntries)
	{
	}

	std::unique_ptr<Object> create(obj_type_t t){
		auto factoryEntry = std::find_if(objTypes.begin(), objTypes.end(), [&t](ObjectFactoryEntry & entry){ return entry.typeId == t;});
		if(factoryEntry == objTypes.end()){
			return nullptr;
		}
		return (*factoryEntry).createFn();
	}
};

} // end namespace cbox
