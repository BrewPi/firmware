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

#ifndef FLASHDEVICETEST_H
#define	FLASHDEVICETEST_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "flashee-eeprom.h"
#include "FlashTestUtil.h"
#include "Generators.h"
#include <memory>

using namespace Flashee;

/**
 * Generator function to produce the flash device for each instance of the test
 * suite. 
 */
template <class T> FlashDevice* CreateFlashDevice();
template <class T> bool SupportsWriteErase() { return true; }

template <typename T> 
class FlashDeviceTest : public ::testing::Test {
protected:
        
    FlashDevice* const flash;
    
public:
    FlashDeviceTest() : flash(CreateFlashDevice<T>()) {}
    virtual ~FlashDeviceTest() { delete flash; }

    bool SupportsWriteErase() {
        return ::SupportsWriteErase<T>();
    }
    
    void assertPageIsReset(page_count_t page) {
        assertFillPageValue(page, 0xFF);
    }
    
    void eraseAll() {
        page_count_t count = this->flash->pageCount();    
        for (page_count_t i=0; i<count; i++) {
            ASSERT_TRUE(this->flash->erasePage(i*this->flash->pageSize())) << "unable to erase page " << i;
        }        
    }
    
    void assertPageMatches(page_count_t page, Generator& gen) {
        assertPageRegionMatches(page, 0, flash->pageSize(), gen);
    }
    
    void assertPageRegionMatches(page_count_t page, page_size_t offset, page_size_t end, Generator& gen) {
        uint8_t buf[128];        
        flash_addr_t base = flash->pageAddress(page);
        while (offset<end) {
            page_size_t toRead = min(sizeof(buf), end-offset);
            ASSERT_TRUE(flash->readPage(buf, base+offset, toRead)) << "unable to read flash addr:" << base+offset << "len:" << toRead;
            for (int i=0; i<toRead; i++) {
                EXPECT_EQ(buf[i], gen.next());
            }
            offset += toRead;
        }        
    }

    void fillPageValue(page_count_t page, uint8_t value) {
        ValueGenerator gen(value);
        fillPage(page, gen);
    }
    
    void assertFillPageValue(page_count_t page, uint8_t value) {
        ValueGenerator gen(value);
        assertPageMatches(page, gen);
    }

    void fillPage(page_count_t page, Generator& gen) {
        fillPageRegion(page, 0, flash->pageSize(), gen);
    }
    
    void fillPageRegion(page_count_t page, page_size_t offset, page_size_t end, Generator& gen) {
        uint8_t buf[128];
        flash_addr_t base = flash->pageAddress(page);
        while (offset<end) {
            page_size_t toRead = min(sizeof(buf), end-offset);
            for (page_size_t i=0; i<toRead; i++) {
                buf[i] = gen.next();
            }
            ASSERT_TRUE(flash->writePage(buf, base+offset, toRead)) << "unable to write flash addr:" << base+offset << "len:" << toRead;            
            offset += toRead;
        }
    }
    
    void fillRegion(FlashDevice* flash, flash_addr_t start, flash_addr_t end, Generator& gen, bool eraseWrite) {
        uint8_t buf[127];
        while (start<end) {                        
            page_size_t toWrite = min(sizeof(buf), end-start);
            for (page_size_t i=0; i<toWrite; i++) {
                buf[i] = gen.next();
            }
            if (eraseWrite)
                ASSERT_TRUE(flash->writeErasePage(buf, start, toWrite)) << "unable to write flash addr:" << start << "len:" << toWrite;            
            else
                ASSERT_TRUE(flash->writePage(buf, start, toWrite)) << "unable to write flash addr:" << start << "len:" << toWrite;            
            start += toWrite;
        }
    }

    void assertWrite(page_size_t offset, page_size_t count, const uint8_t* write, const uint8_t* read) {
        ASSERT_TRUE(this->flash->writePage(write, offset, count));        
        std::auto_ptr<uint8_t> _buf(new uint8_t[count]);
        uint8_t* buf = buf;        
        ASSERT_TRUE(this->flash->readPage(buf, offset, count));        
        ASSERT_THAT(std::vector<uint8_t>(buf, buf + count), 
            ::testing::ElementsAreArray(read, count));            
    }
    
    void assertEraseWrite(page_size_t offset, page_size_t count, const uint8_t* write) {
        ASSERT_TRUE(this->flash->writeErasePage(write, offset, count));        
        std::auto_ptr<uint8_t> _buf(new uint8_t[count]);
        uint8_t* buf = _buf.get();        
        ASSERT_TRUE(this->flash->readPage(buf, offset, count));        
        ASSERT_THAT(std::vector<uint8_t>(buf, buf + count), 
            ::testing::ElementsAreArray(write, count));            
    }
    
};


TYPED_TEST_CASE_P(FlashDeviceTest);

TYPED_TEST_P(FlashDeviceTest, HasNonZeroPageSize) {
    page_size_t size = this->flash->pageSize();
    EXPECT_GT(size, 0) << "page size should > 0";
}

TYPED_TEST_P(FlashDeviceTest, HasNonZeroPageCount) {
    page_count_t count = this->flash->pageCount();
    EXPECT_GT(count, 0) << "page count should > 0";
}

TYPED_TEST_P(FlashDeviceTest, PageAddress) {
    page_count_t count = this->flash->pageCount();
    while (count-->0) {
        EXPECT_EQ(this->flash->pageAddress(count), count*this->flash->pageSize()) << "page address different for page " << count;
    }    
}

TYPED_TEST_P(FlashDeviceTest, ErasePageResetsData) {
    this->eraseAll();    
    const page_count_t count = this->flash->pageCount();
    for (page_count_t i=0; i<count; i++) {
        this->assertPageIsReset(i);
    }    
}

TYPED_TEST_P(FlashDeviceTest, WriteDistinctValueToPages) {
    this->eraseAll();                    
    const page_count_t count = this->flash->pageCount();
    for (page_count_t i=0; i<count; i++) {
        this->fillPageValue(i, i);
        this->assertFillPageValue(i, i);
    }
    
    for (page_count_t i=0; i<count; i++) {
        this->assertFillPageValue(i, i);
    }
}

TYPED_TEST_P(FlashDeviceTest, ErasePageNoEffectOnOtherPages) {
    this->eraseAll();                    
    const page_count_t count = this->flash->pageCount();
    for (page_count_t i=0; i<count; i++) {
        this->fillPageValue(i, i);
        this->assertFillPageValue(i, i);
    }

    ASSERT_TRUE(this->flash->erasePage(this->flash->pageAddress(2))) << "unable to erase page 2";
    
    for (page_count_t i=0; i<count; i++) {
        if (i==2)
            this->assertPageIsReset(i);
        else
            this->assertFillPageValue(i, i);
    }
}

TYPED_TEST_P(FlashDeviceTest, SuccessiveWrittenValuesAreAnded) {
    ASSERT_TRUE(this->flash->erasePage(0)) << "Unable to erase page 0";
    
    // once a bit is set to 0, it cannot be unset.
    this->assertWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    this->assertWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xFF" "\xA9" "\x9A"));    
    this->assertWrite(10, 4, as_bytes("\xF0" "\xF8" "\x56" "\x98"), as_bytes("\x00" "\xF8" "\x00" "\x98"));        
    this->assertWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xF8" "\x00" "\x98"));
}

TYPED_TEST_P(FlashDeviceTest, CanWriteAfterErase) {
    ASSERT_TRUE(this->flash->erasePage(0)) << "Unable to erase page 0";
    this->assertWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    ASSERT_TRUE(this->flash->erasePage(0)) << "Unable to erase page 0";    
    this->assertWrite(10, 4, as_bytes("\xFD" "\xFF" "\xF9" "\x65"), as_bytes("\xFD" "\xFF" "\xF9" "\x65"));    
}


TYPED_TEST_P(FlashDeviceTest, EraseWriteAllowsBitsToBeSet) {
    if (!this->SupportsWriteErase())
        return;
    this->assertEraseWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    this->assertEraseWrite(10, 4, as_bytes("\xF0" "\xF8" "\x56" "\x98"));       
}


TYPED_TEST_P(FlashDeviceTest, EraseWritePreservesRestOfPage) {
    if (!this->SupportsWriteErase())
        return;
    
    ASSERT_TRUE(this->flash->erasePage(0)) << "Unable to erase page 0";
    SequenceGenerator seq(0);
    this->fillPage(0, seq);    
    this->assertEraseWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    SequenceGenerator seq2(0);
    this->assertPageRegionMatches(0, 0, 10, seq2);    
    SequenceGenerator seq3(14);
    this->assertPageRegionMatches(0, 14, this->flash->pageSize(), seq3);
}

TYPED_TEST_P(FlashDeviceTest, RepeatedEraseWritePreservesRestOfPage) {
    if (!this->SupportsWriteErase())
        return;
    
    ASSERT_TRUE(this->flash->erasePage(0)) << "Unable to erase page 0";
    SequenceGenerator seq(0);
    this->fillPage(0, seq);    
    
    for (int i=0; i<10; i++) {
        this->assertEraseWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
        SequenceGenerator seq2(0);
        this->assertPageRegionMatches(0, 0, 10, seq2);    
        SequenceGenerator seq3(14);
        this->assertPageRegionMatches(0, 14, this->flash->pageSize(), seq3);

        this->assertEraseWrite(10, 4, as_bytes("\xFF" "\x00" "\x9A" "\xA9"));
        SequenceGenerator seq4(0);
        this->assertPageRegionMatches(0, 0, 10, seq4);    
        SequenceGenerator seq5(14);
        this->assertPageRegionMatches(0, 14, this->flash->pageSize(), seq5);
    }
}

TYPED_TEST_P(FlashDeviceTest, WriteOverPageFails) {
    char buf[2];
    ASSERT_FALSE(this->flash->writePage(buf, this->flash->pageSize()-1, 2));
}

TYPED_TEST_P(FlashDeviceTest, WriteEraseOverPageFails) {
    char buf[2];
    ASSERT_FALSE(this->flash->writeErasePage(buf, this->flash->pageSize()-1, 2));
}

TYPED_TEST_P(FlashDeviceTest, ReadOverPageFails) {    
    char buf[2];
    ASSERT_FALSE(this->flash->readPage(buf, this->flash->pageSize()-1, 2));
}


/**
 * Randomly erases, writes and rewrites regions of flash memory, in parallel
 * to a fake implementation and the tested implementation.  After
 * each operation, the entire region of flash is compared.
 */
TYPED_TEST_P(FlashDeviceTest, StressTest) {
    if (!this->SupportsWriteErase())
        return;
    
    srand(0);       // make this deterministic
    
    const flash_addr_t length = this->flash->length();
    FakeFlashDevice expected(this->flash->pageCount(), this->flash->pageSize());        // emulate the same logical size
    //PageSpanFlashDevice actual(*this->flash);    
    FlashDevice& actual = *this->flash;
    
    ASSERT_EQ(length, expected.length());
    ASSERT_EQ(expected.pageSize(), this->flash->pageSize());
    ASSERT_EQ(expected.pageCount(), this->flash->pageCount());
    int pageSize = this->flash->pageSize();
    actual.eraseAll();
    expected.eraseAll();
    ASSERT_TRUE(FlashTestUtil::assertSamePagewise(expected, actual)) << " erase";
    char actualBuf[128];
    char expectedBuf[128];
    
    
    char msg[20];
    flash_addr_t start, end;
    for (int i=0; i<1000; i++) {
        int op = rand()%3;
        if (!(i%100))
            printf("step %d",i);
        start = rand()%length;        
        end = start + rand()%(length-start);
        end = start + rand()%(pageSize-(start%pageSize));
        const uint8_t startFrom = rand()&0xFF;
        SequenceGenerator gen1(startFrom);
        SequenceGenerator gen2(startFrom);
        switch (op) {
            case 0:                
                start -= (start%expected.pageSize());
                expected.erasePage(start);
                actual.erasePage(start);
                break;
            case 1:
            case 2:
                this->fillRegion(&expected, start, end, gen1, op==2);
                this->fillRegion(&actual, start, end, gen2, op==2);
                break;               
        }
        // now verify
        ASSERT_TRUE(FlashTestUtil::assertSamePagewise(expected, actual)) << " step " << i;
        ASSERT_TRUE(FlashTestUtil::assertSamePagewise(expected, actual)) << " step " << i << "(2nd compare)";
    }
}




REGISTER_TYPED_TEST_CASE_P(FlashDeviceTest, HasNonZeroPageSize, HasNonZeroPageCount,
                            PageAddress,
                            ErasePageResetsData, ErasePageNoEffectOnOtherPages,
                            WriteDistinctValueToPages, SuccessiveWrittenValuesAreAnded,
                            CanWriteAfterErase, EraseWriteAllowsBitsToBeSet,
                            EraseWritePreservesRestOfPage, RepeatedEraseWritePreservesRestOfPage,
                            WriteOverPageFails, WriteEraseOverPageFails, ReadOverPageFails,
                            StressTest
                            );



template <typename T>
class FlashSingleByteDeviceTest : public FlashDeviceTest<T> {
    
public:    
    
};

TYPED_TEST_CASE_P(FlashSingleByteDeviceTest);


TYPED_TEST_P(FlashSingleByteDeviceTest, CanReadSingleByte) {
    this->eraseAll();
    uint8_t buf[1];
    buf[0] = 0;
    ASSERT_TRUE(this->flash->readPage(buf, 0, 1));
    ASSERT_EQ(buf[0], 0xFF);
}

TYPED_TEST_P(FlashSingleByteDeviceTest, CanReadOddBytes) {
    this->eraseAll();     
    SequenceGenerator gen(0);
    this->fillPage(0u, gen);
    uint8_t buf[3];    
    ASSERT_TRUE(this->flash->readPage(buf, 4, 3));
    EXPECT_EQ(buf[0], 4);
    EXPECT_EQ(buf[1], 5);
    EXPECT_EQ(buf[2], 6);
}

TYPED_TEST_P(FlashSingleByteDeviceTest, CanReadOddBytesAtOddAddress) {
    this->eraseAll();
    SequenceGenerator gen(0);
    this->fillPage(0, gen);
    uint8_t buf[3];    
    ASSERT_TRUE(this->flash->readPage(buf, 5, 3));
    EXPECT_EQ(buf[0], 5);
    EXPECT_EQ(buf[1], 6);
    EXPECT_EQ(buf[2], 7);
}

TYPED_TEST_P(FlashSingleByteDeviceTest, CanReadEvenBytesAtOddAddress) {
    this->eraseAll();
    SequenceGenerator gen(0);
    this->fillPage(0, gen);
    uint8_t buf[4];    
    ASSERT_TRUE(this->flash->readPage(buf, 5, 4));
    EXPECT_EQ(buf[0], 5);
    EXPECT_EQ(buf[1], 6);
    EXPECT_EQ(buf[2], 7);
    EXPECT_EQ(buf[3], 8);
}


REGISTER_TYPED_TEST_CASE_P(FlashSingleByteDeviceTest, CanReadSingleByte, CanReadOddBytes,
        CanReadOddBytesAtOddAddress, CanReadEvenBytesAtOddAddress);



#endif	/* FLASHDEVICETEST_H */

