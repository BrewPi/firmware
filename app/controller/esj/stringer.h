/*

	Compact type-safe string formatting and conversion

	Copyright (c) Jerry Evans, 2004-2014
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


#ifndef STRINGER_H
#define STRINGER_H

#include "platform_selector.h"

namespace Chordia
{
	//-----------------------------------------------------------------------------
	// narrow (UTF8) to wide (UTF16|UTF32)
	inline std::wstring n2w(const std::string& arg)
	{
		return convert(arg);
	}


	//-----------------------------------------------------------------------------
	// wide to narrow (UTF8)
	inline std::string w2n(const std::wstring& arg)
	{
		return convert(arg);
	}


	//-------------------------------------------------------------------------
	// strings to various numbers
	inline int toInt(const char* arg)
	{
		return (atoi(arg));
	}

	//-------------------------------------------------------------------------
	// strings to various numbers
	inline int toInt(const std::string& arg)
	{
		return toInt(arg.c_str());
	}

#ifndef ESJ_DISABLE_DOUBLE
	//-------------------------------------------------------------------------
	inline double toDouble(const char* arg)
	{
		return (atof(arg));
	}

	//-------------------------------------------------------------------------
	inline double toDouble(const std::string& arg)
	{
		return toDouble(arg.c_str());
	}
#endif

	//-------------------------------------------------------------------------
	// rather tricksy in place string reversal
	inline void reverse(std::string& ret,size_t offset = 0)
	{
		// 
		for (size_t i = offset, j = ret.size() - 1; i < j; i++, j--)
		{
			// swap without temporary
			ret[i] ^= ret[j];
			ret[j] ^= ret[i];
			ret[i] ^= ret[j];
		}
	}

	//-------------------------------------------------------------------------
	// integer to ASCII
	inline std::string toString(int64_t value, int base = 10)
	{
		std::string result;
		// this is a symmetric lookup table. zero is mid range
		static const char* digits = 
			"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

		// check that the base if valid
		if (base < 2 || base > 36)
		{
			return result;
		}
		int64_t tmp_value = 0;
		do
		{
			tmp_value = value;
			value /= base;
			result += digits [35 + (tmp_value - value * base)];
		}
		while (value);
		// Apply negative sign
		if (tmp_value < 0)
		{
			result += '-';
		}
		//
		reverse(result);
		//
		return result;
	}

	//-------------------------------------------------------------------------
	inline std::string toString(int8_t value, int base = 10)
	{
		return toString((int64_t)value,base);
	}

    
	inline std::string toString(int16_t value, int base = 10)
    {
        return toString((int64_t)value,base);
    }

	inline std::string toString(int32_t value, int base = 10)
    {
        return toString((int64_t)value,base);
    }

	//-------------------------------------------------------------------------
    inline std::string toString(uint8_t value, int base = 10)
    {
        return toString((int64_t)value,base);
    }


    inline std::string toString(uint16_t value, int base = 10)
    {
        return toString((int64_t)value,base);
    }
    
    inline std::string toString(uint32_t value, int base = 10)
    {
        return toString((int64_t)value,base);
    }
    
#ifndef ESJ_DISABLE_DOUBLE

#ifdef _MSC_VER
	//-------------------------------------------------------------------------
	// double to ASCII. 
	// This is pretty simplistic, not, for example, handling rounding
	inline std::string toString(double value,double precision = 0.0001)
	{
		std::string ret;
		// handle special cases
		if (std::isnan(value))
		{
			ret = "nan";
		}
		else if (std::isinf(value))
		{
			ret = "inf";
		}
		else if (value == 0.0)
		{
			ret = "0";
		}
		else
		{
			int digit = 0;
			int m1 = 0;
			bool neg = (value < 0);
			if (neg)
			{
				value = -value;
			}
			// calculate magnitude
			int mag = static_cast<int>(log10(value));
			bool useExp = (mag >= 14 || (neg && mag >= 9) || mag <= -9);
			if (neg)
			{
				ret += '-';
			}
			// set up for scientific notation
			if (useExp)
			{
				if (mag < 0)
				{
					mag -= 1;
				}
				value = value / pow(10.0, mag);
				m1 = mag;
				mag = 0;
			}
			if (mag < 1.0)
			{
				mag = 0;
			}
			// convert the number
			while (value > precision || mag >= 0)
			{
				double weight = pow(10.0, mag);
				if (weight > 0 && !std::isinf(weight))
				{
					digit = static_cast<int>(floor(value / weight));
					value -= (digit * weight);
					ret += static_cast<char>('0' + digit);
				}
				if (mag == 0 && value > 0)
				{
					ret += '.';
				}
				mag--;
			}
			if (useExp)
			{
				// convert the exponent
				ret += 'e';
				if (m1 > 0)
				{
					ret += '+';
				}
				else
				{
					ret += '-';
					m1 = -m1;
				}
				size_t offset = ret.size();
				while (m1 > 0)
				{
					ret += ('0' + m1 % 10);
					m1 /= 10;
				}
				// swap the exponent as it has been added in reverse
				reverse(ret,offset);
			}
		}
		return ret;
	}
#endif

	//-----------------------------------------------------------------------------
	// round up
	inline int64_t round_int( double r ) 
	{
		return (r > 0.0) ? static_cast<int64_t>(r + 0.5) : static_cast<int64_t>(r - 0.5); 
	}

	//-----------------------------------------------------------------------------
	// returns number of digits required for string representation of 'value'
	inline int countFractionalDigits(double value,double eps = 0.00001)
	{
		int digits = 0;
		double tmp = 0;
		while ((tmp = fabs(round_int(value)-value)) > eps)
		{
			value=value*10;
			digits++;
		}
		return digits;
	}

	//-----------------------------------------------------------------------------
	template <typename T=int_size>
	class DoubleConverter
	{
		public:
		//-----------------------------------------------------------------------------
		// As succinctly as possible, given a value like 12.345:
		// 1. convert the integral value => 12
		// 2. determine number of digits required to represent fraction (3)
		// 3. multiply value by 10^3 or 1000 => 12345.0
		// 4. multiply integral (12) by 1000 => 12000.0
		// 5. integer representation of fraction is now 12345-12000 or 345
		// 6. append string representation to result of 1.
		static std::string Convert(double value,double eps = 0.00001)
		{
			std::string ret;

			// limits of integral representation
			T imin = std::numeric_limits<T>::min();
			T imax = std::numeric_limits<T>::max();

			// handle special cases
			if (_IS_NAN(value))
			{
				ret = "nan";
			}
			else if (_IS_INF(value))
			{
				ret = "inf";
			}
			else if (value == 0.0)
			{
				ret = "0";
			}
			else
			{
				// 1. 
				int64_t ival = static_cast<int64_t>(value);
				// check that the floating point value can be represented by an integral value ...
				if (ival <= imin)
				{
					// this is decidedly non-standard ...
					ret = "-inf";
				}
				else if (ival >= imax)
				{
					// ditto but makes sense here I think ...
					ret = "+inf";
				}
				else
				{
					// 
					if (ival == 0)
					{
						ret += (value < 0.0 ? "-0" : "0");
					}
					else
					{
						ret = Chordia::toString(ival);
					}
					// work on the absolute value
					double avalue = fabs(value);
					// 2. how many digits required to represent fractional part?
					int digits = countFractionalDigits(avalue,eps);
					if (digits > 0)
					{
						// add decimal point
						ret += '.';
						// special case for less than 1.0
						if (avalue < 1.0)
						{
							int iv = 0;
							for (int digit = 0; digit < digits; digit++)
							{
								avalue *= 10;
								iv = static_cast<int>(avalue);
								ret += static_cast<char>('0' + iv);
								avalue -= static_cast<double>(iv);
							}
						}
						else
						{
							// 3. calculate multiplicand required to make value integral
							double d1 = pow(10.0,digits);
							// multiply the absolute value to scale it
							double d2 = avalue * d1;
							// 4. scale up integral value for subtraction
							d1 *= std::abs(ival);
							// 5. take integral value
							int64_t iv = static_cast<int64_t>(d2-d1);
							// 6. append conversion of 'integral' fraction
							// ack. need to ensure number is correctly zero padded
							ret += Chordia::toString(iv,digits);
						}
					}
				}
			}
			return ret;
		}
	};

#endif

	//-----------------------------------------------------------------------------
	// bytes to upper-case hex - skipping the overhead of std::string conversion
	inline std::string BinToHex(const void* pSrc,size_t bytes)
	{
		uint8_t val = 0;
		static char hexChars[] = ("0123456789ABCDEF");
		const uint8_t* data = reinterpret_cast<const uint8_t*>(pSrc);
		std::string dst;
		for (int index = (int)bytes-1; index >= 0; index--)
		{
			val = (data[index] >> 4);
			val &= 0x0F;
			dst += hexChars[val];
			val = (data[index] >> 0);
			val &= 0x0F;
			dst += hexChars[val];
		}
		return dst;
	}

	//-------------------------------------------------------------------------
	inline std::string escape(const std::string& arg)
	{
		char chr = 0;
		std::string ret;
		for (size_t s = 0; s < arg.size(); s++)
		{
			chr = arg[s];
			if (chr == '"' || chr == '\\' || chr == '/')
			{
				ret += '\\';
				ret += chr;
			}
			else if (chr == '\b')
			{
				ret += "\\b";
			}
			else if (chr == '\f')
			{
				ret += "\\f";
			}
			else if (chr == '\n')
			{
				ret += "\\n";
			}
			else if (chr == '\r')
			{
				ret += "\\r";
			}
			else if (chr == '\t')
			{
				ret += "\\t";
			}
			else
			{
				ret += chr;
			}
		}
		return ret;
	}

	//-------------------------------------------------------------------------
	// convert from UTF16|32 to JSON encoded UTF8
	inline std::string escape(const std::wstring& arg)
	{
		int32_t chr = 0;
		std::string ret;
		for (size_t s = 0; s < arg.size(); s++)
		{
			chr = arg[s];
			if (chr == '"' || chr == '\\' || chr == '/')
			{
				ret += '\\';
				ret += static_cast<char>(chr);
			}
			else if (chr == '\b')
			{
				ret += "\\b";
			}
			else if (chr == '\f')
			{
				ret += "\\f";
			}
			else if (chr == '\n')
			{
				ret += "\\n";
			}
			else if (chr == '\r')
			{
				ret += "\\r";
			}
			else if (chr == '\t')
			{
				ret += "\\t";
			}
			// 
			// codepoint is in Basic Multilingual Plane
			else if (chr >= 0x255 && chr <= 0xFFFF)
			{
				// represented as a 4 character hex string
				ret += "\\u";
				ret += Chordia::BinToHex(&chr,sizeof(uint16_t));
			}
			// not in BMP -> construct a UTF-16 surrogate pair
			else if (chr > 0xFFFF)
			{
				chr -= 0x10000;
				int32_t first = 0xD800 | ((chr & 0xffc00) >> 10);
				ret += "\\u";
				ret += Chordia::BinToHex(&first, sizeof(uint16_t));
				int32_t last = 0xDC00 | (chr & 0x003ff);
				ret += "\\u";
				ret += Chordia::BinToHex(&last, sizeof(uint16_t));
			}
			else
			{
				ret += static_cast<char>(chr);
			}
		}
		return ret;
	}

//-----------------------------------------------------------------------------
// Light-weight replacement for stringstream and friends. 
class stringer
{
	//
	std::string m_buffer;

public:

	//-------------------------------------------------------------------------
	stringer()	{}

	//-------------------------------------------------------------------------
	stringer(const stringer& arg) : m_buffer(arg.m_buffer) {}

	//-------------------------------------------------------------------------
	stringer(const std::string& arg) : m_buffer(arg) {}

	//-------------------------------------------------------------------------
	stringer(const char* arg) : m_buffer(arg) {}

	//-------------------------------------------------------------------------
	// for debug support
#if defined(ESJ_DEBUG)
	stringer(const char* file,int line)
	{
		(*this) << file << "(" << line << ") : ";
	}
#endif

	//-------------------------------------------------------------------------
	stringer& operator<<(char arg)
	{
		m_buffer += arg;
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(const char* arg)
	{
		if (arg)
		{
			m_buffer += arg;
		}
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(const std::string& arg)
	{
		m_buffer += arg;
		return (*this);
	}

	//-------------------------------------------------------------------------
	// JME audit: UTF16|32 => UTF8
	stringer& operator<<(const std::wstring& arg)
	{
		m_buffer += convert(arg);
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(bool arg)
	{
		m_buffer += (arg ? "true" : "false");
		return (*this);
	}

	//-------------------------------------------------------------------------
	stringer& operator<<(int8_t arg)
	{
		//
		m_buffer += toString(arg,10);
		return (*this);
	}

	//-------------------------------------------------------------------------
    stringer& operator<<(int16_t arg)
    {
        //
        m_buffer += toString(arg,10);
        return (*this);
    }

	//-------------------------------------------------------------------------
    stringer& operator<<(int32_t arg)
    {
        //
        m_buffer += toString(arg,10);
        return (*this);
    }

    stringer& operator<<(uint8_t arg)
    {
        //
        m_buffer += toString(arg,10);
        return (*this);
    }

    //-------------------------------------------------------------------------
    stringer& operator<<(uint16_t arg)
    {
        //
        m_buffer += toString(arg,10);
        return (*this);
    }
    
    //-------------------------------------------------------------------------
    stringer& operator<<(uint32_t arg)
    {
        //
        m_buffer += toString(arg,10);
        return (*this);
    }

    stringer& operator<<(const temp_t& arg)
    {
        //
        m_buffer += arg.toCstring();
        return (*this);
    }

    stringer& operator<<(const temp_precise_t& arg)
    {
        //
        m_buffer += arg.toCstring();
        return (*this);
    }

    stringer& operator<<(const temp_long_t& arg)
    {
        //
        m_buffer += arg.toCstring();
        return (*this);
    }

#ifndef ESJ_DISABLE_DOUBLE
	//-------------------------------------------------------------------------
	stringer& operator<<(double arg)
	{
		// attempt to preserve representation of the original value ...
		m_buffer += Chordia::DoubleConverter<>::Convert(arg);
		return (*this);
	}
#endif

	//-------------------------------------------------------------------------
	const char* c_str() const
	{
		return m_buffer.c_str();
	}

	//-------------------------------------------------------------------------
	bool empty() const
	{
		return m_buffer.empty();
	}

	//-------------------------------------------------------------------------
	void clear()
	{
		m_buffer.clear();
	}

	//-------------------------------------------------------------------------
	size_t size() const
	{
		return m_buffer.size();
	}
};  // class

//-----------------------------------------------------------------------------
#if defined(ESJ_DEBUG)
// debug support
static void Debug(const stringer& sb)
{
#if (defined(_WINDOWS_))
    OutputDebugStringA(sb.c_str());
#elif (defined(__MACH__))
    //NSLog(@"%s",(char*)sb.c_str());
    std::cout << sb.c_str();
#else

#endif
}

#define DBMSG(sb)	Debug(Chordia::stringer(__FILE__,__LINE__) << sb << "\r\n");
#else
#define DBMSG(sb)	
#endif

}		// namespace

#endif	// guard

