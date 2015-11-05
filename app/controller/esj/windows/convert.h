/*

	MBCS/UTF8 to UTF16 (Windows wchar_t) helpers

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

#ifndef CONVERT_H
#define CONVERT_H

#include <windows.h>
#include <string>

namespace Chordia
{

	//-----------------------------------------------------------------------------
	// convert wide to ASCII
	inline std::string convert(const wchar_t* parg,int lcid = CP_UTF8)
	{
		std::string ret;
		size_t length = 0;
		if (parg)
		{
			length = wcslen(parg);
		}
		if (length)
		{
			// wide character string to multibyte character string 
#if (_MSC_VER >= 1400)	
			int chars = WideCharToMultiByte(lcid,0,parg,length,0,0,0,0);
			if (chars > 0)
			{
				ret.assign(chars,0);
				WideCharToMultiByte(lcid,0,parg,length,&ret[0],chars,0,0);
			}
#else		
			ret.assign(length,0);
			wcstombs(&ret[0],parg,length);
#endif		
		}
		return ret;
	}

	//-----------------------------------------------------------------------------
	// convert wide to ASCII
	inline std::string convert(const std::wstring& warg,int lcid = CP_UTF8)
	{
		return convert(warg.c_str(),lcid);
	}

	//-----------------------------------------------------------------------------
	// char to wchar_t
	inline std::wstring convert(const char* parg,int lcid = CP_UTF8)
	{
		std::wstring ret;
		size_t length = 0;

		if (parg)
		{
			length = strlen(parg);
		}
		if (length)
		{
			// multibyte to widechar
			int chars = MultiByteToWideChar(lcid,0,parg,length,0,0);
			if (chars > 0)
			{
				ret.assign(chars,0);
				MultiByteToWideChar(lcid,0,parg,length,&ret[0],chars);
			}
		}
		return ret;
	}

	//-----------------------------------------------------------------------------
	// convert ASCII to wide
	inline std::wstring convert(const std::string& arg)
	{
		return convert(arg.c_str());
	}

}

#endif

