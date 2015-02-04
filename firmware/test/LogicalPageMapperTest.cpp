/**
 * Copyright 2014  Matthew McGowan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "flashee-eeprom.h"
#include "FlashTestUtil.h"

using namespace Flashee;

// NB: The FakeFlashDevice is initially random content.
// And the LogicalPageMapperImpl doesn't initialize the storage either.
TEST(LogicalPageMapperTest, PageIsDurty_Small) {
    FakeFlashDevice fake(6, 50);
    fake.eraseAll();
    LogicalPageMapperImpl<> mapper(fake, 5);
    ASSERT_FALSE(mapper.pageIsDirty(0));
}

TEST(LogicalPageMapperTest, PageIsDurty_Large) {
    // this catches a bug
    FakeFlashDevice fake(6, STACK_BUFFER_SIZE*2+50);
    fake.eraseAll();
    LogicalPageMapperImpl<> mapper(fake, 5);
    ASSERT_FALSE(mapper.pageIsDirty(0));
}

TEST(LogicalPageMapperTest, MaxPage) {
    FakeFlashDevice fake(6, 50);
    LogicalPageMapperImpl<> mapper(fake, 5);
    ASSERT_EQ(mapper.maxPage(), 5);
}

TEST(LogicalPageMapperTest, LogicalPageCount) {
    FakeFlashDevice fake(6, 50);
    LogicalPageMapperImpl<> mapper(fake, 5);
    ASSERT_EQ(mapper.logicalPageCount, 5);
}

TEST(LogicalPageMapperTest, LogicalPageMapInitialized) {
    FakeFlashDevice fake(40, 50);
    LogicalPageMapperImpl<> mapper(fake, 20);
    mapper.formatIfNeeded();
    mapper.buildInUseMap();
    for (int i=0; i<mapper.logicalPageCount; i++) {
        ASSERT_LE(mapper.logicalPageMap[i], 39) << "logical page " << i << " maps to physical page " << mapper.logicalPageMap[i];
    }
}

TEST(LogicalPageMapperTest, WriteHeader) {
    FakeFlashDevice fake(40, 50);
    LogicalPageMapperImpl<> mapper(fake, 20);    
    mapper.formatIfNeeded();    
    mapper.writeHeader(5, 0x1234);
    
    uint16_t value;
    ASSERT_TRUE(fake.read(&value, fake.pageAddress(5), sizeof(value)));
    ASSERT_EQ(0x1234, value);
}

TEST(LogicalPageMapperTest, FormatWritesHeaderID) {
    FakeFlashDevice fake(40, 50);           // random content
    LogicalPageMapper<> mapper(fake, 20);   // ensure the last page is erased

    uint16_t value;
    ASSERT_TRUE(fake.read(&value, fake.pageAddress(39), sizeof(value)));
    ASSERT_EQ(0x2FFFu, value);    
}

TEST(LogicalPageMapperTest, LogicalMappingIsPersisted) {    
    FakeFlashDevice fake(40, 50);
    LogicalPageMapper<> mapper(fake, 20);

    const char* msg = "Hello";
    ASSERT_TRUE(mapper.writeString(msg, 75));
    
    char buf[10];       // sanity test - read that it was written.
    ASSERT_TRUE(mapper.readPage(buf, 75, sizeof(buf)));
    ASSERT_STREQ(buf, msg) << "string not correctly written";
    
    // now allocate a new mapper in the same storage
    LogicalPageMapper<> mapper2(fake, 20);
    
    memset(buf, 0, sizeof(buf));
    ASSERT_TRUE(mapper.readPage(buf, 75, sizeof(buf)));
    ASSERT_STREQ(buf, msg) << "expected data to be unchanged in original mapper.";

    memset(buf, 0, sizeof(buf));
    ASSERT_TRUE(mapper2.readPage(buf, 75, sizeof(buf)));
    ASSERT_STREQ(buf, msg) << "expected data to be persisted for use by second mapper";

}

TEST(LogicalPageMapperTest, ContentIsPersisted) {    

    FakeFlashDevice fake(256, 4096);
    LogicalPageMapper<> mapper(fake, 254);
    PageSpanFlashDevice span(mapper);
    
    char buf[4096*15];    
    char buf2[4096*15];    
    // fill memory
    for (int i=0; i<512*15; i++) {        
        buf[i] = i;        
    }
    
    ASSERT_TRUE(span.write(buf, 234, sizeof(buf)));
    span.read(buf2, 234, sizeof(buf));    
    ASSERT_TRUE(!memcmp(buf, buf2, sizeof(buf))) << "Initial read failed";
        
    LogicalPageMapper<> mapper2(fake, 254);
    ASSERT_TRUE(FlashTestUtil::assertSamePagewise(mapper, mapper2)) << "mapper compare failed";
    
    memset(buf2, -1, sizeof(buf2));
    span.read(buf2, 234, sizeof(buf));    
    ASSERT_TRUE(!memcmp(buf, buf2, sizeof(buf))) << "Read after compare failed";
       
    // do it twice, just to confirm that reading doesn't change the content
    ASSERT_TRUE(FlashTestUtil::assertSamePagewise(mapper, mapper2)) << "2nd mapper compare failed";
    
    memset(buf2, -1, sizeof(buf2));
    span.read(buf2, 234, sizeof(buf));    
    ASSERT_TRUE(!memcmp(buf, buf2, sizeof(buf))) << "2nd Read after compare failed";
}


const int blockOffset = 50;
const int blockSize = blockOffset*4;

bool verifyEepromArea(FlashDevice* device, uint8_t value) {
    uint8_t* buf = new uint8_t[blockSize];    
    flash_addr_t offset = flash_addr_t(value) * blockOffset;
    device->read(buf, offset, blockSize);
    bool success = true;
    for (int i=0; i<blockSize && success; i++) {
        if (buf[i]!=value) {
            success = false;            
        }
    }
    delete[] buf;
    return success;
}

void fillEepromArea(FlashDevice* device, uint8_t value) {
    char* buf = new char[blockSize];
    memset(buf, value, blockSize);
    flash_addr_t offset = flash_addr_t(value) * blockOffset;
    device->write(buf, offset, blockSize);
    delete[] buf;
    
}

TEST(LogicalPageMapperTest, StressReset) {
    FakeFlashDevice fake(384, 4096);    // the persistent flash.    
    {
    LogicalPageMapper<> mapper(fake, 254);
    mapper.eraseAll();
    }
    
    for (uint8_t i=0; i<255; i++) {
        {   // separate blocks so scope is restricted
            LogicalPageMapper<> mapper(fake, 254);
            PageSpanFlashDevice span(mapper);
            fillEepromArea(&span, i);
            ASSERT_TRUE(verifyEepromArea(&span, i)) << "Error on value " << i << " before emulated reset";
        }
        {
            LogicalPageMapper<> mapper(fake, 254);
            PageSpanFlashDevice span(mapper);
            ASSERT_TRUE(verifyEepromArea(&span, i)) << "Error on value " << i << " after emulated reset";
        }
    }
}

  

