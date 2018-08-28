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

#pragma once

#include "CboxError.h"
#include "CboxMixins.h"
#include "DataStream.h"
#include "ObjectIds.h"
#include <limits>

namespace cbox {

using update_t = uint32_t;

class Object : virtual public ObjectMixin {
public:
    Object() = default;
    virtual ~Object() = default;

    /**
     * get the unique typeID of the Object
     * @return object type
     *
     */
    virtual obj_type_t typeId() const = 0;

    /**
	 * update the object, returns timestamp at which the object wants to be updated again (in ms).
	 */
    virtual update_t update(const update_t& now) = 0;

    /**
	 * Call this function in the body of the update function for Objects that don't need updating
	 * @param now: current time in milliseconds
	 * @return next update time, 24.8 days in the future
	 */
    static inline update_t update_never(const update_t& now)
    {
        return now + std::numeric_limits<update_t>::max() / 2;
    }

    /**
	 * Call this function in the body of the update function for Objects that don't need updating
	 * @param now: current time in milliseconds
	 * @return next update time, 24.8 days in the future
	 */
    inline update_t update_1s(const update_t& now)
    {
        return now + 1000;
    }

    /**
	 * Each object is at least stream readable. StreamTo streams to the given output
	 */
    virtual CboxError streamTo(DataOut& out) const = 0;

    /**
	 * An object can (optionally) receive new data from a DataIn stream.
	 */
    virtual CboxError streamFrom(DataIn& in) = 0;

    /**
	 * Objects can stream data they want persisted.
	 * The persisted data should be compatible with streamFrom, which is used to re-instantiate the object from the persisted data.
	 */
    virtual CboxError streamPersistedTo(DataOut& out) const = 0;

    virtual bool implements(const obj_type_t& iface) const = 0;
};

} // end namespace cbox
