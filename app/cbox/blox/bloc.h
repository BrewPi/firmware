#pragma once

#include "../EepromAwareWritableValue.h"
#include "../nanopb_callbacks.h"

/*
 * We use a few macros to define Blox compatible wrapped objects.
 *  BLOC_CLASS_SETTINGS_ONLY can be used for classes that only have settingsPtr.
 *  BLOC_CLASS_SETTINGS_AND_STATE(T, CONSTRUCTOR_ARG...) is used for classes that also have statePtr
 *  Settings can be streamed in and are stored to eeprom
 *  State is only streamed out
 *  ReadTo streams settings first and then state.
 *  A compatible proto file has to be written and the filename should match the wrapped class name.
 *  The proto file should have the format:
 *  message {
 *      message Settings {
 *         <format exactly matching the settings struct>
 *      }
 *      message State {
 *         <format exactly matching the state struct>
 *      }
 *  }
 *  The State submessage is only included for classes that have state.
 */

/* note: should we use returned value of pb_encode_delimited? */\
#define BLOC_CLASS_SETTINGS_ONLY(T, CONSTRUCTOR_ARG...) \
    BLOC_CLASS_IMPL(T, READTO_IMPL_SETTINGS_ONLY, CONSTRUCTOR_ARG)

#define BLOC_CLASS_SETTINGS_AND_STATE(T, CONSTRUCTOR_ARG...) \
    BLOC_CLASS_IMPL(T, READTO_IMPL_SETTINGS_AND_STATE, CONSTRUCTOR_ARG)

#define READTO_IMPL_SETTINGS_ONLY(T) \
    do { \
        blox_ ## T message; \
        memcpy(&message.settings, wrapped.settingsPtr(), sizeof(message.settings));\
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_ ## T ## _size + 2, 0 };\
        pb_encode_delimited(&stream, blox_ ## T ## _fields, &message); \
    } while(0);

#define READTO_IMPL_SETTINGS_AND_STATE(T) \
    do { \
        blox_ ## T message; \
        memcpy(&message.settings, wrapped.settingsPtr(), sizeof(message.settings));\
        memcpy(&message.state, wrapped.statePtr(), sizeof(message.state));\
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_ ## T ## _size + 2, 0 };\
        pb_encode_delimited(&stream, blox_ ## T ## _fields, &message); \
    } while(0);

#define BLOC_CLASS_IMPL(T, READTO_IMPL, CONSTRUCTOR_ARG...) \
class T ## Bloc: public EepromAwareWritableValue { \
private:\
    T wrapped;\
 \
public:\
    T ## Bloc() : \
    wrapped(CONSTRUCTOR_ARG)\
    {} \
 \
    static const size_t persistedMaxSize(){ \
        static_assert(blox_ ## T ## _Persisted_size < 128, "varint for settings size will be larger than 1 byte"); \
        return blox_ ## T ## _Persisted_size + 1; \
    } \
    virtual uint8_t readStreamSize() override final {\
        /* maximum size of settings  +1 for varint for length in delimited message */\
        return persistedMaxSize();\
    }\
 \
    virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override final {\
        /* copy old settings, because the update can be sparse and can only overwrite some of the values */\
        void * settingsPtr = wrapped.settingsPtr();\
        blox_ ## T ## _Persisted newData;\
        /* copy old settings in case of a sparse update */ \
        memcpy(&newData.settings, settingsPtr, sizeof(newData.settings));\
        /* stream in new settings, overwriting copy of old settings */\
        size_t maxSize = persistedMaxSize();\
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize, 0 };\
        bool success = pb_decode_delimited_noinit(&stream, blox_ ## T ## _Persisted_fields, &newData);\
        /* if no errors occur, write new settings to wrapped object */\
        if(success){\
            memcpy(settingsPtr, &newData.settings, sizeof(newData.settings));\
            storeSettings();\
        }\
    }\
 \
    static Object* create(ObjectDefinition& defn) {\
        auto obj = new_object(T ## Bloc);\
        if(obj != nullptr){ \
            obj->writeMaskedFrom(*defn.in, *defn.in);\
        }\
        return obj;\
    }\
 \
    bool storeSettings(){\
        if(eeprom_offset() == eptr_t(-1)){\
            return false; /* EEPROM location is not set */\
        }\
        eptr_t offset = eeprom_offset();\
        pb_ostream_t stream = { &eepromOutStreamCallback, &offset, readStreamSize(), 0 };\
        blox_ ## T ## _Persisted definition;\
        memcpy(&definition.settings, wrapped.settingsPtr(), sizeof(definition.settings));\
        bool status = pb_encode_delimited(&stream, blox_ ## T ## _Persisted_fields, &definition);\
 \
        return status;\
    }\
 \
    T & get() {\
        return wrapped;\
    }\
    virtual void readTo(DataOut& out) override final {\
        READTO_IMPL(T)\
    }\
 \
};



