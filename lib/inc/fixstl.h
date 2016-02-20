/**
 ******************************************************************************
 * @file    fixstl.h
 * @authors mat
 * @date    26 February 2015
 ******************************************************************************
  Copyright (c) 2015 Spark Labs, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#ifndef FIXSTL_H
#define	FIXSTL_H

// the Spark platform defines min/max as macros - these interfere with the STL and
// need to be undefined. see https://github.com/spark/firmware/pull/398

#ifndef ARDUINO
#undef min
#undef max
#include <algorithm>
#endif

#endif	/* FIXSTL_H */

