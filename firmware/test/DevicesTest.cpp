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

using namespace Flashee;

TEST(DevicesTest, CanCreateCircularBuffer) {
    CircularBuffer* buf = Devices::createCircularBuffer(0, 4096*10);
    ASSERT_TRUE(buf!=NULL);
    delete buf;
}

TEST(DevicesTest, CanCreateAddressEraseFull) {
    FlashDevice* dev = Devices::createAddressErase();
    ASSERT_TRUE(dev!=NULL);
    delete dev;
}

TEST(DevicesTest, CanCreateAddressEraseSegment) {
    FlashDevice* dev = Devices::createAddressErase(4096*20, 4096*100);
    ASSERT_TRUE(dev!=NULL);
    delete dev;
}

TEST(DevicesTest, CreateAddressEraseNonPageBoundaryFails) {
    FlashDevice* dev = Devices::createAddressErase(4096*20+50, 4096*100);
    EXPECT_TRUE(dev==NULL);
    delete dev;
}

TEST(DevicesTest, CanCreateMultiPageEraseFull) {
    FlashDevice* dev = Devices::createWearLevelErase();
    EXPECT_TRUE(dev!=NULL);
    delete dev;
}

TEST(DevicesTest, CanCreateWearLevelEraseSegment) {
    FlashDevice* dev = Devices::createWearLevelErase(4096*20, 4096*40);
    EXPECT_TRUE(dev!=NULL);
    delete dev;
}

TEST(DevicesTest, CreateWearLevelEraseSegmentNonPageAddressFails) {
    FlashDevice* dev = Devices::createWearLevelErase(4096*20+20, 4096*40);
    EXPECT_TRUE(dev==NULL);
    delete dev;
}

TEST(DevicesTest, CreateSinglePageEraseFull) {
    FlashDevice* dev = Devices::createSinglePageErase(0, Devices::userFlash().length());
    EXPECT_TRUE(dev!=NULL);
    delete dev;
}

TEST(DevicesTest, CreateSinglePageEraseSegment) {
    FlashDevice* dev = Devices::createSinglePageErase(20*4096, 100*4096);
    EXPECT_TRUE(dev!=NULL);
    delete dev;
}

TEST(DevicesTest, CreateSinglePageEraseNonPageAddressFails) {
    FlashDevice* dev = Devices::createSinglePageErase(20*4096+20, 100*4096);
    EXPECT_TRUE(dev==NULL);
    delete dev;
}

TEST(DeviceTest, CanCreateCircularBufferFull) {
    CircularBuffer* buf = Devices::createCircularBuffer(0, Devices::userFlash().length());
    ASSERT_TRUE(buf!=NULL);
    delete buf;
}

TEST(DeviceTest, CreateSinglePageCircularBufferFails) {
    CircularBuffer* buf = Devices::createCircularBuffer(0, Devices::userFlash().pageSize());
    ASSERT_TRUE(buf==NULL);
    delete buf;
}

TEST(DeviceTest, CanCreateTwoPageCircularBuffer) {
    page_size_t size = Devices::userFlash().pageSize();
    CircularBuffer* buf = Devices::createCircularBuffer(size*2, size*4);
    ASSERT_TRUE(buf!=NULL);
    delete buf;
}

TEST(DeviceTest, CreateTwoPageCircularBufferNonPageAddressFails) {
    page_size_t size = Devices::userFlash().pageSize();
    CircularBuffer* buf = Devices::createCircularBuffer(size*2+20, size*4+20);
    ASSERT_TRUE(buf==NULL);
    delete buf;
}