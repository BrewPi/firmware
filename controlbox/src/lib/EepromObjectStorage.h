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

enum class BlockType : uint8_t {
    eof = 0, // using 0 as eof ensures cleared EEPROM reads as empty file
    object,
    disposed_block,
};

bool operator==(const uint8_t & a, const BlockType & b){
    return a == static_cast<uint8_t>(b);
}

class EepromObjectStorage : public ObjectStorage {
public:
    EepromObjectStorage(EepromAccess & _eeprom) :
        eeprom(_eeprom),
        reader(_eeprom),
        writer(_eeprom)
    {

    }
    virtual ~EepromObjectStorage() = default;

    virtual StreamResult streamObjectTo(DataOut& out, obj_id_t id) override final {
        RegionDataIn objectData = getObjectReader(id);
        stream_size_t size = objectData.available();
        if(size > 0){
            if(reader.push(out, size)){
                return StreamResult::success;
            }
            return StreamResult::stream_error;
        }
        return StreamResult::end_of_input;
    }

    virtual StreamResult streamAllObjectsTo(DataOut& out) override final {
        reader.reset(EepromLocation(objects), EepromLocationEnd(objects)-EepromLocation(objects));

        while(reader.hasNext()){
            uint8_t type = reader.next();
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
            case static_cast<uint8_t>(BlockType::object):
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

    virtual StreamResult retreiveObject(obj_id_t id, Object & target) override final {
        RegionDataIn objectEepromData = getObjectReader(id);
        return target.streamFrom(objectEepromData);
    }

    virtual StreamResult storeObject(obj_id_t id, Object & source) override final {
        RegionDataOut objectEepromData = getObjectWriter(id);
        return source.streamTo(objectEepromData);
    }

private:
    /**
     * The application supplied EEPROM storage class
     */
    EepromAccess& eeprom;


    /**
     * Stream wrappers for reading, writing and limiting region
     */
    EepromDataIn reader;
    EepromDataOut writer;


    const uint8_t magicByte = 0x69;
    const uint8_t storageVersion = 0x01;

    void resetReader(){
        reader.reset(EepromLocation(objects), EepromLocationEnd(objects)-EepromLocation(objects));
    }

    // This function assumes that the reader is at the start of a block.
    // To ensure this, after using the RegionDataIn object, call spool() on it.
    RegionDataIn getBlockReader(const BlockType requestedType){
        while(reader.hasNext()){
            uint8_t type = reader.next();
            uint16_t blockSize = 0;
            reader.get(blockSize);

            uint16_t blockDataLeft = blockSize - sizeof(uint16_t);

            if(!(type == requestedType)) {
                reader.skip(blockDataLeft);
                continue;
            }
            return RegionDataIn(reader, blockDataLeft);
        }
        return RegionDataIn(reader, 0);
    }

    // Search for the block matching the requested id
    // If found, return an EEPROM data stream limited to the block
    // This function assumes that the reader is at the start of a block.
    // To ensure this, after using the RegionDataIn object, call spool() on it.
    RegionDataIn getObjectReader(const obj_id_t id) {
        resetReader();
        while(reader.hasNext()){
            RegionDataIn block = getBlockReader(BlockType::object);
            obj_id_t blockId = 0;
            block.get(blockId);

            if(blockId != id) {
                block.spool();
                continue;
            }
            return block;
        }
        return RegionDataIn(reader, 0);
    }

    RegionDataOut getObjectWriter(const obj_id_t id){
        RegionDataIn objectData = getObjectReader(id); // this sets the eeprom to the object location
        if(objectData.available() > 0){
            // block found. now wrap the eeprom location with a writer instead of a reader
            writer.reset(reader.offset(), reader.available());
            return RegionDataOut(writer, objectData.available());
        }
        return RegionDataOut(writer, 0); // length 0 writer
    }

    virtual bool disposeObject(obj_id_t id) override final{
        RegionDataIn block = getObjectReader(id); // sets reader to data start of block
        if(block.available() > 0){
            eptr_t dataStart = reader.offset();
            uint16_t headerLength = sizeof(BlockType) + sizeof(uint16_t);
            eptr_t blockTypeOffset = dataStart - headerLength;
            eeprom.writeByte(blockTypeOffset, static_cast<uint8_t>(BlockType::disposed_block));
            return true;
        }
        return false;
    }

    constexpr uint16_t referenceHeader(){
        return magicByte << 8 | storageVersion;
    }

    void init(){
        uint16_t header;
        resetReader();
        reader.get(header);
        if(header != referenceHeader()){
            eeprom.clear(); // writes zeros
            eeprom.put(EepromLocation(header), header);
        }
        // active profiles is now 0x00
        // object storage is eof
    }

    // move a single disposed block backwards by swapping it with an object
    bool moveDisposedBackwards(){
        resetReader();
        RegionDataIn disposedBlock = getBlockReader(BlockType::disposed_block);

        eptr_t disposedStart = reader.offset();
        uint16_t disposedLength = disposedBlock.available();

        RegionDataIn objectBlock = getBlockReader(BlockType::object);

        eptr_t objectStart = reader.offset();
        uint16_t objectLength = disposedBlock.available();

        uint16_t headerLength = sizeof(BlockType) + sizeof(uint16_t);

        // write object at location of disposed block and mark the remainder as disposed.
        // essentially, they swap places
        writer.reset(disposedStart, objectLength);

        if(reader.push(writer, objectLength)){ // true on success
            writer.reset(disposedStart - headerLength , headerLength);
            writer.put(BlockType::object); // write header for new block location
            writer.put(objectLength);
            writer.reset(objectStart - headerLength , headerLength);
            writer.put(BlockType::disposed_block); // write header of the now discarded block data
            writer.put(disposedLength);
            return true;
        }
        return false;
    }

    bool mergeDisposedBlocks(){
        resetReader();
        bool didMerge = false;
        while(reader.hasNext()){
            RegionDataIn disposedBlock1 = getBlockReader(BlockType::disposed_block);

            eptr_t disposedStart1 = reader.offset();
            uint16_t disposedLength1 = disposedBlock1.available();

            disposedBlock1.spool();

            eptr_t nextBlockStart = reader.offset();
            uint8_t nextBlockType = reader.peek();
            if(nextBlockType == BlockType::disposed_block){
                RegionDataIn disposedBlock2 = getBlockReader(BlockType::disposed_block);
                eptr_t disposedStart2 = reader.offset();
                uint16_t disposedLength2 = disposedBlock2.available();
                // now merge the blocks
                uint16_t combinedLength = disposedLength1 + disposedLength2 + sizeof(BlockType) + sizeof(uint16_t);
                writer.reset(disposedStart1 - sizeof(uint16_t), sizeof(uint16_t));
                writer.put(combinedLength);
                didMerge = true;
            }
        }
        return didMerge;
    }

    void degrag(){
        do {
            mergeDisposedBlocks();

        } while(moveDisposedBackwards());
    }
};

} // end namespace cbox

