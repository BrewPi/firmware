/*

	Exceptions wrapper for GCC/CLang compatibility

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

#ifndef JSON_EX_H
#define JSON_EX_H

namespace JSON
{

class json_exception : public std::exception
{
	// JME WTF: this means allocating off heap during a throw()
	std::string m_error;
	
public:
	json_exception(const char* arg) :
		m_error(arg == 0 ? "<null arg>" : arg)
	{
	}
	
	// 
    virtual ~json_exception() throw()	{}

    // Returns a C-style character string describing 
	// the general cause of the current error.
	 virtual const char* what() const throw()
	 {
		return m_error.c_str();
	 }
};	// class

}	//namespace

#endif
