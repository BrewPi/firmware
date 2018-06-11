#pragma once

#include "Block.h"
#include "SensorSetPointPair.pb.h"
#include "SensorSetPointPair.h"
#include "TempSensorDelegate.h"
#include "SetPointDelegate.h"
#include "CboxLink.h"

class SensorSetPointPairBlock: public Block {
private:
    CboxLookup sensorLookup;
    CboxLookup setpointLookup;

    TempSensorDelegate sensor;
    SetPointDelegate setpoint;
    SensorSetPointPair pair;

public:
    SensorSetPointPairBlock() :
        sensor(sensorLookup),
        setpoint(setpointLookup),
        pair(sensor, setpoint)
{}

    static const uint8_t persistedMaxSize(){
        static_assert(blox_SensorSetPointPair_Persisted_size < 128, "varint for settings size will be larger than 1 byte");
        return blox_SensorSetPointPair_Persisted_size + 1;
    }
    virtual uint8_t readStreamSize() override final {
        /* maximum size of settings  +1 for varint for length in delimited message */
        return persistedMaxSize();
    }

    virtual void writeFrom(cbox::DataIn& dataIn) override final {
        writeFromImpl(dataIn, true);
    }

    void writeFromImpl(cbox::DataIn& dataIn, bool storeToEeprom){
        blox_SensorSetPointPair_Persisted newData;
        /* stream in new settings, overwriting copy of old settings */
        size_t maxSize = persistedMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize};
        bool success = pb_decode_delimited_noinit(&stream, blox_SensorSetPointPair_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            sensorLookup.copyFrom(newData.links.sensor);
            setpointLookup.copyFrom(newData.links.setpoint);
            if(storeToEeprom){
                storeSettings();
            }
        }
    }

    static cbox::Object* create(cbox::ObjectDefinition& defn) {
        auto obj = new SensorSetPointPairBlock;
        if(obj != nullptr){
            obj->writeFromImpl(*defn.in, false);
        }
        return obj;
    }

    bool storeSettings(){
        if(eeprom_offset() == eptr_t(-1)){
            return false; /* EEPROM location is not set */
        }
        eptr_t offset = eeprom_offset();
        pb_ostream_t stream = { &eepromOutStreamCallback, &offset, eepromSize(), 0 };
        blox_SensorSetPointPair_Persisted definition;
        sensorLookup.copyTo(definition.links.sensor);
        setpointLookup.copyTo(definition.links.setpoint);
        bool status = pb_encode_delimited(&stream, blox_SensorSetPointPair_Persisted_fields, &definition);

        return status;
    }

    SensorSetPointPair & get() {
        return pair;
    }

    virtual Interface* getApplicationInterfaceImpl() override final {
        return &pair;
    }

    virtual void readTo(cbox::DataOut& out) override final {
        blox_SensorSetPointPair message;
        assert_size<sizeof(message.links.sensor), MAX_ID_CHAIN_LENGHT>();
        assert_size<sizeof(message.links.setpoint), MAX_ID_CHAIN_LENGHT>();
        sensorLookup.copyTo(message.links.sensor);
        setpointLookup.copyTo(message.links.setpoint);
        static_assert(blox_SensorSetPointPair_size < 128, "varint for size will be larger than 1 byte");
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_SensorSetPointPair_size + 1, 0 };
        pb_encode_delimited(&stream, blox_SensorSetPointPair_fields, &message);
    }

    virtual cbox::obj_type_t typeID() override {
    	// use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
    	return resolveTypeID(this);
    }
};
