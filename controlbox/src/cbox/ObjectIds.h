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

#pragma once

#include <cstdint>

namespace cbox {

class obj_type_t {
public:
    obj_type_t()
        : id(0){};
    obj_type_t(const uint16_t& rhs)
        : id(rhs){};
    obj_type_t(const obj_type_t& rhs) = default;
    obj_type_t(obj_type_t& rhs) = default;
    obj_type_t(obj_type_t&& rhs) = default;

    obj_type_t& operator=(const uint16_t& rhs)
    {
        id = rhs;
        return *this;
    }

    operator uint16_t() const
    {
        return id;
    }

    bool isValid() const
    {
        return id > invalid().id;
    }

    static const obj_type_t start()
    {
        return obj_type_t(1);
    };
    static const obj_type_t invalid()
    {
        return obj_type_t(0);
    };

private:
    uint16_t id;
};

class obj_id_t {
public:
    obj_id_t()
        : id(0){};
    obj_id_t(const uint16_t& rhs)
        : id(rhs){};
    obj_id_t& operator=(const uint16_t& rhs)
    {
        id = rhs;
        return *this;
    }

    operator uint16_t() const
    {
        return id;
    }

    bool isValid() const
    {
        return id > invalid().id;
    }

    static const obj_id_t start()
    {
        return obj_id_t(1);
    };
    static const obj_id_t invalid()
    {
        return obj_id_t(0);
    };

    obj_id_t& operator++() // ++A
    {
        ++id;
        return *this;
    }

    obj_id_t operator++(int) // A++
    {
        obj_id_t temp = *this;
        ++id;
        return temp;
    }

private:
    uint16_t id;
};

} // end namespace cbox
