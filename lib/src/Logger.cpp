#include "Logger.h"
#include <cstdint>
#include <string>

// for some reason using to_string in a header file causes a compile error, but it works in a cpp file
// would prefer to use a template, but can't

std::string&
operator<<(std::string& lh, const int32_t& in)
{
    lh += std::to_string(in);
    return lh;
}

std::string&
operator<<(std::string& lh, const std::string& in)
{
    lh += in;
    return lh;
}

std::string&
operator<<(std::string& lh, const char* in)
{
    lh += in;
    return lh;
}
