/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FixedPoint.h"
#include <cstring>

void
to_chars_dec(const fp12_t& t, char* buf, uint8_t len, uint8_t decimals)
{
    auto digits = decimals + 2;
    auto rounder = fp12_t(0.5);

    for (; decimals > 0; decimals--) {
        rounder = rounder / 10;
    }

    fp12_t rounded;
    if (t >= fp12_t(0)) {
        rounded = t + rounder;
        auto temporary = rounded;
        while (temporary >= fp12_t(10)) {
            temporary = temporary / 10;
            ++digits;
        }
    } else {
        rounded = t - rounder;
        auto temporary = rounded;
        while (temporary <= fp12_t(-10)) {
            temporary = temporary / 10;
            ++digits;
        }
        digits += 1; // for minus sign
    }

    std::memset(buf, 0, len);
    if (digits > len) {
        digits = len;
    }

    using reduced_precision_t = safe_elastic_fixed_point<11, 10, int32_t>;
    cnl::to_chars(buf, &buf[digits], reduced_precision_t(rounded));
}