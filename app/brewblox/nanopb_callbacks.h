/*
 * Copyright 2017 BrewPi
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <pb_decode.h>
#include <pb_encode.h>

/* This binds the pb_ostream_t into the DataOut stream, which is passed as state in pb_ostream */
bool
dataOutStreamCallback(pb_ostream_t* stream, const pb_byte_t* buf, size_t count);

/* This binds the pb_istream_t into the DataIn stream, which is passed as state in pb_istream */
bool
dataInStreamCallback(pb_istream_t* stream, pb_byte_t* buf, size_t count);
