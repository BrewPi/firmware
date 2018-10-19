#define CATCH_CONFIG_MAIN

#include "Connections.h"
#include "testinfo.h"
#include <catch.hpp>

TestInfo testInfo;

void
handleReset(bool)
{
    ++testInfo.rebootCount;
};

namespace cbox {
void
connectionStarted(DataOut& out)
{
}
}