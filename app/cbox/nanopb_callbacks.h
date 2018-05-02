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
#include "DataStream.h"

/* This binds the pb_ostream_t into the DataOut stream, which is passed as state in pb_ostream */
static bool dataOutStreamCallback(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
	cbox::DataOut * out = (cbox::DataOut *) stream->state;
    return out->writeBuffer(buf, count);
}

/* This binds the pb_istream_t into the DataIn stream, which is passed as state in pb_istream */
static bool dataInStreamCallback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
	cbox::DataIn * in = (cbox::DataIn *) stream->state;
    while(count--){
        if(in->hasNext()){
            *buf++ = in->next();
        }
        else{
            return false;
        }
    }
    return true;
}

/* This write a block of data to eeprom. The address is passed as stream state */
static bool eepromOutStreamCallback(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
    eptr_t dest = * (eptr_t* ) stream->state;
    eepromAccess.writeBlock(dest, buf, count);
    return true;
}
