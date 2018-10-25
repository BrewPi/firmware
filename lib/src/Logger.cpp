#include "Logger.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <string>

std::string&
operator<<(std::string& lh, const int32_t& in)
{
    char temp[12];
    snprintf(temp, 12, "%" PRId32, in);
    lh += std::string(temp);
    return lh;
}

std::string&
operator<<(std::string& lh, const uint32_t& in)
{
    char temp[12];
    snprintf(temp, 12, "%" PRIu32, in);
    lh += std::string(temp);
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
