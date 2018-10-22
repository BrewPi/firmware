
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

#include "Object.h"
#include "ObjectContainer.h"
#include <memory>

namespace cbox {

/**
 * A scanning factory has some kind of scan mechanism for new objects.
 * It has a reference to the object container to check if the new object already exists
 */
class ScanningFactory {
protected:
    ObjectContainer& objectsRef;

public:
    ScanningFactory(ObjectContainer& objects)
        : objectsRef(objects)
    {
    }
    virtual ~ScanningFactory() = default;

    virtual void reset() = 0;
    virtual std::shared_ptr<Object> scan() = 0;

    obj_id_t scanAndAdd()
    {
        if (auto newObj = scan()) {
            return objectsRef.add(newObj, uint8_t(0xFF));
        }
        return 0;
    }
};

} // end namespace cbox
