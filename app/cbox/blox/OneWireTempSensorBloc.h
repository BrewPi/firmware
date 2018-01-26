#pragma once

#include "bloc.h"
#include "OneWireTempSensor.pb.h"
#include "OneWireTempSensor.h"
#include "OneWireBusCBox.h"

// since we only have one then might as well reference it directly
// this will change when support for multiple busses is added.
extern OneWireBusCBox oneWireBus;

class OneWireTempSensorBloc: public Bloc {
private:
    OneWireTempSensor sensor;

public:
    OneWireTempSensorBloc() :
        sensor(&oneWireBus.oneWire())
{}

    static const size_t persistedMaxSize(){
        static_assert(blox_OneWireTempSensor_Persisted_size < 128, "varint for settings size will be larger than 1 byte");
        return blox_OneWireTempSensor_Persisted_size + 1;
    }
    virtual uint8_t readStreamSize() override final {
        /* maximum size of settings  +1 for varint for length in delimited message */
        return persistedMaxSize();
    }

    virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override final {
        writeFromImpl(dataIn, true);
    }

    void writeFromImpl(DataIn& dataIn, bool storeToEeprom){
        /* copy old settings, because the update can be sparse and can only overwrite some of the values */
        blox_OneWireTempSensor_Persisted newData;
        /* copy old settings in case of a sparse update */
        sensor.copySettingsTo(&newData.settings);
        /* stream in new settings, overwriting copy of old settings */
        size_t maxSize = persistedMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize, 0 };
        bool success = pb_decode_delimited_noinit(&stream, blox_OneWireTempSensor_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            sensor.copySettingsFrom(&newData.settings);
            if(storeToEeprom){
                storeSettings();
            }
        }
    }

    static Object* create(ObjectDefinition& defn) {
        auto obj = new OneWireTempSensorBloc;
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
        blox_OneWireTempSensor_Persisted definition;
        sensor.copySettingsTo(&definition.settings);
        bool status = pb_encode_delimited(&stream, blox_OneWireTempSensor_Persisted_fields, &definition);

        return status;
    }

    OneWireTempSensor & get() {
        return sensor;
    }

    Interface * getApplicationInterfaceImpl() override final{
        return &sensor;
    }

    virtual void readTo(DataOut& out) override final {
        blox_OneWireTempSensor message;
        sensor.copySettingsTo(&message.settings);
        sensor.copyStateTo(&message.state);
        static_assert(blox_OneWireTempSensor_size < 128, "varint for size will be larger than 1 byte");
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_OneWireTempSensor_size + 1, 0 };
        pb_encode_delimited(&stream, blox_OneWireTempSensor_fields, &message);
    }
};
