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
#include "MockFlashDevice.h"



class FlashDeviceRegionTest : public MockFlashDeviceTest
{
    public:
        
};

TEST_F(FlashDeviceRegionTest, CanCreateSubregion)
{
    this->mockPageSizeCount(10, 20);
    FlashDeviceRegion region(this->mock);
    FlashDeviceRegion* sub = region.createSubregion(100, 150);
    ASSERT_TRUE(sub!=NULL);
}

TEST_F(FlashDeviceRegionTest, SubregionOutOfRangeFails)
{
    this->mockPageSizeCount(10, 20);
    FlashDeviceRegion region(this->mock);
    FlashDeviceRegion* sub = region.createSubregion(100, 300);
    ASSERT_TRUE(sub==NULL);
}

TEST_F(FlashDeviceRegionTest, NestedSubregionOutOfRangeFails)
{
    this->mockPageSizeCount(10, 20);
    FlashDeviceRegion region(this->mock);
    FlashDeviceRegion* sub = region.createSubregion(50, 100);    
    ASSERT_TRUE(sub!=NULL);
    // this creates a region local size 0-50.
    FlashDeviceRegion* sub2 = sub->createSubregion(30, 60);
    ASSERT_TRUE(sub2==NULL);
}


TEST_F(FlashDeviceRegionTest, NestedSubregionInRangeOk)
{
    this->mockPageSizeCount(10, 20);
    FlashDeviceRegion region(this->mock);
    FlashDeviceRegion* sub = region.createSubregion(50, 100);    
    ASSERT_TRUE(sub!=NULL);
    // this creates a region local size 0-50.
    FlashDeviceRegion* sub2 = sub->createSubregion(30, 50);
    ASSERT_TRUE(sub2!=NULL);
}



