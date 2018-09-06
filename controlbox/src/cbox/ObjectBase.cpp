/*
 * Copyright 2018 Elco Jacobs / BrewBlox
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

#include "ObjectBase.h"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace cbox {
std::vector<uint16_t> allIds;

uint16_t
throwIdNotUnique(uint16_t id)
{
    auto pair = std::equal_range(allIds.begin(), allIds.end(), id);
    if (pair.first != pair.second) {
        // duplicate id!
        throw std::logic_error("ID " + std::to_string(id) + " not unique!");
    }
    allIds.insert(pair.first, id);
    return id;
}
} // end namespace cbox
