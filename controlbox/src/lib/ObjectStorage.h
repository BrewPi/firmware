/*
 * Copyright 2018 Elco Jacobs / BrewBlox.
 *
 * This file is part of BrewBlox.
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

#include <stdint.h>
#include "Object.h"
#include "Container.h"

namespace cbox {

class ObjectStorage {
public:
    ObjectStorage() = default;
    virtual ~ObjectStorage() = default;

    virtual CboxError streamObjectTo(DataOut& out, const obj_id_t & id) = 0;
    virtual CboxError streamAllObjectsTo(DataOut& out) = 0;
    virtual CboxError retreiveObject(const obj_id_t & id, Object & target) = 0;
    virtual CboxError storeObject(const obj_id_t & id, const Object & source) = 0;
    virtual bool disposeObject(const obj_id_t & id) = 0;

    using StreamedObjectHandler = std::function<CboxError (DataIn &)>;

    virtual CboxError retrieveObjects(const StreamedObjectHandler & handler) = 0;
    virtual void clear() = 0;
};

} // end namespace cbox
