#pragma once

#include "Values.h"
#include "SetPointSimple.pb.h"
#include "../nanopb_callbacks.h"
#include "SetPoint.h"


class SetPointSimpleCBox : public WritableValue {
private:
	SetPointSimple setpoint;
public:
	SetPointSimpleCBox(){}


	virtual void readTo(DataOut& out) override {
	    pb_ostream_t stream = {&dataOutStreamCallback, &out, SIZE_MAX, 0};
        bool status = pb_encode_delimited(&stream, blox_SetPointSimple_fields, &setpoint.settings);
	}

	virtual uint8_t readStreamSize() override {
		return 0;
	}

	virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override {
	    pb_istream_t stream = {&dataInStreamCallback, &dataIn, SIZE_MAX, 0};
        bool status = pb_decode_delimited_noinit(&stream, blox_SetPointSimple_fields, &setpoint.settings);
	}

	static Object* create(ObjectDefinition& defn) {
	    auto obj = new_object(SetPointSimpleCBox);
	    obj->writeMaskedFrom(*defn.in, *defn.in); // pass defn.in as mask, because the mask isn't used anyway.
		return obj;
	}

	SetPointSimple& get(){
	    return setpoint;
	}
};
