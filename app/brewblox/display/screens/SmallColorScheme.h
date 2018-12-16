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

#pragma once
#include "d4d.hpp"
#include <algorithm>

/**
 * The core colors for a simple widget. Most widgets use only these core color values
 * so we don't need to waste space storing the full D4D_COLOR_SCHEME which stores colors
 * that aren't applicable (screen, progress bar, guage etc...) 
 */
struct SmallColorScheme {
    D4D_COLOR bckg;        ///< The object background color in standard state
    D4D_COLOR bckgDis;     ///< The object background color in disabled state
    D4D_COLOR bckgFocus;   ///< The object background color in focused state
    D4D_COLOR bckgCapture; ///< The object background color in captured state
    D4D_COLOR fore;        ///< The object fore color in standard state
    D4D_COLOR foreDis;     ///< The object fore color in disabled state
    D4D_COLOR foreFocus;   ///< The object fore color in focused state
    D4D_COLOR foreCapture; ///< The object fore color in captured state
};

#define AS_D4D_COLOR_SCHEME(c) \
    ((D4D_CLR_SCHEME*)(((uint8_t*)c) - offsetof(D4D_CLR_SCHEME, bckg)))

static constexpr SmallColorScheme
makeSmallColorScheme(const uint8_t r, const uint8_t g, const uint8_t b)
{
    auto r_lighter = uint8_t(std::min(uint16_t(r) + 24, 255));
    auto g_lighter = uint8_t(std::min(uint16_t(g) + 24, 255));
    auto b_lighter = uint8_t(std::min(uint16_t(b) + 24, 255));

    SmallColorScheme scheme = {
        D4D_COLOR_RGB(r, g, b),                         ///< The object background color in standard state
        D4D_COLOR_RGB(24, 24, 24),                      ///< The object background color in disabled state
        D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in focused state
        D4D_COLOR_RGB(r_lighter, g_lighter, b_lighter), ///< The object background color in captured state
        D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in standard state
        D4D_COLOR_RGB(48, 48, 48),                      ///< The object fore color in disabled state
        D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in focused state
        D4D_COLOR_RGB(255, 255, 255),                   ///< The object fore color in captured state
    };
    return scheme;
}