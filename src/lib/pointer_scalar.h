/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * Controlbox is free software: you can redistribute it and/or modify
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
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef POINTER_SCALAR_H
#define	POINTER_SCALAR_H

#ifdef	__cplusplus
extern "C" {
#endif


/**
 * The scalar type that is the same size as a pointer.
 */
#ifdef ARDUINO
typedef uint16_t pointer_scalar;
#elif __x86_64__ || __ppc64__
typedef uint64_t pointer_scalar;
#else
typedef uint32_t pointer_scalar;
#endif

#define PLATFORM_BIG_ENDIAN 0

// todo - move to platform?

#ifdef	__cplusplus
}
#endif

#endif	/* POINTER_SCALAR_H */

