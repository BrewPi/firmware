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

 /**
  * Runs eeprom device tests against the address-erasable eeprom emulation.
  */

#pragma SPARK_NO_PREPROCESSOR

#include <memory>

#include "flashee-eeprom/flashee-eeprom.h"
#include "spark-unit.h"

using namespace Flashee;

class Generator {
public:
    virtual uint8_t next()=0;
};


class ValueGenerator : public Generator {
protected:
    uint8_t _value;
public:
    ValueGenerator(uint8_t value) : _value(value) {}

    uint8_t next() { return _value; }
};


class SequenceGenerator : public ValueGenerator {
public:
    SequenceGenerator(uint8_t startValue) : ValueGenerator(startValue) {}
    uint8_t next() { return _value++; }
};


class FlashDeviceTest {

public:
    FlashDevice* const flash;

    FlashDeviceTest(FlashDevice* dev) : flash(dev) {}

    void assertPageIsReset(page_count_t page) {
        assertFillPageValue(page, 0xFF);
    }

    void eraseAll() {
        page_count_t count = this->flash->pageCount();
        for (page_count_t i=0; i<count; i++) {
            assertTrue(this->flash->erasePage(i*this->flash->pageSize()));
        }
    }

    void assertPageMatches(page_count_t page, Generator& gen) {
        assertPageRegionMatches(page, 0, flash->pageSize(), gen);
    }

    void assertPageRegionMatches(page_count_t page, page_size_t offset, page_size_t end, Generator& gen) {
        uint8_t buf[128];
        flash_addr_t base = flash->pageAddress(page);
        while (offset<end) {
            page_size_t toRead = min(page_size_t(sizeof(buf)), end-offset);
            assertTrue(flash->readPage(buf, base+offset, toRead));
            for (page_size_t i=0; i<toRead; i++) {
                assertEqual(buf[i],gen.next());
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
            page_size_t toRead = min(page_size_t(sizeof(buf)), end-offset);
            for (page_size_t i=0; i<toRead; i++) {
                buf[i] = gen.next();
            }
            assertTrue(flash->writePage(buf, base+offset, toRead));
            offset += toRead;
        }
    }

    void assertWrite(page_size_t offset, page_size_t count, const uint8_t* write) {
        assertWrite(offset, count, write, write);
    }


    void assertWrite(page_size_t offset, page_size_t count, const uint8_t* write, const uint8_t* read) {
        assertTrue(this->flash->writePage(write, offset, count));
        std::auto_ptr<uint8_t> _buf(new uint8_t[count]);
        uint8_t* buf = _buf.get();
        assertTrue(this->flash->readPage(buf, offset, count));
        for (page_size_t i=0; i<count;i++) {
            assertEqual(buf[i],read[i]);
        }
    }

    void assertEraseWrite(page_size_t offset, page_size_t count, const uint8_t* write) {
        assertTrue(this->flash->writeErasePage(write, offset, count));
        std::auto_ptr<uint8_t> _buf(new uint8_t[count]);
        uint8_t* buf = _buf.get();
        assertTrue(this->flash->readPage(buf, offset, count));
        for (page_size_t i=0; i<count;i++) {
            assertEqual(buf[i],write[i]);
        }
    }

    void assertWritePageOffsetLength(page_size_t offset, page_size_t length, const uint8_t* write) {
        assertTrue(flash->erasePage(0));
        SequenceGenerator seq(0);
        fillPage(0, seq);
        assertWrite(offset, length, write);
        SequenceGenerator seq2(0);
        assertPageRegionMatches(0, 0, offset, seq2);
        SequenceGenerator seq3(offset+length);
        assertPageRegionMatches(0, offset+length, flash->pageSize(), seq3);
    }
};

FlashDeviceTest* test;

test(HasNonZeroPageSize) {
    page_size_t size = test->flash->pageSize();
    assertMore(size, 0);
}

test(HasNonZeroPageCount) {
    page_count_t count = test->flash->pageCount();
    assertMore(count, 0);
}

test(PageAddress) {
    page_count_t count = test->flash->pageCount();
    while (count-->0) {
        assertEqual(test->flash->pageAddress(count), count*test->flash->pageSize());
    }
}

test(ErasePageResetsData) {
    test->eraseAll();
    const page_count_t count = test->flash->pageCount();
    for (page_count_t i=0; i<count; i++) {
        test->assertPageIsReset(i);
    }
}

test(WriteDistinctValueToPages) {
    test->eraseAll();
    const page_count_t count = test->flash->pageCount();
    for (page_count_t i=0; i<count; i++) {
        test->fillPageValue(i, i);
        test->assertFillPageValue(i, i);
    }

    for (page_count_t i=0; i<count; i++) {
        test->assertFillPageValue(i, i);
    }
}

test(ErasePageNoEffectOnOtherPages) {
    test->eraseAll();
    const page_count_t count = test->flash->pageCount();
    for (page_count_t i=0; i<count; i++) {
        test->fillPageValue(i, i);
        test->assertFillPageValue(i, i);
    }

    assertTrue(test->flash->erasePage(test->flash->pageAddress(2)));

    for (page_count_t i=0; i<count; i++) {
        if (i==2)
            test->assertPageIsReset(i);
        else
            test->assertFillPageValue(i, i);
    }
}

test(SuccessiveWrittenValuesAreAnded) {
    assertTrue(test->flash->erasePage(0));

    // once a bit is set to 0, it cannot be unset.
    test->assertWrite(11, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    test->assertWrite(11, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    test->assertWrite(11, 4, as_bytes("\xF0" "\xF8" "\x56" "\x98"), as_bytes("\x00" "\xF8" "\x00" "\x98"));
    test->assertWrite(11, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xF8" "\x00" "\x98"));
}

test(CanWriteAfterErase) {
    assertTrue(test->flash->erasePage(0));
    test->assertWrite(11, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"), as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    assertTrue(test->flash->erasePage(0));
    test->assertWrite(11, 4, as_bytes("\xFD" "\xFF" "\xF9" "\x65"), as_bytes("\xFD" "\xFF" "\xF9" "\x65"));
}


test(EraseWriteAllowsBitsToBeSet) {
    test->assertEraseWrite(11, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    test->assertEraseWrite(11, 4, as_bytes("\xF0" "\xF8" "\x56" "\x98"));
}


test(EraseWritePreservesRestOfPage) {
    assertTrue(test->flash->erasePage(0));
    SequenceGenerator seq(0);
    test->fillPage(0, seq);
    test->assertEraseWrite(11, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
    SequenceGenerator seq2(0);
    test->assertPageRegionMatches(0, 0, 11, seq2);
    SequenceGenerator seq3(15);
    test->assertPageRegionMatches(0, 15, test->flash->pageSize(), seq3);
}

test(RepeatedEraseWritePreservesRestOfPage) {
    assertTrue(test->flash->erasePage(0));
    SequenceGenerator seq(0);
    test->fillPage(0, seq);

    for (int i=0; i<10; i++) {
        test->assertEraseWrite(10, 4, as_bytes("\x00" "\xFF" "\xA9" "\x9A"));
        SequenceGenerator seq2(0);
        test->assertPageRegionMatches(0, 0, 10, seq2);
        SequenceGenerator seq3(14);
        test->assertPageRegionMatches(0, 14, test->flash->pageSize(), seq3);

        test->assertEraseWrite(10, 4, as_bytes("\xFF" "\x00" "\x9A" "\xA9"));
        SequenceGenerator seq4(0);
        test->assertPageRegionMatches(0, 0, 10, seq4);
        SequenceGenerator seq5(14);
        test->assertPageRegionMatches(0, 14, test->flash->pageSize(), seq5);
    }
}

/**
 * Erases the page, and writes a sequence.
 * Then part of the page is overwritten, and the sequence is still verified.
 */
test(SparkFlashCanWriteOddBytes) {
    FlashDevice& flash = Devices::userFlash();
    FlashDeviceTest driver(&flash);
    // since the direct spark flash doesn't support erase write, just use a normal write, which ANDs with the existing content.
    // Existing content byte is the page offset & 0xFF.
    driver.assertWritePageOffsetLength(9, 4, as_bytes("\x00" "\x08" "\xA" "\x4"));
}

test(SparkFlashCanWriteEvenAddressBytes) {
    FlashDevice& flash = Devices::userFlash();
    FlashDeviceTest driver(&flash);
    driver.assertWritePageOffsetLength(10, 4, as_bytes("\x08" "\x0A" "\x04" "\x01"));
}

test(SparkFlashCanWriteEvenAddressBytesOddLength) {
    FlashDevice& flash = Devices::userFlash();
    FlashDeviceTest driver(&flash);
    driver.assertWritePageOffsetLength(10, 4, as_bytes("\x08" "\x0A" "\x04"));
}



void setup()
{
    Serial.begin(9600);
}


void loop()
{
    static bool run = false;

    if (Serial.available()) {
        char c = Serial.read();

        if (c=='t') {
            if (!run) {
                Serial.println("Running tests");
                run = true;
                // allocate the maximum size possible. At present this is 256 pages, keeping 2 free pages.
                test = new FlashDeviceTest(Devices::createAddressErase(0, 4096*256, 2));
                if (test==NULL)
                    Serial.println("Cannot allocate test harness");
            }
        }
    }
    if (run) {
        Test::run();
    }
}

