#pragma once

#include "BoxApi.h"
#include <string>


std::ostream& operator << (std::ostream& os, Profile const& value)
{
    os << "id=" << int(value.get_id());
    return os;
}

