#include "gtest/gtest.h"
#include "TemperatureFormats.h"
#include "TempControl.h"

TEST(TemperatureFormatsTest, intToTemp){
    // intToTemp is a macro to initialize temperatures in Celsius
    
    ASSERT_EQ(C_OFFSET, intToTemp(0)); "Storing 0 as internal temperature gets C_OFFSET added";
    ASSERT_EQ(20, tempToInt(intToTemp(20))) << "Expect conversion from int to temp and back to do nothing";
    ASSERT_EQ(-20, tempToInt(intToTemp(-20))) << "Expect conversion from negative int to temp and back to do nothing";;    
}

TEST(TemperatureFormatsTest, intToTempDiff){
    // intToTemp is a macro to initialize temperatures in Celsius
    
    ASSERT_EQ(0, intToTempDiff(0)); "Storing 0 as internal temperature difference is stored without offset";
    ASSERT_EQ(20, tempDiffToInt(intToTempDiff(20))) << "Expect conversion from int to temp diff and back to do nothing";
    ASSERT_EQ(-20, tempDiffToInt(intToTempDiff(-20))) << "Expect conversion from negative int to temp diff and back to do nothing";;    
}

TEST(TemperatureFormatsTest, fixedPointToString){
    char result[12];
    
    tempControl.cc.tempFormat = 'C';
    ASSERT_STREQ(" 0.000", fixedPointToString(result, intToTempDiff(0), 3, 12)) << "Test conversion of a 0 fixed point to String";
    ASSERT_STREQ(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)) << "Test conversion of a 5 fixed point to String";
    ASSERT_STREQ("-5.000", fixedPointToString(result, intToTempDiff(-5), 3, 12)) << "Test conversion of a 5 fixed point to String";
    ASSERT_STREQ(" 0.250", fixedPointToString(result, intToTempDiff(1)/4, 3, 12)) << "Test conversion of a 0.25 fixed point to String";
       
    tempControl.cc.tempFormat = 'F';   // test in Fahrenheit too, should be the same
    ASSERT_STREQ(" 0.000", fixedPointToString(result, intToTempDiff(0), 3, 12)) << "Test conversion of a 0 fixed point to String";
    ASSERT_STREQ(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)) << "Test conversion of a 5 fixed point to String";
    ASSERT_STREQ("-5.000", fixedPointToString(result, intToTempDiff(-5), 3, 12)) << "Test conversion of a 5 fixed point to String";
    ASSERT_STREQ(" 0.250", fixedPointToString(result, intToTempDiff(1)/4, 3, 12)) << "Test conversion of a 0.25 fixed point to String";
    
    //test different number of decimals
    ASSERT_STREQ(" 5.0", fixedPointToString(result, intToTempDiff(5), 1, 12)) << "Test conversion of a 5 fixed point to String";
    ASSERT_STREQ(" 5.00", fixedPointToString(result, intToTempDiff(5), 2, 12)) << "Test conversion of a 5 fixed point to String";
    ASSERT_STREQ(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)) << "Test conversion of a 5 fixed point to String";
}

TEST(TemperatureFormatsTest, tempToString){
    char result[9];
    
    tempControl.cc.tempFormat = 'C';
    ASSERT_STREQ(" 5.0", tempToString(result, intToTemp(5), 1, 9)) << "Test conversion of 5C to a string in Celsius" ;
    ASSERT_STREQ(" 20.0", tempToString(result, intToTemp(20), 1, 9)) << "Test conversion of 20C to a string in Celsius";
    ASSERT_STREQ("-20.0", tempToString(result, intToTemp(-20), 1, 9)) << "Test conversion of -20C to a string in Celsius";    
    ASSERT_STREQ(" 20.00", tempToString(result, intToTemp(20), 2, 9)) << "Test conversion of -20C to a string in Celsius in 2 decimals";
    ASSERT_STREQ("null", tempToString(result, INVALID_TEMP, 2, 9)) << "Test printing of invalid temperature as null";
    
    tempControl.cc.tempFormat = 'F';   
    ASSERT_STREQ(" 41.0", tempToString(result, intToTemp(5), 1, 9)) << "Test conversion of 5C to a string in Fahrenheit" ;
    ASSERT_STREQ(" 68.0", tempToString(result, intToTemp(20), 1, 9)) << "Test conversion of 20C to a string in Fahrenheit";
    ASSERT_STREQ("-4.0", tempToString(result, intToTemp(-20), 1, 9)) << "Test conversion of -20C to a string in Fahrenheit";    
    ASSERT_STREQ(" 68.00", tempToString(result, intToTemp(20), 2, 9)) << "Test conversion of -20C to a string in Fahrenheit in 2 decimals";
    ASSERT_STREQ("null", tempToString(result, INVALID_TEMP, 2, 9)) << "Test printing of invalid temperature as null";
}

TEST(TemperatureFormatsTest, tempDiffToString){
    char result[9];
    
    tempControl.cc.tempFormat = 'C';
    ASSERT_STREQ(" 0.0", tempDiffToString(result, intToTempDiff(0), 1, 9)) << "Test conversion of a 0C difference to a string in Celsius";
    ASSERT_STREQ(" 5.0", tempDiffToString(result, intToTempDiff(5), 1, 9)) << "Test conversion of a 5C difference to a string in Celsius";
    ASSERT_STREQ("-5.0", tempDiffToString(result, intToTempDiff(-5), 1, 9)) << "Test conversion of a -5C difference to a string in Celsius";

    tempControl.cc.tempFormat = 'F';   
    ASSERT_STREQ(" 68.0", tempToString(result, intToTemp(20), 1, 9)) << "Test that temperature format is in Fahrenheit";    
    ASSERT_EQ(intToTempDiff(9), convertFromInternalTempDiff(intToTempDiff(5))) << "Test that a 5C temp diff is equal to a 9F temp diff";
    
    ASSERT_STREQ(" 0.0", tempDiffToString(result, intToTempDiff(0), 1, 9)) << "Test conversion of a 0C difference to a string in Fahrenheit";
    ASSERT_STREQ(" 9.0", tempDiffToString(result, intToTempDiff(5), 1, 9)) << "Test conversion of a 5C difference to a string in Fahrenheit";
    ASSERT_STREQ("-9.0", tempDiffToString(result, intToTempDiff(-5), 1, 9)) << "Test conversion of a -5C difference to a string in Fahrenheit";
}

TEST(TemperatureFormatsTest, stringToFixedPoint){   
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(intToTempDiff(20), stringToFixedPoint("20")) << "Test conversion of string \"20\" to fixed point format";
    ASSERT_EQ(intToTempDiff(-5), stringToFixedPoint("-5")) << "Test conversion of string \"-5\" to fixed point format";    
    ASSERT_EQ(intToTempDiff(1)/4, stringToFixedPoint("0.25")) << "Test conversion of string \"0.25\" to fixed point format";    
        
    tempControl.cc.tempFormat = 'F';
    ASSERT_EQ(intToTempDiff(20), stringToFixedPoint("20")) << "Test conversion of string \"20\" to fixed point format";
    ASSERT_EQ(intToTempDiff(-5), stringToFixedPoint("-5")) << "Test conversion of string \"-5\" to fixed point format";    
    ASSERT_EQ(intToTempDiff(1)/4, stringToFixedPoint("0.25")) << "Test conversion of string \"0.25\" to fixed point format";    
}

TEST(TemperatureFormatsTest, stringToTemp){   
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(intToTemp(20), stringToTemp("20")) << "Test conversion of string \"20\" in Celsius to a temperature";    
    ASSERT_EQ(intToTemp(-5), stringToTemp("-5")) << "Test conversion of string \"-5\" in Celsius to a temperature";    
    ASSERT_EQ(intToTemp(20), stringToTemp("20.0")) << "Test conversion of string \"20.0\" in Celsius to a temperature";    
    ASSERT_EQ(intToTemp(20) + intToTempDiff(1)/10, stringToTemp("20.1")) << "Test conversion of string \"20.1\" in Celsius to a temperature";    
    
    tempControl.cc.tempFormat = 'F';
    ASSERT_EQ(intToTemp(20), stringToTemp("68")) << "Test conversion of string \"68\" in Fahrenheit to a temperature";    
    ASSERT_EQ(intToTemp(-5), stringToTemp("23")) << "Test conversion of string \"-20.556\" in Fahrenheit to a temperature";
    ASSERT_EQ(intToTemp(20), stringToTemp("68.0")) << "Test conversion of string \"68.0\" in Fahrenheit to a temperature";    
    ASSERT_EQ(intToTemp(20) + intToTempDiff(1)/10, stringToTemp("68.18")) << "Test conversion of string \"68.18\" in Fahrenheit to a temperature";   
}

TEST(TemperatureFormatsTest, stringToTempDiff){   
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(intToTempDiff(20), stringToTempDiff("20")) << "Test conversion of string \"20\" in Celsius to a temperature difference";    
    ASSERT_EQ(intToTempDiff(-5), stringToTempDiff("-5")) << "Test conversion of string \"-5\" in Celsius to a temperature difference";    
    ASSERT_EQ(intToTempDiff(20), stringToTempDiff("20.0")) << "Test conversion of string \"20.0\" in Celsius to a temperature difference";    
    ASSERT_EQ(intToTempDiff(20) + intToTempDiff(1)/10, stringToTempDiff("20.1")) << "Test conversion of string \"20.1\" in Celsius to a temperature difference";    
    
    tempControl.cc.tempFormat = 'F';
    ASSERT_EQ(intToTempDiff(20), stringToTempDiff("36")) << "Test conversion of string \"68\" in Fahrenheit to a temperature difference";    
    ASSERT_EQ(intToTempDiff(-5), stringToTempDiff("-9")) << "Test conversion of string \"-20.556\" in Fahrenheit to a temperature difference";
    ASSERT_EQ(intToTempDiff(20), stringToTempDiff("36.0")) << "Test conversion of string \"68.0\" in Fahrenheit to a temperature difference";    
    ASSERT_EQ(intToTempDiff(20) + intToTempDiff(1)/10, stringToTempDiff("36.18")) << "Test conversion of string \"68.18\" in Fahrenheit to a temperature difference";   
}

TEST(TemperatureFormatsTest, constrainTempFunctions){
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(intToTemp(1), constrainTemp16(intToTemp(1))) << "Test that constraining value within limits does not alter it";
    ASSERT_EQ(MAX_TEMP, constrainTemp16(MAX_TEMP+1)) << "Test that constraining value above MAX_TEMP limits it to MAX_TEMP";
    ASSERT_EQ(MIN_TEMP, constrainTemp16(MIN_TEMP-1)) << "Test that constraining value below MIN_TEMP limits it to MIN_TEMP";
    
    ASSERT_EQ(intToTemp(4), constrainTemp(intToTemp(5), intToTemp(1), intToTemp(4))) << "Test that constraining value higher than max is clipped to max";
    ASSERT_EQ(intToTemp(1), constrainTemp(intToTemp(0), intToTemp(1), intToTemp(4))) << "Test that constraining value lower than min is clipped to min";
}

TEST(TemperatureFormatsTest, multiplication){
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(intToTempDiff(0), multiplyFactorTemperature(intToTempDiff(0), intToTemp(0))) << "Test multiplying a 0 temperature and a 0 temperature";
    ASSERT_EQ(intToTempDiff(20), multiplyFactorTemperature(intToTempDiff(2), intToTemp(10))) << "Test multiplying a temperature and a temperature";
    ASSERT_EQ(intToTempDiff(20), multiplyFactorTemperatureDiff(intToTempDiff(2), intToTempDiff(10))) << "Test multiplying a factor and a temperature difference";
    
    ASSERT_EQ(intToTempDiff(20), multiplyFactorTemperatureLong(intToTempDiff(2), intToTemp(10))) << "Test multiplying a temperature and a long temperature";
    ASSERT_EQ(intToTempDiff(20), multiplyFactorTemperatureDiffLong(intToTempDiff(2), intToTempDiff(10))) << "Test multiplying a factor difference and a long temperature difference";
    
    ASSERT_EQ(MAX_TEMP, multiplyFactorTemperature(intToTempDiff(10), intToTemp(10))) << "Test multiplying with a result higher than TEMP_MAX to be clipped";
    ASSERT_EQ(MIN_TEMP, multiplyFactorTemperature(intToTempDiff(-10), intToTemp(10))) << "Test multiplying with a result lower than TEMP_MIN to be clipped";
}

TEST(TemperatureFormatsTest, fixedToTenths){
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(200, fixedToTenths(intToTemp(20))) << "Test converting internal temp 20C to tenths: 200";
    ASSERT_EQ(201, fixedToTenths(intToTemp(20) + intToTempDiff(6)/100)) << "Test converting internal temp 20.06C to tenths with rounding: 201";
    ASSERT_EQ(200, fixedToTenths(intToTemp(20) + intToTempDiff(4)/100)) << "Test converting internal temp 20.04C to tenths with rounding: 200";
    
    tempControl.cc.tempFormat = 'F';
    ASSERT_EQ(680, fixedToTenths(intToTemp(20))) << "Test converting internal temp 20C to tenths in F: 680";
    ASSERT_EQ(681, fixedToTenths(intToTemp(20) + intToTempDiff(6)/100)) << "Test converting internal temp 20.06C (68.108F) to tenths with rounding: 681";
    ASSERT_EQ(681, fixedToTenths(intToTemp(20) + intToTempDiff(4)/100)) << "Test converting internal temp 20.04C (68.072F) to tenths with rounding: 681";
}

TEST(TemperatureFormatsTest, tenthsToFixed){
    tempControl.cc.tempFormat = 'C';
    ASSERT_EQ(intToTemp(20), tenthsToFixed(200)) << "Test converting tenths 200 in Celsius to internal temp format";
    ASSERT_EQ(intToTemp(20) + intToTempDiff(1)/10, tenthsToFixed(201)) << "Test converting tenths 201 in Celsius to internal temp format";
        
    tempControl.cc.tempFormat = 'F';
    ASSERT_EQ(intToTemp(20), tenthsToFixed(680)) << "Test converting tenths 680 in Fahrenheit to internal temp format";
    ASSERT_EQ(intToTemp(20) + intToTempDiff(5)/10, tenthsToFixed(689)) << "Test converting tenths 689 in Fahrenheit to internal temp format";
}