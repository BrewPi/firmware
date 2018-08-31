/*
 * Block.cpp TODO
 *
 *  Created on: Jul 23, 2018
 *      Author: elco
 */

#include "blox/Block.h"

#include "cbox/DataStream.h"
#include "nanopb_callbacks.h"

cbox::CboxError
streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize)
{
    pb_ostream_t stream = {dataOutStreamCallback, &out, maxSize, 0};
    bool success = pb_encode(&stream, fields, srcStruct);
    out.write(0); // zero terminate every write, so protobuf will stop processing on encountering this zero field tag

    return (success) ? cbox::CboxError::OK : cbox::CboxError::OUTPUT_STREAM_ENCODING_ERROR;
}

cbox::CboxError
streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize)
{
    pb_istream_t stream = {dataInStreamCallback, &in, maxSize + 1}; // 1 extra byte for the zero termination
    bool success = pb_decode(&stream, fields, destStruct);

    return (success) ? cbox::CboxError::OK : cbox::CboxError::INPUT_STREAM_DECODING_ERROR;
}
