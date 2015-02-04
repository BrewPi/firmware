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

#include "MockFlashDevice.h"


using ::testing::InSequence;
using ::testing::Return;
using ::testing::_;
    
class PageSpanFlashDeviceTest : public MockFlashDeviceTest {
public:    

    PageSpanFlashDevice span;    

    PageSpanFlashDeviceTest() : span(mock) {}
    
};

// these are simple delegate methods. Here we just validate that the arguments
// and return value are correctly propagated.
TEST_F(PageSpanFlashDeviceTest, PageSize) {    
    EXPECT_CALL(mock, pageSize()).WillRepeatedly(Return(10));
    ASSERT_EQ(span.pageSize(), 10);
}

TEST_F(PageSpanFlashDeviceTest, PageCount) {
    EXPECT_CALL(mock, pageCount()).WillRepeatedly(Return(20));
    ASSERT_EQ(span.pageCount(), 20);
}

TEST_F(PageSpanFlashDeviceTest, ErasePage) {
    EXPECT_CALL(mock, pageSize()).WillRepeatedly(Return(10));
    EXPECT_CALL(mock, pageCount()).WillRepeatedly(Return(200));
    EXPECT_CALL(mock, erasePage(50)).WillOnce(Return(true));
    EXPECT_CALL(mock, erasePage(55)).WillOnce(Return(false));
    ASSERT_TRUE(span.erasePage(50));    
    ASSERT_FALSE(span.erasePage(55));
}

TEST_F(PageSpanFlashDeviceTest, CopyPage) {
    uint8_t buf[5];
    EXPECT_CALL(mock, copyPage(50, (TransferHandler)60, (void*)70, buf, 3)).WillOnce(Return(true));    
    EXPECT_CALL(mock, copyPage(30, _, _, _, _)).WillOnce(Return(false));    
    ASSERT_TRUE(span.copyPage(50, (TransferHandler)60, (void*)70, buf, 3));    
    ASSERT_FALSE(span.copyPage(30, NULL, NULL, buf, 3));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteWithinPage) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    EXPECT_CALL(mock, writePage(buf, 5, 3)).WillOnce(Return(true));
    ASSERT_TRUE(span.writePage(buf, 5, 3));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteSinglePage) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    EXPECT_CALL(mock, writePage(buf, 10, 10)).WillOnce(Return(true));
    ASSERT_TRUE(span.writePage(buf, 10, 10));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteMultipleCompletePages) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    EXPECT_CALL(mock, writePage(buf, 10, 10)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+10, 20, 10)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+20, 30, 10)).WillOnce(Return(true));
    ASSERT_TRUE(span.writePage(buf, 10, 30));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteEndsOnPage) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    EXPECT_CALL(mock, writePage(buf, 4, 6)).WillOnce(Return(true));
    ASSERT_TRUE(span.writePage(buf, 4, 6));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteCrossesPage) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    InSequence s;
    EXPECT_CALL(mock, writePage(buf, 5, 5)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+5, 10, 7)).WillOnce(Return(true));
    
    ASSERT_TRUE(span.writePage(buf, 5, 12));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteCrossesEndsOnPage) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    EXPECT_CALL(mock, writePage(buf, 5, 5)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+5, 10, 10)).WillOnce(Return(true));
    ASSERT_TRUE(span.writePage(buf, 5, 15));
}


TEST_F(PageSpanFlashDeviceTest, ChunkedWriteCrosses2Pages) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    InSequence s;
    EXPECT_CALL(mock, writePage(buf, 9, 1)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+1, 10, 10)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+11, 20, 1)).WillOnce(Return(true));
    
    ASSERT_TRUE(span.writePage(buf, 9, 12));
}

TEST_F(PageSpanFlashDeviceTest, ChunkedWriteCrosses2Pages_WriteFail) {
    this->mockPageSizeCount(10, 20);
    uint8_t buf[1];
    InSequence s;
    EXPECT_CALL(mock, writePage(buf, 9, 1)).WillOnce(Return(true));
    EXPECT_CALL(mock, writePage(buf+1, 10, 10)).WillOnce(Return(false));    
    ASSERT_FALSE(span.writePage(buf, 9, 12));
}
