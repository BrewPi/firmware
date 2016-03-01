/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */


#if 0

#include "gtest/gtest.h"
#include "value.h"

TEST(BasicValueTest, valueCanBeRead) {
    BasicValue value(123);
    EXPECT_EQ(123, value.read()) << "Value should be 123";
}

TEST(BasicValueTest, implementsReadable) {
    BasicValue value(123);
    Readable& r = value;
    EXPECT_EQ(123, r.read()) << "Value should be 123";
}

TEST(BasicValueTest, valueCanBeWritten) {
    BasicValue value(123);
    value.write(456);
    EXPECT_EQ(123, r.read()) << "Value should be 456";
}

TEST(BasicValueTest, implementsWritable) {
    BasicValue value(123);
    Writable& w = value;
    Readable& r = value;
    w.write(456);
    EXPECT_EQ(123, r.read()) << "Value should be 456";
}
#endif