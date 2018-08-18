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

#include "Object.h"
#include "ObjectStorage.h"
#include "DataStreamEeprom.h"
#include "EepromAccessImpl.h"
#include "EepromAccess.h"
#include "EepromLayout.h"

namespace cbox {

enum class BlockType : uint8_t {
    invalid, // ensures cleared eeprom reads as invalid block type
    object,
    disposed_block,
};

inline bool operator==(const uint8_t & a, const BlockType & b){
    return a == static_cast<uint8_t>(b);
}

class EepromObjectStorage : public ObjectStorage {
public:
    EepromObjectStorage(EepromAccess & _eeprom) :
        eeprom(_eeprom),
        reader(_eeprom),
        writer(_eeprom)
    {
        init();
    }
    virtual ~EepromObjectStorage() = default;

    virtual CboxError streamObjectTo(DataOut& out, obj_id_t id) override final {
        RegionDataIn objectData = getObjectReader(id);
        stream_size_t size = objectData.available();
        if(size > 0){
            if(reader.push(out, size)){
                return CboxError::no_error;
            }
            return CboxError::output_stream_write_error;
        }
        return CboxError::persisted_object_not_found;
    }

    virtual CboxError streamAllObjectsTo(DataOut& out) override final {
        reader.reset(EepromLocation(objects), EepromLocationEnd(objects)-EepromLocation(objects));

        while(reader.hasNext()){
            uint8_t type = reader.next();
            // loop over all blocks and write objects to output stream
            uint16_t blockSize = 0;
            if (!reader.get(blockSize)){
                return CboxError::could_not_read_persisted_block_size;
            }

            bool gotEntireBlock;

            switch(type){
            case static_cast<uint8_t>(BlockType::object):
                // just copy block from EEPROM input stream to output stream
                gotEntireBlock = reader.push(out, blockSize);
                break;
            case static_cast<uint8_t>(BlockType::disposed_block):
                gotEntireBlock = reader.spool(blockSize);
                break;
            default:
                return CboxError::invalid_persisted_block_type; // unknown block type encountered!
                break;
            }
            if(!gotEntireBlock){
                return CboxError::persisted_block_stream_error;
            }
        }
        return CboxError::no_error;
    }

    virtual CboxError retreiveObject(obj_id_t id, Object & target) override final {
        RegionDataIn objectEepromData = getObjectReader(id);
        if(objectEepromData.available() == 0){
            return cbox::CboxError::persisted_object_not_found;
        }
        return target.streamFrom(objectEepromData);
    }

    virtual CboxError storeObject(obj_id_t id, Object & source) override final {
        CountingBlackholeDataOut counter;
        RegionDataOut objectEepromData = getObjectWriter(id);
        uint16_t blockSize = objectEepromData.availableForWrite();

        TeeDataOut tee(objectEepromData, counter);
        CboxError res = source.streamPersistedTo(tee);
        if(counter.count() > blockSize){
            // block didn't fit or not found, should allocate a new block
            if(blockSize > 0){
                // object did exist but didn't fit in old region, remove old region
                disposeObject(id);
            }

            uint16_t dataSize = counter.count();
            // overprovision at least 4 bytes or 12.5% to prevent having to relocate the block if it grows
            uint16_t overProvision = std::max(dataSize >> 3, 4);
            uint16_t requestedSize = dataSize + overProvision;
            RegionDataOut newObjectEepromData = newObjectWriter(id, requestedSize);
            if(newObjectEepromData.availableForWrite() < requestedSize){
                // not enough continuous free space
                if(freeSpace() < requestedSize){
                    return CboxError::insufficient_persistent_storage; // not even enough total free space
                }

                // if there is enough free space, but it is not continuous, do a defrag to and try again
                defrag();
                RegionDataOut newObjectEepromData2 = newObjectWriter(id, requestedSize);
                if(newObjectEepromData2.availableForWrite() < requestedSize){
                    return CboxError::insufficient_persistent_storage; // still not enough free space
                }
                res = source.streamPersistedTo(newObjectEepromData2);

            }
            else {
                // write to new Block
                res = source.streamPersistedTo(newObjectEepromData);
            }
        }
        return res;
    }

    virtual bool disposeObject(obj_id_t id) override final{
        RegionDataIn block = getObjectReader(id); // sets reader to data start of block data
        if(block.available() > 0){
            // overwrite block type with disposed block
            eptr_t dataStart = reader.offset();
            eptr_t blockTypeOffset = dataStart - blockHeaderLength() - sizeof(obj_id_t);
            eeprom.writeByte(blockTypeOffset, static_cast<uint8_t>(BlockType::disposed_block));
            mergeDisposedBlocks();
            return true;
        }
        return false;
    }

    stream_size_t freeSpace(){
        stream_size_t total = 0;
        resetReader();
        while(reader.available()){
           RegionDataIn blockData = getBlockReader(BlockType::disposed_block);
           total += blockData.available() + blockHeaderLength();
           blockData.spool();
        }
        // subtract one header length, because that will not be available for the object
        return total - blockHeaderLength();
    }

    stream_size_t continuousFreeSpace(){
        stream_size_t space = 0;
        resetReader();
        while(reader.available()){
           RegionDataIn blockData = getBlockReader(BlockType::disposed_block);
           space = std::max(space, blockData.available());
           blockData.spool();
        }
        // subtract one header length, because that will not be available for the object
        return space;
    }

    void defrag(){
        do {
            mergeDisposedBlocks();
        } while(moveDisposedBackwards());
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
        reader.reset(EepromLocation(objects), EepromLocationSize(objects));
    }
    void resetWriter(){
        writer.reset(EepromLocation(objects), EepromLocationSize(objects));
    }

    static const uint16_t blockHeaderLength(){
        return sizeof(BlockType) + sizeof(uint16_t);
    }

    static const uint16_t objectHeaderLength(){
        return blockHeaderLength() + sizeof(obj_id_t);
    }

    // This function assumes that the reader is at the start of a block.
    // To ensure this, after using the RegionDataIn object, call spool() on it.
    RegionDataIn getBlockReader(const BlockType requestedType){
        while(reader.hasNext()){
            uint8_t type = reader.next();
            uint16_t blockSize = 0;
            if(!reader.get(blockSize)){
                break; // couldn't read blocksize, due to reaching end of reader
            }

            if(!(type == requestedType)) {
                reader.spool(blockSize);
                continue;
            }
            return RegionDataIn(reader, blockSize);
        }
        return RegionDataIn(reader, 0);
    }

    // This function assumes that the reader is at the start of a block.
    RegionDataOut getBlockWriter(const BlockType requestedType, uint16_t minSize){
        RegionDataIn objectData = getBlockReader(requestedType); // this sets the eeprom to the object location
        if(objectData.available() >= minSize){
            // block found. now wrap the eeprom location with a writer instead of a reader
            writer.reset(reader.offset(), reader.available());
            return RegionDataOut(writer, objectData.available());
        }
        return RegionDataOut(writer, 0); // length 0 writer
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

    RegionDataOut newObjectWriter(const obj_id_t id, uint16_t size){
        // find a disposed block with enough size available
        resetReader();
        while(reader.available() >= size + sizeof(obj_id_t)){
            RegionDataIn blockData = getBlockReader(BlockType::disposed_block);
            uint16_t blockSize = uint16_t(blockData.available());
            if(blockSize < size){
                blockData.spool();
                continue;
            }
            // Large enough block found. now wrap the eeprom location with a writer instead of a reader
            if(blockSize < size + 8){
                // don't create new disposed blocks smaller than 8 bytes, add space to this object instead
                writer.reset(reader.offset() - blockHeaderLength(), blockSize + blockHeaderLength());
                writer.put(BlockType::object);
                writer.put(blockSize);
                writer.put(uint16_t(id));
                return RegionDataOut(writer, blockSize);
            }
            else{
                // split into object block and new disposed block
                eptr_t blockToSplitHeaderStart = reader.offset() - blockHeaderLength();
                uint16_t newObjectBlockSizeIncludingHeader = size + sizeof(obj_id_t) + blockHeaderLength();
                uint16_t newDisposedBlockSize = blockSize - newObjectBlockSizeIncludingHeader;
                uint16_t newDisposedBlockStart = blockToSplitHeaderStart + newObjectBlockSizeIncludingHeader;

                // first disposed block
                writer.reset(newDisposedBlockStart, blockHeaderLength());
                writer.put(BlockType::disposed_block);
                writer.put(newDisposedBlockSize);
                // then object block
                writer.reset(blockToSplitHeaderStart, newObjectBlockSizeIncludingHeader);
                writer.put(BlockType::object);
                writer.put(uint16_t(newObjectBlockSizeIncludingHeader - blockHeaderLength()));
                writer.put(uint16_t(id));
                return RegionDataOut(writer, size);
            }
        }
        return RegionDataOut(writer, 0); // length 0 writer
    }

    constexpr uint16_t referenceHeader(){
        return magicByte << 8 | storageVersion;
    }

    void init(){
        uint16_t header;
        resetReader();
        reader.get(header);
        if(header != referenceHeader()){
            eeprom.clear(); // writes zeros, active profiles is now also 0x00
            eeprom.put(EepromLocation(header), header);
            resetWriter();
            // make eeprom one big disposed block
            writer.put(BlockType::disposed_block);
            writer.put(uint16_t(EepromLocationSize(objects) - blockHeaderLength()));
        }
        static_assert(EepromLocationEnd(objects) == EepromAccess::length());
    }

    // move a single disposed block backwards by swapping it with an object
    bool moveDisposedBackwards(){
        resetReader();
        RegionDataIn disposedBlock = getBlockReader(BlockType::disposed_block);


        eptr_t disposedStart = reader.offset();
        uint16_t disposedLength = disposedBlock.available();
        if(disposedLength == 0){
            return false;
        }
        disposedBlock.spool();

        RegionDataIn objectBlock = getBlockReader(BlockType::object);
        uint16_t objectLength = objectBlock.available();

        if(objectLength == 0){
            return false;
        }

        // write object at location of disposed block and mark the remainder as disposed.
        // essentially, they swap places

        // The order of operations here is to prevent losing EEPROM block offsets/alignment when power is lost during the swap.
        // We first write the disposed length of the combined block, so that if power is lost, the entire block is treated as disposed and only 1 object is lost.
        writer.reset(disposedStart - sizeof(uint16_t), sizeof(uint16_t) + objectLength + blockHeaderLength());
        writer.put(uint16_t(disposedLength + objectLength + blockHeaderLength()));

        // Then we copy the data to the front of the block
        reader.push(writer, objectLength);

        // Then we mark the remainder as disposed
        writer.put(BlockType::disposed_block); // write header of the now discarded block data
        writer.put(disposedLength);

        // And finally we write the new header for the object that has moved forward
        writer.reset(disposedStart - blockHeaderLength() , blockHeaderLength());
        writer.put(BlockType::object);
        writer.put(objectLength);

        return true;
    }

    bool mergeDisposedBlocks(){
        resetReader();
        bool didMerge = false;
        while(reader.hasNext()){
            RegionDataIn disposedBlock1 = getBlockReader(BlockType::disposed_block);

            eptr_t disposedDataStart1 = reader.offset();
            uint16_t disposedDataLength1 = disposedBlock1.available();

            disposedBlock1.spool();

            uint8_t nextBlockType = reader.peek();
            if(nextBlockType == BlockType::disposed_block){
                RegionDataIn disposedBlock2 = getBlockReader(BlockType::disposed_block);
                uint16_t disposedLength2 = disposedBlock2.available();
                // now merge the blocks
                uint16_t combinedLength = disposedDataLength1 + disposedLength2 + blockHeaderLength();
                writer.reset(disposedDataStart1 - blockHeaderLength() + sizeof(BlockType), sizeof(uint16_t));
                writer.put(combinedLength);
                didMerge = true;
                disposedBlock2.spool();
            }
        }
        return didMerge;
    }
};

} // end namespace cbox

