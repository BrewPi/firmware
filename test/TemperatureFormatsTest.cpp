#include "catch.hpp"
#include "TemperatureFormats.h"
#include "TempControl.h"
#include <cstring>

ControlConstants TempControl::cc;

// Disable some warning about converting string constant
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

TEST_CASE("Test conversion between internal format and stirng", "[tempconversion]") {

    // intToTemp is a macro to initialize temperatures in Celsius

    SECTION("Storing 0 as internal temperature gets C_OFFSET added") {
        REQUIRE(C_OFFSET == intToTemp(0));
    }

    SECTION("Conversion from int to temp and back") {
        REQUIRE(20 == tempToInt(intToTemp(20)));
        REQUIRE(-20 == tempToInt(intToTemp(-20)));
    }

    SECTION("Storing 0 as internal temperature difference is stored without offset") {
        REQUIRE(0 == intToTempDiff(0));
    }

    SECTION("Conversion from int to tempdiff and back") {
        REQUIRE(20 == tempDiffToInt(intToTempDiff(20)));
        REQUIRE(-20 == tempDiffToInt(intToTempDiff(-20)));
    }

    SECTION("Convert double in Celsius to internal temperature") {
        REQUIRE(intToTemp(21) == doubleToTemp(21.0));
        REQUIRE(intToTemp(50) == doubleToTemp(50.0));
        REQUIRE(intToTemp(-10) == doubleToTemp(-10.0));
        REQUIRE((intToTemp(0) - 1) == doubleToTemp(-0.00195));
        REQUIRE(intToTemp(0) == doubleToTemp(0.0));
        REQUIRE((intToTemp(0) + 1) == doubleToTemp(0.00195));
        REQUIRE(-32154 == doubleToTemp(-14.8)); // (-14.8-48)*512 = 32154
    }

    SECTION("Fixed point to string in Celsius") {
        tempControl.cc.tempFormat = 'C';
        char result[12];
        char tempString[12];
        for (long_temperature temp = MIN_TEMP; temp <= MAX_TEMP; temp++) {
            snprintf(tempString, 12, "% .1f", round(10*double(temp - C_OFFSET) / TEMP_FIXED_POINT_SCALE)/10);
            REQUIRE(tempToString(result, temp, 1, 12)); // returns true on success
            if (0 != strcmp(tempString, result)){
                printf("Test failed on temp: %s, %s, %d", tempString, result, temp);
            }
            REQUIRE(0 == strcmp(tempString, result));
        }
    }

    SECTION("Fixed point to string in Fahrenheit") {
        tempControl.cc.tempFormat = 'F';
        char result[12];
        char tempString[12];
        for (long_temperature temp = MIN_TEMP; temp <= MAX_TEMP; temp++) {
            snprintf(tempString, 12, "% .3f", round(1000*double(temp - F_OFFSET)*9/5 / TEMP_FIXED_POINT_SCALE)/1000);
            REQUIRE(tempToString(result, temp, 3, 12)); // returns true on success
            if (0 != strcmp(tempString, result)){
                if((atof(tempString) - atof(result)) < 0.0011 &&
                   (atof(tempString) - atof(result)) > -0.0011){
                    // small rounding errors are inevitable due to rounding in internal calculations
                }
                else{
                    printf("Test failed on temp: %s, %s, %d", tempString, result, temp);
                    REQUIRE(0 == strcmp(tempString, result));
                }
            }
        }
    }

    SECTION("Fixed point to string with specified number of decimals") {
        char result[12];
        REQUIRE(0 == strcmp(" 5.0", fixedPointToString(result, intToTempDiff(5), 1, 12)));
        REQUIRE(0 == strcmp(" 5.00", fixedPointToString(result, intToTempDiff(5), 2, 12)));
        REQUIRE(0 == strcmp(" 5.000", fixedPointToString(result, intToTempDiff(5), 3, 12)));
    }

    SECTION("Internal temperature to string in Celsius") {
        char result[9];
        tempControl.cc.tempFormat = 'C';
        REQUIRE(0 == strcmp(" 5.0", tempToString(result, intToTemp(5), 1, 9)));
        REQUIRE(0 == strcmp(" 20.0", tempToString(result, intToTemp(20), 1, 9)));
        REQUIRE(0 == strcmp("-15.0", tempToString(result, intToTemp(-15), 1, 9)));
        REQUIRE(0 == strcmp(" 20.00", tempToString(result, intToTemp(20), 2, 9)));
        REQUIRE(0 == strcmp("null", tempToString(result, INVALID_TEMP, 2, 9)));
    }

    SECTION("Internal temperature  to string in Fahrenheit") {
        char result[9];
        tempControl.cc.tempFormat = 'F';
        REQUIRE(0 == strcmp(" 41.0", tempToString(result, intToTemp(5), 1, 9)));
        REQUIRE(0 == strcmp(" 68.0", tempToString(result, intToTemp(20), 1, 9)));
        REQUIRE(0 == strcmp(" 14.0", tempToString(result, intToTemp(-10), 1, 9)));
        REQUIRE(0 == strcmp(" 68.00", tempToString(result, intToTemp(20), 2, 9)));
        REQUIRE(0 == strcmp(" 32.45", tempToString(result, (intToTemp(0) + intToTempDiff(1) / 4), 2, 9)));
        REQUIRE(0 == strcmp("null", tempToString(result, INVALID_TEMP, 2, 9)));
    }

    SECTION("String in Celsius to internal temperature") {
        char result[9];
        tempControl.cc.tempFormat = 'C';
        REQUIRE(0 == strcmp(" 0.0", tempDiffToString(result, intToTempDiff(0), 1, 9)));
        REQUIRE(0 == strcmp(" 5.0", tempDiffToString(result, intToTempDiff(5), 1, 9)));
        REQUIRE(0 == strcmp("-5.0", tempDiffToString(result, intToTempDiff(-5), 1, 9)));
    }

    SECTION("String in Fahrenheit to internal temperature") {
        char result[9];
        tempControl.cc.tempFormat = 'F';
        REQUIRE(0 == strcmp(" 68.0", tempToString(result, intToTemp(20), 1, 9)));
        REQUIRE(intToTempDiff(9) == convertFromInternalTempDiff(intToTempDiff(5)));

        REQUIRE(0 == strcmp(" 0.0", tempDiffToString(result, intToTempDiff(0), 1, 9)));
        REQUIRE(0 == strcmp(" 9.0", tempDiffToString(result, intToTempDiff(5), 1, 9)));
        REQUIRE(0 == strcmp("-9.0", tempDiffToString(result, intToTempDiff(-5), 1, 9)));
    }

    SECTION("Convert string in Celsius to internal fixed point") {
        tempControl.cc.tempFormat = 'C';
        temperature result;
        stringToFixedPoint(&result, "20");
        REQUIRE(intToTempDiff(20) == result);
        stringToFixedPoint(&result, "-5");
        REQUIRE(intToTempDiff(-5) == result);
        stringToFixedPoint(&result, "0.25");
        REQUIRE((intToTempDiff(1) / 4) == result);
    }

    SECTION("Convert string in Fahrenheit to internal fixed point") {
        tempControl.cc.tempFormat = 'F';
        temperature result;
        stringToFixedPoint(&result, "20");
        REQUIRE(intToTempDiff(20) == result);
        stringToFixedPoint(&result, "-5");
        REQUIRE(intToTempDiff(-5) == result);
        stringToFixedPoint(&result, "0.25");
        REQUIRE((intToTempDiff(1) / 4) == result);
    }

    SECTION("Convert string in Celsius to internal temperature") {
        tempControl.cc.tempFormat = 'C';
        char tempString[12];
        temperature result;
        for (double temp = -15; temp < 112; temp += 0.1) {
            snprintf(tempString, 12, "%.1f", temp); // "prints like -11.99"
            temperature internalTemp = doubleToTemp(temp);
            REQUIRE(stringToTemp(&result, tempString)); // returns true on success
            if (internalTemp != result) {
                printf("Test failed on temp: ");
                printf(tempString);
            }
            REQUIRE(internalTemp == result);
        }
    }

    SECTION("Convert string in Fahrenheit to internal temperature") {
        tempControl.cc.tempFormat = 'F';
        char tempString[12];
        temperature result;
        for (double tempF = 5; tempF < 233.6; tempF += 0.1) {
            double tempC = (tempF - 32)* 5 / 9;
            snprintf(tempString, 12, "%.1f", tempF); // String in F
            temperature internalTemp = doubleToTemp(tempC);
            REQUIRE(stringToTemp(&result, tempString)); // returns true on success
            temperature diff = internalTemp - result;
            REQUIRE(abs(diff) < 2); // max 1 bit difference
            // More precision unattainable due to internal precision/rounding
            // in fixedPointToString before converting from F to C.
        }
    }

    SECTION("Convert string in Celsius to internal temperature difference") {
        tempControl.cc.tempFormat = 'C';
        char tempDiffString[12];
        temperature result;
        for (double temp = -15; temp < 15; temp += 0.1) {
            snprintf(tempDiffString, 12, "%.1f", temp); // "prints like -11.99"
            temperature internalTempDiff = doubleToTempDiff(temp);
            REQUIRE(stringToTempDiff(&result, tempDiffString)); // returns true on success
            if (internalTempDiff != result) {
                printf("Test failed on temp: ");
                printf(tempDiffString);
            }
            REQUIRE(internalTempDiff == result);
        }
    }

    SECTION("Convert string in Fahrenheit to internal temperature difference") {
        tempControl.cc.tempFormat = 'F';
        char tempDiffString[12];
        temperature result;
        for (double temp = -15; temp < 15; temp += 0.1) {
            snprintf(tempDiffString, 12, "%.1f", temp); // "prints like -11.99"
            temperature internalTempDiff = doubleToTempDiff(temp * 5 / 9);
            REQUIRE(stringToTempDiff(&result, tempDiffString)); // returns true on success
            temperature diff = internalTempDiff - result;
            REQUIRE(abs(diff) < 2); // max 1 bit difference
            // More precision unattainable due to internal precision/rounding
            // in fixedPointToString before converting from F to C.
        }
    }

    SECTION("Test that constrainTemp16 limits the value to min and max temp") {
        tempControl.cc.tempFormat = 'C';
        REQUIRE(intToTemp(1) == constrainTemp16(intToTemp(1)));
        REQUIRE(temperature(MAX_TEMP) == constrainTemp16(MAX_TEMP + 1));
        REQUIRE(temperature(MIN_TEMP) == constrainTemp16(MIN_TEMP - 1));
    }

    SECTION("Test that constrainTemp function limits the value provided min and max temp") {
        REQUIRE(intToTemp(4) == constrainTemp(intToTemp(5), intToTemp(1), intToTemp(4)));
        REQUIRE(intToTemp(1) == constrainTemp(intToTemp(0), intToTemp(1), intToTemp(4)));
    }

    SECTION("Test multiplication functions for internal temperature format") {
        tempControl.cc.tempFormat = 'C';
        REQUIRE(intToTempDiff(0) == multiplyFactorTemperature(intToTempDiff(0), intToTemp(0)));
        REQUIRE(intToTempDiff(20) == multiplyFactorTemperature(intToTempDiff(2), intToTemp(10)));
        REQUIRE(intToTempDiff(20) == multiplyFactorTemperatureDiff(intToTempDiff(2), intToTempDiff(10)));

        REQUIRE(intToTempDiff(20) == multiplyFactorTemperatureLong(intToTempDiff(2), intToTemp(10)));
        REQUIRE(intToTempDiff(20) == multiplyFactorTemperatureDiffLong(intToTempDiff(2), intToTempDiff(10)));
    }

    SECTION("Test that overflowing multiplications are clipped") {
        REQUIRE(temperature(MAX_TEMP) == multiplyFactorTemperature(intToTempDiff(10), intToTemp(10)));
        REQUIRE(temperature(MIN_TEMP) == multiplyFactorTemperature(intToTempDiff(-10), intToTemp(10)));
    }

    SECTION("Test that fixed to tenths returns temperature in Celsius * 10 and rounds the result") {
        tempControl.cc.tempFormat = 'C';
        REQUIRE(200 == fixedToTenths(intToTemp(20)));
        REQUIRE(201 == fixedToTenths(intToTemp(20) + intToTempDiff(6) / 100));
        REQUIRE(200 == fixedToTenths(intToTemp(20) + intToTempDiff(4) / 100));
    }

    SECTION("Test that fixed to tenths returns temperature in Celsius * 10 converted to Fahrenheit") {
        tempControl.cc.tempFormat = 'F';
        REQUIRE(680 == fixedToTenths(intToTemp(20)));
        REQUIRE(681 == fixedToTenths(intToTemp(20) + intToTempDiff(6) / 100));
        REQUIRE(681 == fixedToTenths(intToTemp(20) + intToTempDiff(4) / 100));
    }

    SECTION("Test that tenths to fixed returns value/10 converted to internal format") {
        tempControl.cc.tempFormat = 'C';
        REQUIRE(intToTemp(20) == tenthsToFixed(200));
        REQUIRE((intToTemp(20) + intToTempDiff(1) / 10) == tenthsToFixed(201));

        tempControl.cc.tempFormat = 'F';
        REQUIRE(intToTemp(20) == tenthsToFixed(680));
        REQUIRE((intToTemp(20) + intToTempDiff(5) / 10) == tenthsToFixed(689));
    }

    SECTION("Invalid string returns false and result is not written") {
        tempControl.cc.tempFormat = 'C';

        temperature result = 12345;
        REQUIRE_FALSE(stringToFixedPoint(&result, "foo"));
        REQUIRE_FALSE(stringToTempDiff(&result, "foo"));
        REQUIRE_FALSE(stringToTemp(&result, "foo"));
        REQUIRE(result == 12345);

        REQUIRE_FALSE(stringToTempDiff(&result, "0.")); // A dot with no decimals is not allowed
        REQUIRE_FALSE(stringToTempDiff(&result, ".5")); // omitting the leading zero is not allowed           
        REQUIRE_FALSE(stringToTempDiff(&result, "")); // empty string is invalid
        REQUIRE_FALSE(stringToTempDiff(&result, "-")); // just minus is invalid
        REQUIRE_FALSE(stringToTempDiff(&result, " ")); // just spaces is invalid
        REQUIRE_FALSE(stringToTempDiff(&result, "   ")); // just spaces is invalid
        REQUIRE(stringToTempDiff(&result, " 2")); // leading space is allowed    
        REQUIRE(result == intToTempDiff(2));
        REQUIRE(stringToTempDiff(&result, " 4.5 ")); // trailing space is allowed
        REQUIRE(result == intToTempDiff(9) / 2);
        REQUIRE_FALSE(stringToTempDiff(&result, " 0.5a")); // extra non-space characters after the number are not allowed
        CHECK(stringToTempDiff(&result, " 0.5 a")); // a second word is ignored
    }

    SECTION("String null is converted to DISABLED_TEMP, but only for temp") {
        tempControl.cc.tempFormat = 'C';
        temperature result = 0;
        REQUIRE(stringToTemp(&result, "null"));
        REQUIRE(result == DISABLED_TEMP);
        REQUIRE_FALSE(stringToTempDiff(&result, "null"));
        REQUIRE_FALSE(stringToFixedPoint(&result, "null"));
    }

    SECTION("DISABLED_TEMP converts to null") {
        char result[9];
        tempControl.cc.tempFormat = 'C';

        REQUIRE(0 == strcmp("null", tempToString(result, DISABLED_TEMP, 2, 9)));
    }

    SECTION("Converting bool from string with error checking") {
        bool result = true;
        REQUIRE(stringToBool(&result, "0"));
        REQUIRE_FALSE(result);

        result = false;
        REQUIRE(stringToBool(&result, "1"));
        REQUIRE(result);

        result = true;
        REQUIRE(stringToBool(&result, "false"));
        REQUIRE_FALSE(result);

        result = false;
        REQUIRE(stringToBool(&result, "true"));
        REQUIRE(result);

        // everything else does not convert
        result = true;
        REQUIRE_FALSE(stringToBool(&result, "2"));
        REQUIRE_FALSE(stringToBool(&result, "-1"));
        REQUIRE_FALSE(stringToBool(&result, "foo"));
        REQUIRE_FALSE(stringToBool(&result, "None"));
        REQUIRE_FALSE(stringToBool(&result, " "));
        REQUIRE_FALSE(stringToBool(&result, ""));
        REQUIRE(result);
    }

    SECTION("Converting uint16 from string with error checking") {
        uint16_t result = 0;
        REQUIRE(stringToUint16(&result, "123"));
        REQUIRE(result == 123);

        REQUIRE(stringToUint16(&result, "123456789"));
        REQUIRE(result == UINT16_MAX); // result is clipped to max

        result = 345;
        REQUIRE_FALSE(stringToUint16(&result, "-123")); // negative not allowed
        REQUIRE(result == 345); // result is not written        
    }

    SECTION("Converting temperature higher than max temp results in max temp") {
        tempControl.cc.tempFormat = 'C';
        temperature result = 0;
        REQUIRE(stringToTempDiff(&result, "115"));
        REQUIRE(result == MAX_TEMP);
    }

    SECTION("Max temp converts correctly") {
        tempControl.cc.tempFormat = 'C';
        char result[12];
        REQUIRE(tempToString(result, MAX_TEMP, 1, 9));
        if (0 != strcmp(" 112.0", result)) {
            printf("Max temp converted to %s", result);
        }
        REQUIRE(0 == strcmp(" 112.0", result)); // assert
    }

    SECTION("Min temp converts correctly") {
        tempControl.cc.tempFormat = 'C';
        char result[12];
        REQUIRE(tempToString(result, MIN_TEMP, 1, 9));
        if (0 != strcmp("-16.0", result)) {
            printf("Min temp converted to %s", result);
        }
        REQUIRE(0 == strcmp("-16.0", result)); // assert
    }
}

// Restore original warnings configuration	
#pragma GCC diagnostic pop
