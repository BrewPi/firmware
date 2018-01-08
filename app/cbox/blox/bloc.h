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
        const size_t maxSize = settingsMaxSize(); \
        pb_ostream_t stream = { &dataOutStreamCallback, &out, maxSize, 0 };\
        pb_encode_delimited(&stream, blox_ ## T ## _Settings_fields, wrapped.settingsPtr()); \
    } while(0);

#define READTO_IMPL_SETTINGS_AND_STATE(T) \
    do { \
        const size_t maxSize = settingsMaxSize() + blox_ ## T ## _State_size + 1; \
        static_assert(blox_ ## T ## _State_size < 128, "varint for state size will be larger than 1 byte"); \
        pb_ostream_t stream = { &dataOutStreamCallback, &out, maxSize, 0 };\
        pb_encode_delimited(&stream, blox_ ## T ## _Settings_fields, wrapped.settingsPtr());\
        pb_encode_delimited(&stream, blox_ ## T ## _State_fields, wrapped.statePtr());\
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
    static const size_t settingsMaxSize(){ \
        static_assert(blox_ ## T ## _Settings_size < 128, "varint for settings size will be larger than 1 byte"); \
        return blox_ ## T ## _Settings_size + 1; \
    } \
    virtual uint8_t readStreamSize() override final {\
        /* maximum size of settings  +1 for varint for length in delimited message */\
        return settingsMaxSize();\
    }\
 \
    virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override final {\
        size_t maxSize = settingsMaxSize();\
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize, 0 };\
        bool status = pb_decode_delimited_noinit(&stream, blox_ ## T ## _Settings_fields, wrapped.settingsPtr());\
        if(status){\
            storeSettings();\
        }\
    }\
 \
    static Object* create(ObjectDefinition& defn) {\
        auto obj = new_object(T ## Bloc);\
        pb_istream_t stream = { &dataInStreamCallback, defn.in, defn.len, 0 };\
        /* note: should we use returned value of pb_decode_delimited_noinit? */\
        pb_decode_delimited_noinit(&stream, blox_ ## T ## _Settings_fields, obj->wrapped.settingsPtr());\
        return obj;\
    }\
 \
    bool storeSettings(){\
        if(eeprom_offset() == eptr_t(-1)){\
            return false; /* EEPROM location is not set */\
        }\
        eptr_t offset = eeprom_offset();\
        pb_ostream_t stream = { &eepromOutStreamCallback, &offset, readStreamSize(), 0 };\
        bool status = pb_encode_delimited(&stream, blox_ ## T ## _Settings_fields, wrapped.settingsPtr());\
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



