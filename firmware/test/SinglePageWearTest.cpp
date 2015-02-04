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
    
class SinglePageWearTest : public MockFlashDeviceTest {
public:    

    SinglePageWear wear;    

    SinglePageWearTest() : wear(mock) {}
    
};

// these are simple delegate methods. Here we just validate that the arguments
// and return value are correctly propagated.
TEST_F(SinglePageWearTest, PageCountIsOneLessThanBase) {    
    EXPECT_CALL(mock, pageCount()).WillRepeatedly(Return(10));
    ASSERT_EQ(wear.pageCount(), 9);
}

TEST_F(SinglePageWearTest, Length) {    
    this->mockPageSizeCount(10, 20);
    ASSERT_EQ(wear.length(), 10*19);
}



