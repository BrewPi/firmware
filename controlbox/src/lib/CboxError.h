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

namespace cbox {

enum class CboxError : uint8_t {
    no_error = 0,
    unknown_error = 1,

    // storage errors
    insufficient_persistent_storage = 16,
    persisted_object_not_found = 17,
    invalid_persisted_block_type = 18,
    could_not_read_persisted_block_size = 19,
    persisted_block_stream_error,
    persisted_storage_write_error,

    // object creation
    insufficient_heap = 17,

    object_not_writable = 32,
    object_not_readable = 33,
    object_not_creatable = 34,
    object_not_deletable = 35,

    stream_error_unspecified = 3,
    output_stream_write_error= 5,
    input_stream_read_error = 6,
    input_stream_decoding_error = 7,
    output_stream_encoding_error = 8,

    invalid_command = 63,
    invalid_object_id = 65,
    invalid_object_type = 66,
    invalid_object_profiles = 68,
};

inline uint8_t asUint8(CboxError e){
    return static_cast<uint8_t>(e);
}


} // end namespace cbox
