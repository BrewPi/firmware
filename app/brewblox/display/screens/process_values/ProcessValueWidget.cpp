/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
 *
 * This file is part of BrewPi.
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

#include "ProcessValueWidget.h"
#include "ProcessValue.h"
#include "Temperature.h"

void
ProcessValueWidget::setEnabled(bool enabled)
{
    D4D_EnableObject(pObj(), enabled);
}

void
to_chars(const temp_t& t, char* buf, uint8_t len, uint8_t decimals)
{
    auto digits = decimals + 2;
    auto rounder = temp_t(0.5);

    for (; decimals > 0; decimals--) {
        rounder = rounder / 10;
    }

    temp_t rounded;
    if (t >= temp_t(0)) {
        rounded = t + rounder;
        auto temporary = rounded;
        while (temporary >= temp_t(10)) {
            temporary = temporary / 10;
            ++digits;
        }
    } else {
        rounded = t - rounder;
        auto temporary = rounded;
        while (temporary <= temp_t(-10)) {
            temporary = temporary / 10;
            ++digits;
        }
        digits += 1; // for minus sign
    }

    std::memset(buf, 0, len);
    if (digits > len) {
        digits = len;
    }

    cnl::to_chars(buf, &buf[digits], rounded);
}

void
ProcessValueWidget::update()
{
    if (auto pv = pvLookup.const_lock()) {
        char buf[10];
        to_chars(pv->value(), buf, 10, 1);
        D4D_SetText(&value, buf);
        to_chars(pv->setting(), buf, 10, 1);
        D4D_SetText(&setting, buf);
        D4D_SetText(&name, "Long beer name");
        setEnabled(true);
        return;
    }
    setEnabled(false);
}
