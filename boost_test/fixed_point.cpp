/*******************************************************************************/
/*                                                                             */
/*  Copyright (c) 2007-2009: Peter Schregle,                                   */
/*  All rights reserved.                                                       */
/*                                                                             */
/*  This file is part of the Fixed Point Math Library.                         */
/*                                                                             */
/*  Redistribution of the Fixed Point Math Library and use in source and       */
/*  binary forms, with or without modification, are permitted provided that    */
/*  the following conditions are met:                                          */
/*  1. Redistributions of source code must retain the above copyright notice,  */
/*     this list of conditions and the following disclaimer.                   */
/*  2. Redistributions in binary form must reproduce the above copyright       */
/*     notice, this list of conditions and the following disclaimer in the     */
/*     documentation and/or other materials provided with the distribution.    */
/*  3. Neither the name of Peter Schregle nor the names of other contributors  */
/*     may be used to endorse or promote products derived from this software   */
/*     without specific prior written permission.                              */
/*                                                                             */
/*  THIS SOFTWARE IS PROVIDED BY PETER SCHREGLE AND CONTRIBUTORS 'AS IS' AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE      */
/*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE */
/*  ARE DISCLAIMED. IN NO EVENT SHALL PETER SCHREGLE OR CONTRIBUTORS BE LIABLE */
/*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS    */
/*  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)      */
/*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,        */
/*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN   */
/*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE            */
/*  POSSIBILITY OF SUCH DAMAGE.                                                */
/*                                                                             */
/*******************************************************************************/


#include <boost/test/unit_test.hpp>

#include "fixed_point.h"

BOOST_AUTO_TEST_SUITE( fixed_point_suite )

BOOST_AUTO_TEST_CASE( construction )
{
	fpml::fixed_point<int, 16> fp0(0);
	BOOST_CHECK( (int)fp0 == 0 );

	fpml::fixed_point<int, 16> fp1((signed char)-128);
	BOOST_CHECK( (signed char)fp1 == -128 );

	fpml::fixed_point<int, 16> fp2((unsigned char)127);
	BOOST_CHECK( (unsigned char)fp2 == 127 );

	fpml::fixed_point<int, 16> fp3((signed short)-32768);
	BOOST_CHECK( (signed short)fp3 == -32768 );

	fpml::fixed_point<int, 16> fp4((unsigned short)32767);
	BOOST_CHECK( (unsigned short)fp4 == 32767 );

	fpml::fixed_point<int, 16> fp5((signed int)-32768);
	BOOST_CHECK( (signed int)fp5 == -32768 );

	fpml::fixed_point<int, 16> fp6((unsigned int)32767);
	BOOST_CHECK( (unsigned int)fp6 == 32767 );

	fpml::fixed_point<int, 16> fp7((float)-1.5);
	BOOST_CHECK( (float)fp7 == -1.5 );

	fpml::fixed_point<int, 16> fp8((float)1.5);
	BOOST_CHECK( (float)fp8 == 1.5 );

	fpml::fixed_point<int, 16> fp9((double)-1.5);
	BOOST_CHECK( (double)fp9 == -1.5 );

	fpml::fixed_point<int, 16> fp10((double)1.5);
	BOOST_CHECK( (double)fp10 == 1.5 );

	fpml::fixed_point<int, 16> fp11((long double)-1.5);
	BOOST_CHECK( (long double)fp11 == -1.5 );

	fpml::fixed_point<int, 16> fp12((long double)1.5);
	BOOST_CHECK( (long double)fp12 == 1.5 );
}

BOOST_AUTO_TEST_CASE( copy_construction )
{
	fpml::fixed_point<int, 16> fp0(0);
	fpml::fixed_point<int, 16> fp0c(fp0);
	BOOST_CHECK( (int)fp0c == 0 );

	fpml::fixed_point<int, 16> fp1((signed char)-128);
	fpml::fixed_point<int, 16> fp1c(fp1);
	BOOST_CHECK( (signed char)fp1c == -128 );

	fpml::fixed_point<int, 16> fp2((unsigned char)127);
	fpml::fixed_point<int, 16> fp2c(fp2);
	BOOST_CHECK( (unsigned char)fp2c == 127 );

	fpml::fixed_point<int, 16> fp3((signed short)-32768);
	fpml::fixed_point<int, 16> fp3c(fp3);
	BOOST_CHECK( (signed short)fp3c == -32768 );

	fpml::fixed_point<int, 16> fp4((unsigned short)32767);
	fpml::fixed_point<int, 16> fp4c(fp4);
	BOOST_CHECK( (unsigned short)fp4c == 32767 );

	fpml::fixed_point<int, 16> fp5((signed int)-32768);
	fpml::fixed_point<int, 16> fp5c(fp5);
	BOOST_CHECK( (signed int)fp5c == -32768 );

	fpml::fixed_point<int, 16> fp6((unsigned int)32767);
	fpml::fixed_point<int, 16> fp6c(fp6);
	BOOST_CHECK( (unsigned int)fp6c == 32767 );

	fpml::fixed_point<int, 16> fp7((float)-1.5);
	fpml::fixed_point<int, 16> fp7c(fp7);
	BOOST_CHECK( (float)fp7c == -1.5 );

	fpml::fixed_point<int, 16> fp8((float)1.5);
	fpml::fixed_point<int, 16> fp8c(fp8);
	BOOST_CHECK( (float)fp8c == 1.5 );

	fpml::fixed_point<int, 16> fp9((double)-1.5);
	fpml::fixed_point<int, 16> fp9c(fp9);
	BOOST_CHECK( (double)fp9c == -1.5 );

	fpml::fixed_point<int, 16> fp10((double)1.5);
	fpml::fixed_point<int, 16> fp10c(fp10);
	BOOST_CHECK( (double)fp10c == 1.5 );

	fpml::fixed_point<int, 16> fp11((long double)-1.5);
	fpml::fixed_point<int, 16> fp11c(fp11);
	BOOST_CHECK( (long double)fp11c == -1.5 );

	fpml::fixed_point<int, 16> fp12((long double)1.5);
	fpml::fixed_point<int, 16> fp12c(fp12);
	BOOST_CHECK( (long double)fp12c == 1.5 );
}

BOOST_AUTO_TEST_CASE( copy_assignment )
{
	fpml::fixed_point<int, 16> fp0(0);
	fpml::fixed_point<int, 16> fp0c;
	fp0c = fp0;
	BOOST_CHECK( (int)fp0c == 0 );

	fpml::fixed_point<int, 16> fp1((signed char)-128);
	fpml::fixed_point<int, 16> fp1c;
	fp1c = fp1;
	BOOST_CHECK( (signed char)fp1c == -128 );

	fpml::fixed_point<int, 16> fp2((unsigned char)127);
	fpml::fixed_point<int, 16> fp2c;
	fp2c = fp2;
	BOOST_CHECK( (unsigned char)fp2c == 127 );

	fpml::fixed_point<int, 16> fp3((signed short)-32768);
	fpml::fixed_point<int, 16> fp3c;
	fp3c = fp3;
	BOOST_CHECK( (signed short)fp3c == -32768 );

	fpml::fixed_point<int, 16> fp4((unsigned short)32767);
	fpml::fixed_point<int, 16> fp4c;
	fp4c = fp4;
	BOOST_CHECK( (unsigned short)fp4c == 32767 );

	fpml::fixed_point<int, 16> fp5((signed int)-32768);
	fpml::fixed_point<int, 16> fp5c;
	fp5c = fp5;
	BOOST_CHECK( (signed int)fp5c == -32768 );

	fpml::fixed_point<int, 16> fp6((unsigned int)32767);
	fpml::fixed_point<int, 16> fp6c;
	fp6c = fp6;
	BOOST_CHECK( (unsigned int)fp6c == 32767 );

	fpml::fixed_point<int, 16> fp7((float)-1.5);
	fpml::fixed_point<int, 16> fp7c;
	fp7c = fp7;
	BOOST_CHECK( (float)fp7c == -1.5 );

	fpml::fixed_point<int, 16> fp8((float)1.5);
	fpml::fixed_point<int, 16> fp8c;
	fp8c = fp8;
	BOOST_CHECK( (float)fp8c == 1.5 );

	fpml::fixed_point<int, 16> fp9((double)-1.5);
	fpml::fixed_point<int, 16> fp9c;
	fp9c = fp9;
	BOOST_CHECK( (double)fp9c == -1.5 );

	fpml::fixed_point<int, 16> fp10((double)1.5);
	fpml::fixed_point<int, 16> fp10c;
	fp10c = fp10;
	BOOST_CHECK( (double)fp10c == 1.5 );

	fpml::fixed_point<int, 16> fp11((long double)-1.5);
	fpml::fixed_point<int, 16> fp11c;
	fp11c = fp11;
	BOOST_CHECK( (long double)fp11c == -1.5 );

	fpml::fixed_point<int, 16> fp12((long double)1.5);
	fpml::fixed_point<int, 16> fp12c;
	fp12c = fp12;
	BOOST_CHECK( (long double)fp12c == 1.5 );
}

BOOST_AUTO_TEST_CASE( comparison )
{
	fpml::fixed_point<int, 16> fp0(0);
	fpml::fixed_point<int, 16> fp1((signed char)-128);
	fpml::fixed_point<int, 16> fp2((unsigned char)127);
	fpml::fixed_point<int, 16> fp3((signed short)-32768);
	fpml::fixed_point<int, 16> fp4((unsigned short)32767);
	fpml::fixed_point<int, 16> fp5((signed int)-32768);
	fpml::fixed_point<int, 16> fp6((unsigned int)32767);
	fpml::fixed_point<int, 16> fp7((float)-1.5);
	fpml::fixed_point<int, 16> fp8((float)1.5);
	fpml::fixed_point<int, 16> fp9((double)-1.5);
	fpml::fixed_point<int, 16> fp10((double)1.5);
	fpml::fixed_point<int, 16> fp11((long double)-1.5);
	fpml::fixed_point<int, 16> fp12((long double)1.5);

	bool smaller = fp1 < fp2;
	bool bigger = fp2 > fp1;
	bool notsmaller = !(fp4 < fp3);
	bool notbigger = !(fp3 > fp4);

	BOOST_CHECK( smaller );
	BOOST_CHECK( bigger );
	BOOST_CHECK( notsmaller );
	BOOST_CHECK( notbigger );
}

BOOST_AUTO_TEST_CASE( equality )
{
	fpml::fixed_point<int, 16> fp0(0);
	fpml::fixed_point<int, 16> fp1((signed char)-128);
	fpml::fixed_point<int, 16> fp2((unsigned char)127);
	fpml::fixed_point<int, 16> fp3((signed short)-32768);
	fpml::fixed_point<int, 16> fp4((unsigned short)32767);
	fpml::fixed_point<int, 16> fp5((signed int)-32768);
	fpml::fixed_point<int, 16> fp6((unsigned int)32767);
	fpml::fixed_point<int, 16> fp7((float)-1.5);
	fpml::fixed_point<int, 16> fp8((float)1.5);
	fpml::fixed_point<int, 16> fp9((double)-1.5);
	fpml::fixed_point<int, 16> fp10((double)1.5);
	fpml::fixed_point<int, 16> fp11((long double)-1.5);
	fpml::fixed_point<int, 16> fp12((long double)1.5);

	bool same = fp3 == fp5 && fp4 == fp6 && fp7 == fp9 && fp9 == fp11 &&
		fp8 == fp10 && fp10 == fp12;
	bool notsame = fp1 != fp2 && fp3 != fp4 && fp5 != fp6 && fp7 != fp8 &&
		fp9 != fp10 && fp11 != fp12;

	BOOST_CHECK( same );
	BOOST_CHECK( notsame );
}

BOOST_AUTO_TEST_CASE( unary_minus )
{
	fpml::fixed_point<int, 16> fp1((signed char)-127);
	fpml::fixed_point<int, 16> fp2((unsigned char)127);
	fpml::fixed_point<int, 16> fp3((signed short)-32767);
	fpml::fixed_point<int, 16> fp4((unsigned short)32767);
	fpml::fixed_point<int, 16> fp5((signed int)-32767);
	fpml::fixed_point<int, 16> fp6((unsigned int)32767);
	fpml::fixed_point<int, 16> fp7((float)-1.5);
	fpml::fixed_point<int, 16> fp8((float)1.5);
	fpml::fixed_point<int, 16> fp9((double)-1.5);
	fpml::fixed_point<int, 16> fp10((double)1.5);
	fpml::fixed_point<int, 16> fp11((long double)-1.5);
	fpml::fixed_point<int, 16> fp12((long double)1.5);

	BOOST_CHECK( fp1 == -fp2 );
	BOOST_CHECK( -fp1 == fp2 );
	BOOST_CHECK( fp3 == -fp4 );
	BOOST_CHECK( -fp3 == fp4 );
	BOOST_CHECK( fp5 == -fp6 );
	BOOST_CHECK( -fp5 == fp6 );
	BOOST_CHECK( fp7 == -fp8 );
	BOOST_CHECK( -fp7 == fp8 );
	BOOST_CHECK( fp9 == -fp10 );
	BOOST_CHECK( -fp9 == fp10 );
	BOOST_CHECK( fp11 == -fp12 );
	BOOST_CHECK( -fp11 == fp12 );
}
/*
BOOST_AUTO_TEST_CASE( prefix_increment )
{
	double a=(std::numeric_limits<fpml::fixed_point<int, 16> >::min)(); 
	fpml::fixed_point<int, 16> b = a;

	while (a < (double)(std::numeric_limits<fpml::fixed_point<int, 16> >::max)())
	{
		++a;
		BOOST_CHECK( ++b == (fpml::fixed_point<int, 16>)(a) );
	}
}

BOOST_AUTO_TEST_CASE( prefix_decrement )
{
	double a=(std::numeric_limits<fpml::fixed_point<int, 16> >::max)(); 
	fpml::fixed_point<int, 16> b = a;

	while (a > (double)(std::numeric_limits<fpml::fixed_point<int, 16> >::min)()+1)
	{
		--a;
		BOOST_CHECK( --b == (fpml::fixed_point<int, 16>)(a) );
	}
}

BOOST_AUTO_TEST_CASE( postfix_increment )
{
	double a=(std::numeric_limits<fpml::fixed_point<int, 16> >::min)(); 
	fpml::fixed_point<int, 16> b = a;

	while (a < (double)(std::numeric_limits<fpml::fixed_point<int, 16> >::max)())
	{
		BOOST_CHECK( b++ == (fpml::fixed_point<int, 16>)(a) );
		a++;
	}
}

BOOST_AUTO_TEST_CASE( postfix_decrement )
{
	double a=(std::numeric_limits<fpml::fixed_point<int, 16> >::max)(); 
	fpml::fixed_point<int, 16> b = a;

	while (a > (double)(std::numeric_limits<fpml::fixed_point<int, 16> >::min)()+1)
	{
		BOOST_CHECK( b-- == (fpml::fixed_point<int, 16>)(a) );
		a--;
	}
}

BOOST_AUTO_TEST_CASE( add )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx = x;
			fpml::fixed_point<int, 16> yy = y;

			BOOST_CHECK( (fpml::fixed_point<int, 16>)(x+y) == xx+yy );
		}
	}
}

BOOST_AUTO_TEST_CASE( subtract )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx = x;
			fpml::fixed_point<int, 16> yy = y;

			BOOST_CHECK( (fpml::fixed_point<int, 16>)(x-y) == xx-yy );
		}
	}
}

BOOST_AUTO_TEST_CASE( add_assign )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx(x);
			xx += fpml::fixed_point<int, 16>(y);

			BOOST_CHECK( (fpml::fixed_point<int, 16>)(x+y) == xx );
		}
	}
}

BOOST_AUTO_TEST_CASE( subtract_assign )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx(x);
			xx -= fpml::fixed_point<int, 16>(y);

			BOOST_CHECK( (fpml::fixed_point<int, 16>)(x-y) == xx );
		}
	}
}

BOOST_AUTO_TEST_CASE( multiply )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx = x;
			fpml::fixed_point<int, 16> yy = y;

			BOOST_CHECK( (fpml::fixed_point<int, 16>)(x*y) == xx*yy );
		}
	}
}

BOOST_AUTO_TEST_CASE( divide )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx = x;
			fpml::fixed_point<int, 16> yy = y;

			if (y != 0)
				BOOST_CHECK( (fpml::fixed_point<int, 16>)(x/y) == xx/yy );
		}
	}
}

BOOST_AUTO_TEST_CASE( multiply_assign )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx(x);
			xx *= fpml::fixed_point<int, 16>(y);

			BOOST_CHECK( (fpml::fixed_point<int, 16>)(x*y) == xx );
		}
	}
}

BOOST_AUTO_TEST_CASE( divide_assign )
{
	double a=(std::numeric_limits<fpml::fixed_point<short, 8> >::min)(); 
	double b=(std::numeric_limits<fpml::fixed_point<short, 8> >::max)(); 

	for (double x=a; x<b; ++x)
	{
		for (double y=a; y<b; ++y)
		{
			fpml::fixed_point<int, 16> xx(x);
			if (y != 0)
			{
				xx /= fpml::fixed_point<int, 16>(y);

				BOOST_CHECK( (fpml::fixed_point<int, 16>)(x/y) == xx );
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( absolute )
{
	double a=(std::numeric_limits<fpml::fixed_point<int, 16> >::min)(); 
	a += 1;
	while (a<=(double)(std::numeric_limits<fpml::fixed_point<int, 16> >::max)()) 
	{
		fpml::fixed_point<int, 16> b = fabs((fpml::fixed_point<int, 16>)a);

		BOOST_CHECK( (double)b == fabs(a) );
		a += .5;
	}
}

BOOST_AUTO_TEST_CASE( square_root )
{
	for (double a=0; 
		a<=(double)(std::numeric_limits<fpml::fixed_point<int, 16> >::max)(); 
		a += .5)
	{
		fpml::fixed_point<int, 16> b = sqrt((fpml::fixed_point<int, 16>)a);

		fpml::fixed_point<int, 16> bb = (double)b;
		fpml::fixed_point<int, 16> sqrta = sqrt(a);

		BOOST_CHECK( (fpml::fixed_point<int, 16>)(double)b == (fpml::fixed_point<int, 16>)sqrt(a) );
	}
}



using namespace fpml;
BOOST_AUTO_TEST_CASE( scenario1 )
{
	fixed_point<int, 16> a = 256.;
	fixed_point<int, 16> b = sqrt(a);
	fixed_point<int, 16> c = 16.;
	BOOST_CHECK_EQUAL(b, c);
}


BOOST_AUTO_TEST_CASE( sceanario2 )
{
#define double fpml::fixed_point<int, 16>

	double a = 256.;
	double b = sqrt(a);
	double c = 16.;
	BOOST_CHECK_EQUAL(b, c);

#undef double
}

*/
BOOST_AUTO_TEST_SUITE_END()

