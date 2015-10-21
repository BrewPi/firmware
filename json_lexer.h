/*

	A really simple lexer for UTF-8 JSON, part of ESJ

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


#ifndef JSON_LEXER_H
#define JSON_LEXER_H

#include <string>
#include <stdint.h>

// this is to enable ESJ to play nice with an existing (and complex)
// set of header implementations. apologies for the noise
// that pulls in these headers
#ifdef _INTEGRATED

#include "../strbuild.h"

namespace Chordia
{
	typedef string_builder stringer;
}

class json_exception : public std::runtime_error
{
public:

	json_exception(const wchar_t* p) : std::runtime_error(Chordia::convert_w(p).c_str()) {}
	json_exception(const char* p) : std::runtime_error(p) {}
};

#define __PLATFORM_THROW(arg) throw (arg)

#else

//
#include "platform_selector.h"
#include "stringer.h"

#endif


namespace JSON
{

//-----------------------------------------------------------------------------
enum TokenType
{
	T_UNKNOWN,
	T_OBJ_BEGIN,
	T_OBJ_END,
	T_ARRAY_BEGIN,
	T_ARRAY_END,
	T_COMMA,
	T_COLON,
	T_STRING,
	T_NUMBER,
	T_BOOLEAN,
	T_NULL,
	T_EOF
};

//-----------------------------------------------------------------------------
//
struct Token
{
	// verbatim text
	std::string text;
	// guess ...
	TokenType type;
	//
	Token() : type(T_UNKNOWN) {}
};

//-----------------------------------------------------------------------------
// defines the interface for a TokenType source
class ISource
{
public:
	//
	virtual ~ISource() {}
	// get the next character
	virtual char Next() = 0;
	// peek the next character
	virtual char Peek() = 0;
	// if matched, advances, 
	virtual bool Match(const char*) = 0;
	//
	virtual void Pushback() = 0;
	//
	virtual int Line() const = 0;
	virtual int Col() const = 0;
};

//-----------------------------------------------------------------------------
// A string source
class StringSource : public ISource
{
	//
	const char* m_ps;
	const char* m_pc;
	const char* m_pe;
	// position info.
	int m_line;
	int m_col;

	void Track(char tch)
	{
		if (tch == '\n')
		{
			m_line++;
			m_col = 1;
		}
	}

public:

	// string source. N.B. we do not make a copy of the data
	StringSource(const char* json,size_t size) :
		m_ps(json), m_pc(json), m_pe(json + size), m_line(1), m_col(1)
	{
	}

	// for error reporting
	virtual int Line() const
	{
		return m_line;
	}

	// for error reporting
	virtual int Col() const
	{
		return m_col;
	}

	// get the next character
	virtual char Next()
	{
		char ret = '\0';
		if (m_pc < m_pe)
		{
			ret = *m_pc++;
			Track(ret);
		}
		return ret;
	}

	// peek the next character
	virtual char Peek()
	{
		char ret = '\0';
		if (m_pc < m_pe)
		{
			ret = *m_pc;
		}
		return ret;
	}

	// speculative look-ahead. advance if matched
	virtual bool Match(const char* arg)
	{
		bool matched = true;
		const char* pc = m_pc;
		while (matched && pc < m_pe && arg && *arg)
		{
			if (tolower(*arg) != tolower(*pc))
			{
				matched = false;
			}
			else
			{
				arg++;
				pc++;
			}
		}
		if (matched)
		{
			m_pc = pc;
		}
		return matched;
	}

	// pushback. should probably 
	virtual void Pushback()
	{
		if (m_pc > m_ps)
		{
			m_pc--;
		}
	}
};

//-----------------------------------------------------------------------------
// throw with a suitable error message if the condition is true
inline void throw_if(ISource* pSource,bool condition,std::string msg)
{
	if (condition == true)
	{
		Chordia::stringer sb;
		sb << "Error (" << (pSource ? pSource->Line() : 0) << ':' << (pSource ? pSource->Col() : 0) << ") " << msg;
		__PLATFORM_THROW(json_exception(sb.c_str()));
	}
}

//-----------------------------------------------------------------------------
// throw with a suitable error message if the condition is true
inline void throw_if(bool condition,Chordia::stringer& msg)
{
	if (condition == true)
	{
		__PLATFORM_THROW(json_exception(msg.c_str()));
	}
}

//-----------------------------------------------------------------------------
//
class Scanner
{
	// source of characters for scanner
	ISource* m_pSource;

	//-----------------------------------------------------------------------------
	//
	uint8_t fromHex(char c)
	{
		uint8_t digit = 0;
		if (c >= '0' && c <= '9') 
		{
			digit = c - '0';
		} 
		else if (c >= 'a' && c <= 'f') 
		{
			digit = c - 'a' + 10;
		} 
		else if (c >= 'A' && c <= 'F') 
		{
			digit = c - 'A' + 10;
		}
		return digit;
	}

	//-----------------------------------------------------------------------------
	// this handles \UXXXX conversions 
	Token readString(char tch)
	{
		Token ret;
		//
		while (ret.type == T_UNKNOWN)
		{
			tch = m_pSource->Next();
			//
			// terminator
			if (tch == '"')
			{
				ret.type = T_STRING;
			}
			// it's an escaped character
			else if (tch == '\\')
			{
				tch = m_pSource->Next();
				if (tch == '"')
				{
					ret.text += tch;
				}
				else if (tch == '\\')
				{
					ret.text += tch;
				}
				else if (tch == '/')
				{
					ret.text += tch;
				}
				else if (tch == 'b')
				{
					ret.text += '\b';
				}
				else if (tch == 'f')
				{
					ret.text += '\f';
				}
				else if (tch == 'n')
				{
					ret.text += '\n';
				}
				else if (tch == 'r')
				{
					ret.text += '\r';
				}
				else if (tch == 't')
				{
					ret.text += '\t';
				}
				else if (tch == 'u')
				{
					unsigned long codepoint = 0;
					// Expect 4 hex digits.
					for (size_t i = 0; i < 4 ; ++i) 
					{
						tch = m_pSource->Next();
						uint8_t digit = 0;
						throw_if(m_pSource,tch == '\0',"Unterminated string");
						throw_if(m_pSource,tch == '"',"Truncated unicode escape sequence");
						throw_if(m_pSource,isxdigit(tch) == false,"Badly formed unicode escape sequence");
						digit = fromHex(tch);
						codepoint *= 16;
						codepoint += digit;
					}

					if (codepoint < 0x80)
					{
						ret.text += (char)(codepoint);
					}
					else if (codepoint < 0x800)
					{
						ret.text += (char) (0xC0 | (codepoint >> 6));
						ret.text += (0x80 | (codepoint & 0x3F));
					}
					else
					{
						ret.text += (char)(0xE0 | (codepoint >> 12));
						ret.text += (0x80 | ((codepoint >> 6) & 0x3F));
						ret.text += (0x80 | (codepoint & 0x3F));
					}
				}
			}
			// oops! run out of road ...
			else if (tch == '\0')
			{
				throw_if(m_pSource,true,"Unterminated string");
			}
			else
			{
				ret.text += tch;
			}
		}
		//
		return ret;
	}

	//-----------------------------------------------------------------------------
	// validate a JSON number. Does not handle exponentiation
	Token readNumber(char tch)
	{
		Token ret;
		ret.type = T_NUMBER;
		//
		ret.text += tch;
		//
		tch = m_pSource->Next();
		while (isdigit(tch))
		{
			ret.text += tch;
			tch = m_pSource->Next();
		}
		// floating point value?
		if (tch == '.')
		{
			//
			ret.text += tch;
			// expecting fractional value
			if (isdigit(m_pSource->Peek()))
			{
				tch = m_pSource->Next();
				while (isdigit(tch))
				{
					ret.text += tch;
					tch = m_pSource->Next();
				}
				// push back the character on which we failed
				m_pSource->Pushback();
			}
			else
			{
				throw_if(m_pSource,true,"badly formed number");
			}
		}
		else
		{
			// push back the character on which we failed
			m_pSource->Pushback();
		}
		//
		return ret;
	}

	//-------------------------------------------------------------------------
	//
	bool issign(char tch)
	{
		return (tch == '+' || tch == '-');
	}

public:

	//-------------------------------------------------------------------------
	//
	Scanner(ISource& source) : m_pSource(&source)	{}

	//-------------------------------------------------------------------------
	// get the next TokenType
	Token Scan()
	{
		Token token;
		char tch = '\0';
		while (token.type == T_UNKNOWN)
		{
			tch = m_pSource->Next();
			if (tch == ' ' || tch == '\t' || tch == '\r' || tch == '\n')
			{
				continue;
			}
			tch = static_cast<char>(tolower(tch));
			// we are done...
			if (tch == '\0')
			{
				token.text = "EOF";
				token.type = T_EOF;
			}
			else if (tch == '{')
			{
				token.text = "{";
				token.type = T_OBJ_BEGIN;
			}
			else if (tch == '}')
			{
				token.text = "}";
				token.type = T_OBJ_END;
			}
			else if (tch == '[')
			{
				token.text = "[";
				token.type = T_ARRAY_BEGIN;
			}
			else if (tch == ']')
			{
				token.text = "]";
				token.type = T_ARRAY_END;
			}
			else if (tch == ',')
			{
				token.text = ",";
				token.type = T_COMMA;
			}
			else if (tch == ':')
			{
				token.text = ":";
				token.type = T_COLON;
			}
			else if (tch == '"')
			{
				token = readString(tch);
			}
			else if (tch == 't' && m_pSource->Match("rue"))
			{
				token.text = "true";
				token.type = T_BOOLEAN;
			}
			else if (tch == 'f' && m_pSource->Match("alse"))
			{
				token.text = "false";
				token.type = T_BOOLEAN;
			}
			else if (tch == 'n' && m_pSource->Match("ull"))
			{
				token.text = "null";
				token.type = T_NULL;
			}
			else if (issign(tch) || isdigit(tch))
			{
				token = readNumber(tch);
			}
			else
			{
				throw_if(0,true,"Unexpected character ...");
			}
		}
		//
		return token;
	}

	// for error reporting
	int Line() const
	{
		return m_pSource->Line();
	}

	int Col() const
	{
		return m_pSource->Line();
	}
};

}

#endif
