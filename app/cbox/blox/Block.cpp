/*
 * Block.cpp TODO
 *
 *  Created on: Jul 23, 2018
 *      Author: elco
 */

#include "Block.h"


cbox::StreamResult streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize){
    pb_ostream_t stream = { dataOutStreamCallback, &out, maxSize, 0 };
    bool success = pb_encode(&stream, fields, srcStruct);
    out.write(0); // zero terminate every write, so protobuf will stop processing on encountering this zero field tag
    return (success) ? cbox::StreamResult::success : cbox::StreamResult::stream_error;
}

cbox::StreamResult streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize){
    pb_istream_t stream = { dataInStreamCallback, &in, maxSize};
    bool success = pb_decode(&stream, fields, destStruct);
    /* if no errors occur, write new settings to wrapped object */
    if(!success){
        return cbox::StreamResult::stream_error;
    }
    return cbox::StreamResult::success;
}
