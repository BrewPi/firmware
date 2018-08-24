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

#include "Container.h"
#include "Object.h"
#include <stdint.h>

namespace cbox {

using storage_id_t = uint16_t;

class ObjectStorage {
public:
    ObjectStorage() = default;
    virtual ~ObjectStorage() = default;

    using fromStorageHandler = std::function<CboxError(DataIn&)>;
    using toStorageHandler = std::function<CboxError(DataOut&)>;
    using fromStorageWithIdHandler = std::function<CboxError(const storage_id_t& id, DataIn&)>;

    virtual CboxError retrieveObject(const storage_id_t& id, const fromStorageHandler& handler) = 0;
    virtual CboxError storeObject(const storage_id_t& id, const toStorageHandler& handler) = 0;
    virtual CboxError retrieveObjects(const fromStorageWithIdHandler& handler) = 0;
    virtual bool disposeObject(const storage_id_t& id) = 0;

    virtual void clear() = 0;
};

} // end namespace cbox
