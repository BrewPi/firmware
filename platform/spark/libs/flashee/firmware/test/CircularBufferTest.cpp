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
#include "flashee-eeprom.h"
#include "Generators.h"

using namespace Flashee;

class CircularBufferTest : public ::testing::Test {
    
    protected:
        FakeFlashDevice base;
        CircularBuffer sut;
    
        page_size_t fillBuffer(page_size_t max=page_size_t(-1)) {
            char buf[50];
            page_size_t written = 0;
            page_size_t w;
            do {
                w = this->sut.write_soft(buf, min(page_size_t(50), max-written));
                written += w;
            }
            while (w!=0);    
            return written;
        }
        
        page_size_t fillBufferWith(PushBackGenerator& gen, page_size_t max=page_size_t(-1)) {
            char buf[256];
            page_size_t written = 0;
            page_size_t w;
            page_size_t toWrite;
            do {
                toWrite = min(page_size_t(sizeof(buf)), max-written);
                for (int i=0; i<toWrite; i++)
                    buf[i] = gen.next();                
                w = this->sut.write_soft(buf, toWrite);
                for (int i=toWrite; i-->w; ) {
                    gen.push_back(buf[i]);
                }
                written += w;
            }
            while (w!=0);    
            return written;
        }        
        
        page_size_t drainBuffer(page_size_t max=page_size_t(-1)) {
            char buf[50];
            page_size_t read  = 0;
            page_size_t r;
            
            do {                
                r = this->sut.read_soft(buf, min(page_size_t(50), max-read));
                read += r;
            }
            while (r!=0);    
            return read;
        }

        void assertBufferEqual(uint8_t* buf, page_size_t r, Generator& gen) {
            for (page_size_t i=0; i<r; i++) {
                uint8_t next = gen.next();
                ASSERT_EQ(next, buf[i]);
            }            
        }
        
        page_size_t verifyBufferFrom(Generator& gen, page_size_t max=page_size_t(-1)) {
            uint8_t buf[50];
            page_size_t read  = 0;
            page_size_t r;
            
            do {                
                r = this->sut.read_soft(buf, min(page_size_t(50), max-read));
                assertBufferEqual(buf, r, gen);
                read += r;
            }
            while (r!=0);    
            return read;
        }
       
        
    public:        
        CircularBufferTest() : base(10, 20), sut(base) {}
};

TEST_F(CircularBufferTest, CannotReadWhenEmpty) {
    char buf[10];
    EXPECT_EQ(0, this->sut.available());
    ASSERT_EQ(0, this->sut.read(buf, 10)) << "Expected no data to be read";
}

TEST_F(CircularBufferTest, CanFillBuffer) {
    page_size_t written = this->fillBuffer();
    ASSERT_EQ(written, this->base.length());
    ASSERT_EQ(written, this->sut.available());
}

TEST_F(CircularBufferTest, CanDrainBuffer) {
    page_size_t written = this->fillBuffer();
    page_size_t read = this->drainBuffer();
    ASSERT_EQ(written, this->base.length());
    ASSERT_EQ(read, this->base.length());
    ASSERT_EQ(0, this->sut.available());
}

TEST_F(CircularBufferTest, WriteFailsUntilPageIsRead) {
    this->fillBuffer();    
    char buf[50];   
    EXPECT_EQ(0, this->sut.write_soft(buf,10)) << "should not be able to write while buffer is full";
    
    // read the data - but not past the first page
    EXPECT_EQ(10, this->sut.read_soft(buf, 10)) << "Should be able to read 10 bytes";
    EXPECT_EQ(0, this->sut.write_soft(buf,10)) << "should not be able to write while buffer has no free pages";
    
    EXPECT_EQ(25, this->sut.read_soft(buf, 25)) << "Should be able to read 25 bytes";
    EXPECT_EQ(20, this->sut.write_soft(buf,40)) << "should be able to write 1 page";
    EXPECT_EQ(0, this->sut.write_soft(buf,10)) << "should not be able to write while buffer has no free pages";
    EXPECT_EQ(5, this->sut.read_soft(buf, 5)) << "Should be able to read 5 bytes";
    EXPECT_EQ(20, this->sut.write_soft(buf,50)) << "should be able to write 1 page (2)";    
}

/**
 * Fills/drains the buffer so that the read pointer is greater than the write pointer
 */
TEST_F(CircularBufferTest, WriteFailsUntilPageIsReadWrappedAround) {
    
    char buf[50];
    EXPECT_EQ(200, this->fillBuffer());
    EXPECT_EQ(150, this->drainBuffer(150));
    
    EXPECT_EQ(100, this->fillBuffer(100));
    EXPECT_EQ(150, this->sut.available());
    
    // now we have the read pointer at offset 150 and the write pointer at offset 100
    EXPECT_EQ(40, this->fillBuffer()) << "can only write up to page before read pointer";
    
    EXPECT_EQ(0, this->sut.write_soft(buf,10)) << "should not be able to write while buffer is full";
    EXPECT_EQ(190, this->sut.available());
    EXPECT_EQ(0, this->sut.free()) << "no space to write since this would erase the read page";
    
    // read the data - but not past the first page
    EXPECT_EQ(10, this->sut.read_soft(buf, 10)) << "Should be able to read 10 bytes";
    EXPECT_EQ(20, this->sut.write_soft(buf,40)) << "should be able to write one more page after read";
    
    EXPECT_EQ(30, this->sut.read_soft(buf, 30)) << "Should be able to read 30 bytes";
    EXPECT_EQ(20, this->sut.write_soft(buf,40)) << "should not be able to write while buffer has no free pages";
    EXPECT_EQ(0, this->sut.write_soft(buf,10)) << "should not be able to write while buffer is full ";    
}

TEST_F(CircularBufferTest, VerifyDataContentTheSame) {
    
    SequenceGenerator readGen(0);
    SequenceGenerator _write(0);
    PushBackGenerator writeGen(_write);
          
    this->fillBufferWith(writeGen);    
    this->verifyBufferFrom(readGen);        
}


TEST_F(CircularBufferTest, VerifyDataContentTheSameOnePage) {
    
    SequenceGenerator readGen(0);
    SequenceGenerator _write(0);
    PushBackGenerator writeGen(_write);
          
    this->fillBufferWith(writeGen, 10);    
    this->verifyBufferFrom(readGen, 10);
}

TEST_F(CircularBufferTest, VerifyDataContentTheSameTwoPages) {
    
    SequenceGenerator readGen(0);
    SequenceGenerator _write(0);
    PushBackGenerator writeGen(_write);
          
    this->fillBufferWith(writeGen, 40);    
    this->verifyBufferFrom(readGen, 40);
}

TEST_F(CircularBufferTest, VerifyDataContentTheSameWrappedAround) {
    
    SequenceGenerator readGen(0);
    SequenceGenerator _write(0);
    PushBackGenerator writeGen(_write);
              
    page_size_t written = this->fillBufferWith(writeGen);    
    written -= this->verifyBufferFrom(readGen, 150);
    written += this->fillBufferWith(writeGen);
    
    EXPECT_EQ(190, this->sut.available());
    EXPECT_EQ(190, written);    
    EXPECT_EQ(30, this->verifyBufferFrom(readGen, 30));
    
    EXPECT_EQ(40, this->fillBufferWith(writeGen, 60));

    EXPECT_EQ(50, this->verifyBufferFrom(readGen, 50));
}




