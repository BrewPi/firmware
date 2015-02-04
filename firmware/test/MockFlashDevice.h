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

#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "flashee-eeprom.h"

using namespace Flashee;

class MockFlashDevice : public FlashDevice {
public:        

    /**
     * @return The size of each page in this flash device.
     */
    MOCK_CONST_METHOD0(pageSize, page_size_t());
    
    /**
     * @return The number of pages in this flash device.
     */
    MOCK_CONST_METHOD0(pageCount, page_count_t());
    
    MOCK_METHOD1(erasePage, bool (flash_addr_t address));
        
    /**
     * Writes directly to the flash. Depending upon the state of the flash, the
     * write may provide the data required or it may not. 
     * @param data
     * @param address
     * @param length
     * @return 
     */
    MOCK_METHOD3(writePage, bool(const void* data, flash_addr_t address, page_size_t length));
    
    
    MOCK_CONST_METHOD3(readPage, bool(void* data, flash_addr_t address, page_size_t length));
    
    MOCK_METHOD3(writeErasePage, bool(const void* data, flash_addr_t address, page_size_t length));
        
    MOCK_METHOD5(copyPage, bool(flash_addr_t address, TransferHandler handler, void* data, uint8_t* buf, page_size_t bufSize));
    
};


class MockFlashDeviceTest : public ::testing::Test {
public:    

    MockFlashDevice mock;

    MockFlashDeviceTest() {}
    
    void mockPageSizeCount(page_size_t size, page_count_t count) {
        EXPECT_CALL(mock, pageSize()).WillRepeatedly(::testing::Return(size));
        EXPECT_CALL(mock, pageCount()).WillRepeatedly(::testing::Return(count));
    }
};
