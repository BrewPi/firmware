#pragma once

#include "Block.h"
#include "OneWireBusBlock.h"
#include "OneWireTempSensor.pb.h"
#include "OneWireTempSensor.h"

// since we only have one then might as well reference it directly
// this will change when support for multiple busses is added.
extern OneWireBusBlock oneWireBus;

class OneWireTempSensorBlock: public Block {
private:
    OneWireTempSensor sensor;

public:
    OneWireTempSensorBlock() :
        sensor(&oneWireBus.oneWire())
{}

    static const uint8_t persistedMaxSize(){
        static_assert(blox_OneWireTempSensor_Persisted_size < 128, "varint for settings size will be larger than 1 byte");
        return blox_OneWireTempSensor_Persisted_size + 1;
    }
    virtual uint8_t readStreamSize() override final {
        /* maximum size of settings  +1 for varint for length in delimited message */
        return persistedMaxSize();
    }

    virtual void writeFrom(cbox::DataIn& dataIn) override final {
        writeFromImpl(dataIn, true);
    }

    void writeFromImpl(cbox::DataIn& dataIn, bool storeToEeprom){
        blox_OneWireTempSensor_Persisted newData;
        size_t maxSize = persistedMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize};
        bool success = pb_decode_delimited_noinit(&stream, blox_OneWireTempSensor_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            sensor.copySettingsFrom(&newData.settings);
            if(storeToEeprom){
                storeSettings();
            }
        }
    }

    static cbox::Object* create(cbox::ObjectDefinition& defn) {
        auto obj = new OneWireTempSensorBlock;
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

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &sensor;
    }

    virtual void readTo(cbox::DataOut& out) override final {
        blox_OneWireTempSensor message;
        assert_size<sizeof(message.settings), OneWireTempSensor::sizeof_Settings>();
        assert_size<sizeof(message.state), OneWireTempSensor::sizeof_State>();
        sensor.copySettingsTo(&message.settings);
        sensor.copyStateTo(&message.state);
        static_assert(blox_OneWireTempSensor_size < 128, "varint for size will be larger than 1 byte");
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_OneWireTempSensor_size + 1, 0 };
        pb_encode_delimited(&stream, blox_OneWireTempSensor_fields, &message);
    }

    virtual cbox::obj_type_t typeID() override {
    	// use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
    	return resolveTypeID(this);
    }
};
