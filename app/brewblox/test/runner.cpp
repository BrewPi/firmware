#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "testinfo.h"

TestInfo testInfo;

void
handleReset(bool)
{
    ++testInfo.rebootCount;
};
