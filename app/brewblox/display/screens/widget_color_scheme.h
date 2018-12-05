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
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The core colors for a simple widget. Most widgets use only these core color values
 * so we don't need to waste space storing the full D4D_COLOR_SCHEME which stores colors
 * that aren't applicable (screen, progress bar, guage etc...) 
 */
struct WIDGET_COLOR_SCHEME {
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

/**
 * Declare a label with the given colors. 
 * The WIDGET_COLOR_SCHEME is availalbe as name_color.
 */
#define D4D_DECLARE_COLOR_LABEL(name, text, x, y, cx, cy, font, type, bg, fg) \
    WIDGET_COLOR_SCHEME name##_color = {bg, bg, bg, bg, fg, fg, fg, fg};

#ifdef __cplusplus
}
#endif
