/*
 * Copyright 2018 Elco Jacobs / BrewBlox
 *
 * This file is part of ControlBox
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

enum class CboxError : uint8_t {
    OK = 0,
    UNKNOWN_ERROR = 1,

    // object creation
    INSUFFICIENT_HEAP = 4,

    // generic stream errors
    STREAM_ERROR_UNSPECIFIED = 8,
    OUTPUT_STREAM_WRITE_ERROR = 9,
    INPUT_STREAM_READ_ERROR = 10,
    INPUT_STREAM_DECODING_ERROR = 11,
    OUTPUT_STREAM_ENCODING_ERROR = 12,

    // storage errors
    INSUFFICIENT_PERSISTENT_STORAGE = 16,
    PERSISTED_OBJECT_NOT_FOUND = 17,
    INVALID_PERSISTED_BLOCK_TYPE = 18,
    COULD_NOT_READ_PERSISTED_BLOCK_SIZE = 19,
    PERSISTED_BLOCK_STREAM_ERROR = 20,
    PERSISTED_STORAGE_WRITE_ERROR = 21,
    CRC_ERROR_IN_STORED_OBJECT = 22,

    // invalid actions
    OBJECT_NOT_WRITABLE = 32,
    OBJECT_NOT_READABLE = 33,
    OBJECT_NOT_CREATABLE = 34,
    OBJECT_NOT_DELETABLE = 35,

    // invalid parameters
    INVALID_COMMAND = 63,
    INVALID_OBJECT_ID = 64,
    INVALID_OBJECT_TYPE = 65,
    INVALID_OBJECT_PROFILES = 66,
    CRC_ERROR_IN_COMMAND = 67,
    OBJECT_DATA_NOT_ACCEPTED = 68,

    // freak events that should not be possible
    PERSISTING_TO_INACTIVE_OBJECT = 200,
};

inline uint8_t
asUint8(CboxError e)
{
    return static_cast<uint8_t>(e);
}

} // end namespace cbox
