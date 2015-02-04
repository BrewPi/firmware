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

using namespace Flashee;

TEST(FakeFlashDeviceTest, CorrectSize) {
    FakeFlashDevice mock(100, 202);
    ASSERT_EQ(20200, mock.length());
}

TEST(FakeFlashDeviceTest, CorrectSize2) {
    FakeFlashDevice mock(6, 2+20*8);
    ASSERT_EQ((20ul*8+2)*6, mock.length());
}

TEST(FakeFlashDeviceTest, EraseAll) {
    FakeFlashDevice fake(40, 50);
    fake.eraseAll();
    uint8_t buf[50];
    for (int i=0; i<fake.pageCount(); i++) {
        ASSERT_TRUE(fake.readPage(buf, fake.pageAddress(i), sizeof(buf)));
        for (int x=0; x<sizeof(buf); x++) {
            ASSERT_EQ(255, buf[x]) << "page offset " << x << " in page " << i;
        }
    }
}

// These tests are for non-virtual methods on the FlashDevice interface
TEST(FakeFlashDeviceTest, PageAddress) {
    FakeFlashDevice fake(23, 10);
    ASSERT_EQ(30, fake.pageAddress(3));
}

TEST(FakeFlashDeviceTest, PageAddressOutOfBounds) {
    FakeFlashDevice fake(23, 10);
    ASSERT_EQ(300, fake.pageAddress(30));
}

TEST(FakeFlashDeviceTest, AddressPageOnPageBoundary) {
    FakeFlashDevice fake(23, 10);
    ASSERT_EQ(3, fake.addressPage(30));
}

TEST(FakeFlashDeviceTest, AddressPageMiddleOfPage) {
    FakeFlashDevice fake(23, 10);
    ASSERT_EQ(3, fake.addressPage(35));
}

TEST(FakeFlashDeviceTest, AddressPageEndOfPage) {
    FakeFlashDevice fake(23, 10);
    ASSERT_EQ(3, fake.addressPage(39));
}

TEST(FakeFlashDeviceTest, PageSpanValidWhenConfigured)
{
    FakeFlashDevice fake(40, 10, true);
    ASSERT_TRUE(fake.isValidRegion(38,48));
}

TEST(FakeFlashDeviceTest, PageSpanNotValidByDefault)
{
    FakeFlashDevice fake(40, 10);
    ASSERT_FALSE(fake.isValidRegion(38,48));
}