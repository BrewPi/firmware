#include "../inc/testinfo.h"

TestInfo testInfo;

void
handleReset(bool)
{
    ++testInfo.rebootCount;
};
