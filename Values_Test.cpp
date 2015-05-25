
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