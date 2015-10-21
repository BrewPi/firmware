/*

	Platform selector header.

	Copyright (c) Jerry Evans, 2012-2014
	All rights reserved.

	Visit www.github.com/g40/esj

	The MIT License (MIT)

	Copyright (c) 2012-2014 Jerry Evans

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

*/


#ifndef PLATFORM_SELECTOR_H
#define PLATFORM_SELECTOR_H

#include <math.h>
#include <cmath>
#include <limits>
#include <float.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#if defined(_MSC_VER)

#include "windows\convert.h"
#include "windows\json_ex.h"

//
typedef int64_t int_size;

//-------------------------------------------------------------------------
// compatibility
namespace std
{

	//-------------------------------------------------------------------------
	//
	inline int isnan(double x)
	{
		return _isnan(x);
	}
	inline int isinf(double x)
	{
		return _finite(x) == 0;
	}

// apologies for the macros once more ...?
#define _IS_NAN(arg) std::isnan(arg)
#define _IS_INF(arg) std::isinf(arg)
#define __PLATFORM_THROW(arg) throw (arg)
	
}

#elif defined(__MACH__)

#include <stdlib.h>

#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>  // for mach_absolute_time() and friends
#include "osx/convert.h"
#include "osx/json_ex.h"

//
typedef int64_t int_size;

// apologies for the macros once more ...?
#define _IS_NAN(arg) isnan(arg)
#define _IS_INF(arg) isinf(arg)
#define __PLATFORM_THROW(arg) throw (arg)

// g++
#elif defined(__GNUG__)

#include "osx/convert.h"
#include "osx/json_ex.h"

// apologies for the macros once more ...?
#define _IS_NAN(arg) std::isnan(arg)
#define _IS_INF(arg) std::isinf(arg)
#define __PLATFORM_THROW(arg) throw (arg)
//
typedef int64_t int_size;

#elif defined(__arm__)

#include "arm/json_ex.h"
#include "arm/convert.h"

// apologies for the macros once more ...?
#define _IS_NAN(arg) isnan(arg)
#define _IS_INF(arg) isinf(arg)
#define __PLATFORM_THROW(arg) throw (arg)
//
typedef int32_t int_size;

#else

#error No support for current platform

#endif

#if defined(__MACH__) || defined(__arm__)

#include <stdarg.h>

#endif


#endif	// platform_selector

