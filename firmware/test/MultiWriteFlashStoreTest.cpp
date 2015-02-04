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

TEST(MultiWriteSlotAccess, readSlot_uninitlaized) {
    uint8_t slot[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_EQ(0xFF, MultiWriteSlotAccess::readSlot(slot));
}

TEST(MultiWriteSlotAccess, readSlot_initialzied1) {
    uint8_t slot[] = { 0xFE, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_EQ(0xAB, MultiWriteSlotAccess::readSlot(slot));
}

TEST(MultiWriteSlotAccess, readSlot_initialzied7) {
    uint8_t slot[] = { 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81 };
    ASSERT_EQ(0x81, MultiWriteSlotAccess::readSlot(slot));
}

/*
 * In principle, the bitmap should never be 0. But in practice this can happen if
 * the scheme is created on top of previously initialized data. 
 */
TEST(MultiWriteSlotAccess, readSlot_invalid_bitmap) {
    uint8_t slot[] = { 0x00, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1C };
    ASSERT_EQ(0x0, MultiWriteSlotAccess::readSlot(slot));
}


#define ASSERT_ARRAYS_EQ(expected, actual) \
    ASSERT_THAT(std::vector<uint8_t>(expected, expected + sizeof(expected)), \
            ::testing::ElementsAreArray(actual))


TEST(MultiWriteSlotAccess, writeSlot_uninitlaized_non_destructive) {
    uint8_t slot[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_TRUE(MultiWriteSlotAccess::writeSlot(0xFF, slot));
    uint8_t expected[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_ARRAYS_EQ(expected, slot);
}

TEST(MultiWriteSlotAccess, writeSlot_uninitlaized_destructive) {
    uint8_t slot[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_TRUE(MultiWriteSlotAccess::writeSlot(0xAB, slot));
    uint8_t expected[] = { 0xFE, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_ARRAYS_EQ(expected, slot);
}

TEST(MultiWriteSlotAccess, writeSlot_1_non_destructive) {
    uint8_t slot[] = { 0xFE, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_TRUE(MultiWriteSlotAccess::writeSlot(0x8A, slot));
    uint8_t expected[] = { 0xFE, 0x8A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_ARRAYS_EQ(expected, slot);
}

TEST(MultiWriteSlotAccess, writeSlot_1_destructive) {
    uint8_t slot[] = { 0xFE, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_TRUE(MultiWriteSlotAccess::writeSlot(0xBF, slot));
    uint8_t expected[] = { 0xFC, 0xAB, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    ASSERT_ARRAYS_EQ(expected, slot);
}

TEST(MultiWriteSlotAccess, writeSlot_7_non_destructive) {
    uint8_t slot[] = { 0x80, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1C };
    ASSERT_TRUE(MultiWriteSlotAccess::writeSlot(0x08, slot));
    uint8_t expected[] = { 0x80, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x08 };
    ASSERT_ARRAYS_EQ(expected, slot);
}

TEST(MultiWriteSlotAccess, writeSlot_7_destructive) {
    uint8_t slot[] = { 0x80, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
    ASSERT_FALSE(MultiWriteSlotAccess::writeSlot(0x12, slot));
    uint8_t expected[] = { 0x80, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
    ASSERT_ARRAYS_EQ(expected, slot);
}

TEST(MultiWriteSlotAccess, writeSlot_7_destructive_inplace) {
    uint8_t slot[] = { 0x80, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1C };
    ASSERT_TRUE(MultiWriteSlotAccess::writeSlot(0x12, slot, true));
    uint8_t expected[] = { 0x80, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x10 };
    ASSERT_ARRAYS_EQ(expected, slot);
}

/*
 * In principle, the bitmap should never be 0. But in practice this can happen if
 * the scheme is created on top of previously initialized data. 
 */
TEST(MultiWriteSlotAccess, writeSlot_invalid_bitmap) {
    uint8_t slot[] = { 0x00, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1C };
    ASSERT_FALSE(MultiWriteSlotAccess::writeSlot(0x12, slot, false));
    uint8_t expected[] = { 0x00, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1C };
    ASSERT_ARRAYS_EQ(expected, slot);    
}

TEST(MultiWriteSlotAccess, findLastUsedIndex_bitmap_0) {
    ASSERT_EQ(MultiWriteSlotAccess::findLastUsedIndex(0), 8);    
}

TEST(MultiWriteSlotAccess, findLastUsedIndex_bitmap_0x80) {
    ASSERT_EQ(MultiWriteSlotAccess::findLastUsedIndex(0x80), 7);
}

TEST(MultiWriteSlotAccess, findLastUsedIndex_bitmap_0xFF) {
    ASSERT_EQ(MultiWriteSlotAccess::findLastUsedIndex(0xFF), 0);
}






#if 0
void AddOneCopyHandler(page_size_t pageOffset, void* data, uint8_t* buf, page_size_t bufSize) {
    EXPECT_EQ(NULL, data);
    EXPECT_LE(bufSize, 1);
    for (int i=0; i<bufSize; i++) {
        buf[i]++;
    }
}

TYPED_TEST_P(FlashDeviceTest, CanCopyPageVerbatim) {
    ASSERT_TRUE(this->flash->erasePage(0)) << "Unable to erase page 0";
    SequenceGenerator seq(0);
    this->fillPage(0, seq);    

    uint8_t buf[4];
    this->flash->copyPage(0, AddOneCopyHandler, NULL, buf, 4);
    
    SequenceGenerator seq2(1);
    this->assertPageMatches(0, seq2);
}
#endif


