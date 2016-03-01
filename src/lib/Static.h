/*
 * Copyright 2016 Matthew McGowan.
 *
 * This file is part of Controlbox.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONTROLBOX_STATIC
#define CONTROLBOX_STATIC 1
#endif

#if CONTROLBOX_STATIC
#define cb_static static
#define cb_static_decl(...) __VA_ARGS__
#define cb_nonstatic_decl(...)
#else
#define cb_static
#define cb_static_decl(...)
#define cb_nonstatic_decl(...) __VA_ARGS__
#endif
