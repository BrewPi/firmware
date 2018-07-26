/*
 * Copyright 2018 Elco Jacobs / BrewBlox.
 *
 * This file is part of BrewBlox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "ObjectStorage.h"
#include "DataStreamEeprom.h"
#include "EepromAccess.h"
#include "EepromLayout.h"

namespace cbox {

class EepromObjectStorage : public ObjectStorage {
public:
    EepromObjectStorage(EepromAccess & _eeprom) :
        eeprom(_eeprom){

    }
    virtual ~EepromObjectStorage() = default;

    virtual StreamResult streamObjectTo(DataOut& out) override final;

    virtual StreamResult streamAllObjectsTo(DataOut& out) override final {
        EepromDataIn reader(eeprom);
        reader.reset(EepromLocation(objects), EepromLocationEnd(objects)-EepromLocation(objects));

        while(reader.hasNext()){
            BlockType type = reader.next();
            if(type == BlockType::eof){
                return StreamResult::success;
            }

            // loop over all blocks and write objects to output stream
            uint16_t blockSize = 0;
            if (!reader.get(blockSize)){
                return StreamResult::unexpected_end_of_input;
            }

            bool gotEntireBlock;

            switch(type){
            case BlockType::object:
                // just copy block from EEPROM input stream to output stream
                gotEntireBlock = reader.push(out, blockSize);
                break;
            default:
                // skip unknown block types
                gotEntireBlock = reader.skip(blockSize);
                break;
            }
            if(!gotEntireBlock){
                return StreamResult::unexpected_end_of_input;
            }
        }
        return StreamResult::success;
    }

    virtual StreamResult retreiveObject(obj_id_t id, Object & target) override final;
    virtual StreamResult storeBlock(obj_id_t id, const Object & source) override final;

private:
    /**
     * The eeprom stream that maintains the current write position in eeprom for the current profile.
     * For open profiles, this keeps a pointer to the end of the profile.
     */
    EepromDataOut writer;

    /**
     * The application supplied EEPROM storage class
     */
    EepromAccess& eeprom;

    const uint8_t magicByte = 0x69;
    const uint8_t storageVersion = 0x01;

    StreamResult findObject(const obj_id_t id, RegionDataIn & streamOfObject) {
        EepromDataIn reader(eeprom);
        reader.reset(EepromLocation(objects), EepromLocationEnd(objects)-EepromLocation(objects));

        while(reader.hasNext()){
            BlockType type = reader.next();

            if(type == BlockType::eof){
                return StreamResult::end_of_input;
            }

            // loop over all blocks and find a match
            uint16_t blockSize = 0;
            obj_id_t blockId = 0;
            if(!reader.get(blockSize)){
                return StreamResult::unexpected_end_of_input;
            }
            if(!reader.get(blockId)){
                return StreamResult::unexpected_end_of_input;
            }

            stream_size_t blockDataLeft = blockSize - sizeof(uint16_t) - sizeof(obj_id_t);
            if(blockId != id) {
                if(!reader.skip(blockDataLeft)){
                    return StreamResult::unexpected_end_of_input;
                }
                continue;
            }

            streamOfObject = RegionDataIn(reader, blockDataLeft);
            return StreamResult::success;
        }
        return StreamResult::end_of_input;
    }

    enum class BlockType : uint8_t {
        eof = 0, // using 0 as eof ensures cleared EEPROM reads as empty file
        object,
        disposed_block,

    };

    constexpr uint16_t referenceHeader(){
        return magicByte << 8 | storageVersion;
    }

    void init(){
        uint16_t header = eeprom.get(EepromLocation(header), header);
        if(header() != referenceHeader()){
            eeprom.clear(); // writes zeros
            eeprom.put(EepromLocation(header), header);
        }
        // active profiles is now 0x00
        // object storage is eof
    }

    eptr_t defrag();
};

/**
 * equivalent to /dev/null. Used for discarding output.
 */
BlackholeDataOut blackhole;

inline void writeEepromRange(EepromAccess& eepromAccess, eptr_t start, eptr_t end, uint8_t val) {
    while (start<end) {
        eepromAccess.writeByte(start++, val);
    }
}

void SystemProfile::initializeEeprom() {
    writeEepromRange(eepromAccess, startAddress, profilesEepromEnd, 0xFF);
}

void SystemProfile::initialize() {

}

} // end namespace cbox

