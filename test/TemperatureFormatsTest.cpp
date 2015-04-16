#include "catch.hpp"
#include "TemperatureFormats.h"
#include "TempControl.h"

ControlConstants TempControl::cc;

// Disable some warning about converting string constant
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

TEST_CASE("Test conversion between internal format and stirng", "[tempconversion]"){       
    
    // intToTemp is a macro to initialize temperatures in Celsius
    SECTION("Storing 0 as internal temperature gets C_OFFSET added") {
        CHECK(C_OFFSET == intToTemp(0));
    }
 
    SECTION("Conversion from int to temp and back") {
        CHECK(20 == tempToInt(intToTemp(20)));
        CHECK(-20 == tempToInt(intToTemp(-20)));
    }
  
    SECTION("Storing 0 as internal temperature difference is stored without offset"){
        CHECK(0 == intToTempDiff(0));
    }

    SECTION("Conversion from int to tempdiff and back") {
        CHECK(20 == tempDiffToInt(intToTempDiff(20)));
        CHECK(-20 == tempDiffToInt(intToTempDiff(-20)));
    }
     
    SECTION("Fixed point to string in Celsius"){
        char result[12];
        tempControl.cc.tempFormat = 'C';
        CHECK(0 == strcmp(" 0.000", fixedPointToString(result, intToTempDiff(0), 3, 12)));
        CHECK(0 == strcmp(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)));
        CHECK(0 == strcmp("-5.000", fixedPointToString(result, intToTempDiff(-5), 3, 12)));
        CHECK(0 == strcmp(" 0.250", fixedPointToString(result, intToTempDiff(1)/4, 3, 12)));
    }
    
    SECTION("Fixed point to string in Fahrenheit"){
        // test in Fahrenheit too, should be the same
        char result[12];
        tempControl.cc.tempFormat = 'F';
        CHECK(0 == strcmp(" 0.000", fixedPointToString(result, intToTempDiff(0), 3, 12)));
        CHECK(0 == strcmp(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)));
        CHECK(0 == strcmp("-5.000", fixedPointToString(result, intToTempDiff(-5), 3, 12)));
        CHECK(0 == strcmp(" 0.250", fixedPointToString(result, intToTempDiff(1)/4, 3, 12)));
    }
    
    SECTION("Fixed point to string with specified number of decimals"){       
        char result[12];
        CHECK(0 == strcmp(" 5.0", fixedPointToString(result, intToTempDiff(5), 1, 12)));
        CHECK(0 == strcmp(" 5.00", fixedPointToString(result, intToTempDiff(5), 2, 12)));
        CHECK(0 == strcmp(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)));
    }

    SECTION("Internal temperature to string in Celsius"){
        char result[9];
        tempControl.cc.tempFormat = 'C';
        CHECK(0 == strcmp(" 5.0", tempToString(result, intToTemp(5), 1, 9)));
        CHECK(0 == strcmp(" 20.0", tempToString(result, intToTemp(20), 1, 9)));
        CHECK(0 == strcmp("-20.0", tempToString(result, intToTemp(-20), 1, 9)));
        CHECK(0 == strcmp(" 20.00", tempToString(result, intToTemp(20), 2, 9)));
        CHECK(0 == strcmp("null", tempToString(result, INVALID_TEMP, 2, 9)));
    }
    
    SECTION("Internal temperature  to string in Fahrenheit"){        
        char result[9];
        tempControl.cc.tempFormat = 'F';   
        CHECK(0 == strcmp(" 41.0", tempToString(result, intToTemp(5), 1, 9)));
        CHECK(0 == strcmp(" 68.0", tempToString(result, intToTemp(20), 1, 9)));
        CHECK(0 == strcmp("-4.0", tempToString(result, intToTemp(-20), 1, 9)));
        CHECK(0 == strcmp(" 68.00", tempToString(result, intToTemp(20), 2, 9)));
        CHECK(0 == strcmp("null", tempToString(result, INVALID_TEMP, 2, 9)));       
    }

    SECTION("String in Celsius to internal temperature"){
        char result[9];
        tempControl.cc.tempFormat = 'C';
        CHECK(0 == strcmp(" 0.0", tempDiffToString(result, intToTempDiff(0), 1,  9)) );
        CHECK(0 == strcmp(" 5.0", tempDiffToString(result, intToTempDiff(5), 1,  9)));
        CHECK(0 == strcmp("-5.0", tempDiffToString(result, intToTempDiff(-5), 1,  9)));
    }
    
    SECTION("String in Fahrenheit to internal temperature"){
        char result[9];
        tempControl.cc.tempFormat = 'F';   
        CHECK(0 == strcmp(" 68.0", tempToString(result, intToTemp(20), 1, 9)));
        CHECK(intToTempDiff(9) ==  convertFromInternalTempDiff(intToTempDiff(5)));

        CHECK(0 == strcmp(" 0.0", tempDiffToString(result, intToTempDiff(0), 1,  9)));
        CHECK(0 == strcmp(" 9.0", tempDiffToString(result, intToTempDiff(5), 1,  9)));
        CHECK(0 == strcmp("-9.0", tempDiffToString(result, intToTempDiff(-5), 1,  9)));
    }


    SECTION("Convert string in Celsius to internal fixed point"){
        tempControl.cc.tempFormat = 'C';
        CHECK(intToTempDiff(20) ==  stringToFixedPoint("20"));
        CHECK(intToTempDiff(-5) ==  stringToFixedPoint("-5"));
        CHECK((intToTempDiff(1)/4) ==  stringToFixedPoint("0.25"));
    }
    
    SECTION("Convert string in Fahrenheit to internal fixed point"){
        tempControl.cc.tempFormat = 'F';
        CHECK(intToTempDiff(20) ==  stringToFixedPoint("20"));
        CHECK(intToTempDiff(-5) ==  stringToFixedPoint("-5"));
        CHECK((intToTempDiff(1)/4) ==  stringToFixedPoint("0.25"));
    }

    SECTION("Convert string in Celsius to internal temperature"){
        tempControl.cc.tempFormat = 'C';
        CHECK(intToTemp(20) ==  stringToTemp("20"));
        CHECK(intToTemp(-5) ==  stringToTemp("-5"));
        CHECK(intToTemp(20) ==  stringToTemp("20.0"));
        CHECK((intToTemp(20) + intToTempDiff(1)/10) ==  stringToTemp("20.1"));
    }
    SECTION("Convert string in Fahrenheit to internal temperature"){
        tempControl.cc.tempFormat = 'F';
        CHECK(intToTemp(20) ==  stringToTemp("68"));
        CHECK(intToTemp(-5) ==  stringToTemp("23"));
        CHECK(intToTemp(20) ==  stringToTemp("68.0"));
        CHECK((intToTemp(20) + intToTempDiff(1)/10) ==  stringToTemp("68.18"));
    }

    SECTION("Convert string in Celsius to internal temperature difference"){
        tempControl.cc.tempFormat = 'C';
        CHECK(intToTempDiff(20) ==  stringToTempDiff("20"));
        CHECK(intToTempDiff(-5) ==  stringToTempDiff("-5"));
        CHECK(intToTempDiff(20) ==  stringToTempDiff("20.0"));
        CHECK((intToTempDiff(20) + intToTempDiff(1)/10) ==  stringToTempDiff("20.1"));
    }
    
    SECTION("Convert string in Fahrenheit to internal temperature difference"){
        tempControl.cc.tempFormat = 'F';
        CHECK(intToTempDiff(20) ==  stringToTempDiff("36"));
        CHECK(intToTempDiff(-5) ==  stringToTempDiff("-9"));
        CHECK(intToTempDiff(20) ==  stringToTempDiff("36.0"));
        CHECK((intToTempDiff(20) + intToTempDiff(1)/10) ==  stringToTempDiff("36.18"));
    }

    SECTION("Test that constrainTemp16 limits the value to min and max temp"){
        tempControl.cc.tempFormat = 'C';
        CHECK(intToTemp(1) ==  constrainTemp16(intToTemp(1)));
        CHECK(temperature(MAX_TEMP) ==  constrainTemp16(MAX_TEMP+1));
        CHECK(temperature(MIN_TEMP) ==  constrainTemp16(MIN_TEMP-1));
    }
    
    SECTION("Test that constrainTemp function limits the value provided min and max temp"){
        CHECK(intToTemp(4) == constrainTemp(intToTemp(5), intToTemp(1), intToTemp(4)));
        CHECK(intToTemp(1) == constrainTemp(intToTemp(0), intToTemp(1), intToTemp(4)));
    }

    SECTION("Test multiplication functions for internal temperature format"){
        tempControl.cc.tempFormat = 'C';
        CHECK(intToTempDiff(0) == multiplyFactorTemperature(intToTempDiff(0), intToTemp(0)));
        CHECK(intToTempDiff(20) == multiplyFactorTemperature(intToTempDiff(2), intToTemp(10)));
        CHECK(intToTempDiff(20) == multiplyFactorTemperatureDiff(intToTempDiff(2), intToTempDiff(10)));

        CHECK(intToTempDiff(20) == multiplyFactorTemperatureLong(intToTempDiff(2), intToTemp(10)));
        CHECK(intToTempDiff(20) == multiplyFactorTemperatureDiffLong(intToTempDiff(2), intToTempDiff(10)));
    }
    SECTION("Test that overflowing multiplications are clipped"){
        CHECK(temperature(MAX_TEMP) == multiplyFactorTemperature(intToTempDiff(10), intToTemp(10)));
        CHECK(temperature(MIN_TEMP) == multiplyFactorTemperature(intToTempDiff(-10), intToTemp(10)));
    }

    SECTION("Test that fixed to tenths returns temperature in Celsius * 10 and rounds the result"){
        tempControl.cc.tempFormat = 'C';
        CHECK(200 == fixedToTenths(intToTemp(20)));
        CHECK(201 == fixedToTenths(intToTemp(20) + intToTempDiff(6)/100));
        CHECK(200 == fixedToTenths(intToTemp(20) + intToTempDiff(4)/100));
    }
    SECTION("Test that fixed to tenths returns temperature in Celsius * 10 converted to Fahrenheit"){
        tempControl.cc.tempFormat = 'F';
        CHECK(680 == fixedToTenths(intToTemp(20)));
        CHECK(681 == fixedToTenths(intToTemp(20) + intToTempDiff(6)/100));
        CHECK(681 == fixedToTenths(intToTemp(20) + intToTempDiff(4)/100));
    }
    
    SECTION("Test that thents to fixed returns value/10 converted to internal format"){
        tempControl.cc.tempFormat = 'C';
        CHECK(intToTemp(20) ==  tenthsToFixed(200));
        CHECK((intToTemp(20) + intToTempDiff(1)/10) ==  tenthsToFixed(201));

        tempControl.cc.tempFormat = 'F';
        CHECK(intToTemp(20) ==  tenthsToFixed(680));
        CHECK((intToTemp(20) + intToTempDiff(5)/10) ==  tenthsToFixed(689));
    }
}

// Restore original warnings configuration	
#pragma GCC diagnostic pop
