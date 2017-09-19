#pragma once

#include <pb_decode.h>
#include <pb_encode.h>
#include "DataStream.h"

bool dataOutStreamCallback(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
    DataOut * out = (DataOut *) stream->state;
    return out->writeBuffer(buf, count);
}

/* This binds the pb_istream_t into the DataIn stream, which is passed as state in pb_istream */
bool dataInStreamCallback(pb_istream_t *stream, uint8_t *buf, size_t count)
{
    DataIn * in = (DataIn *) stream->state;
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
