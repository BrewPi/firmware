#pragma once
#include "Values.h"
#include "SetPointSimple.pb.h"
#include <assert.h>

/* This binds the pb_ostream_t into the DataOut stream, which is passed as state in pb_ostream */
bool dataOutStreamCallback(pb_ostream_t *stream, const uint8_t *buf, size_t count)
{
    DataOut * out = (DataOut *) stream->state;
    return out->writeBuffer(buf, count);
}

/* This binds the pb_istream_t into the DataIn stream, which is passed as state in pb_istream */
bool dataInStreamCallback(pb_istream_t *stream, const uint8_t *buf, size_t count)
{
    DataIn * in = (DataIn *) stream->state;
    while(count--){
        if(in->hasNext()){
            buf++ = in->next();
        }
        else{
            return false;
        }
    }
    return true;
}


class SetPointSimpleCBox : public WritableValue {

	SetPointSimple setpoint;

	SetPointSimpleCBox(blox_SetPointSimple & settings)
	: settings(settings){
	}

public:

	virtual void readTo(DataOut& out) override {
	    pb_ostream_t stream = {&dataOutStreamCallback, &out, SIZE_MAX, 0};
        bool status = pb_encode_delimited(&stream, blox_SetPointSimple_fields, &setpoint.settings);
	}

	virtual uint8_t readStreamSize() override {
		return 0;
	}

	virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override {

	}

	static Object* create(ObjectDefinition& defn) {
	    blox_SetPointSimple settings = blox_SetPointSimple_init_default;
	    pb_istream_t stream = {&dataInStreamCallback, &defn.in, defn.len, 0};
	    bool status = pb_decode_delimited(&stream, blox_SetPointSimple_fields, &settings);

		return new_object(SetPointSimpleCBox(settings));
	}
};
