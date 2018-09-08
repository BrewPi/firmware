#include "nanopb_callbacks.h"

#include "cbox/DataStream.h"

/* This binds the pb_ostream_t into the DataOut stream, which is passed as state in pb_ostream */
bool
dataOutStreamCallback(pb_ostream_t* stream, const pb_byte_t* buf, size_t count)
{
    auto out = static_cast<cbox::DataOut*>(stream->state);
    return out->writeBuffer(buf, count);
}

/* This binds the pb_istream_t into the DataIn stream, which is passed as state in pb_istream */
bool
dataInStreamCallback(pb_istream_t* stream, pb_byte_t* buf, size_t count)
{
    auto in = static_cast<cbox::DataIn*>(stream->state);
    return in->read(buf, count);
}
