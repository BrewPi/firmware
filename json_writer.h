/*

	Extremely simple JSON serialization for C++
		
	www.github.com/g40/esj

	Copyright (c) Jerry Evans, 2012-2014
	All rights reserved.

	Trivial example:

	Given a class with std::wstring members a,b,c implement
	a function called serialize() as in:

	void serialize(JSON::json_adapter& adapter)
	{
		JSON::Class root(adapter,"classname");
		JSON_E(adapter,a);
		JSON_E(adapter,b);
		JSON_T(adapter,c);	// Note 'T' type
	}

	That's all that is required. The adapter can be a reader or writer, the
	system is symmetric.

	Given the constraints of JSON/Javascript semantics the only container supported 
	out of the box is std::vector<> and there is obviously no pointer support as the 
	additional JSON overhead required would not interop with any JS consumers/producers.

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


#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#ifndef JSON_ADAPTER_H
#include "json_adapter.h"
#endif

namespace JSON
{
	//-----------------------------------------------------------------------------
	// base class for facade over eventual destination, 
	// e.g. a socket instead of a string
	class ISink
	{
	public:
		virtual ISink& operator<<(const char*) = 0;
		virtual ISink& operator<<(const std::string&) = 0;
		virtual ISink& operator<<(const std::wstring&) = 0;
		virtual ISink& operator<<(const char&) = 0;
		virtual ISink& operator<<(const int&) = 0;
		virtual ISink& operator<<(const unsigned int&) = 0;
		virtual ISink& operator<<(const long&) = 0;
		virtual ISink& operator<<(const double&) = 0;
		virtual ISink& operator<<(const bool&) = 0;

	};

	//-----------------------------------------------------------------------------
	// Simply a facade over stringer, writes directly to a std::string.
	class StringSink : public ISink
	{
		//
		Chordia::stringer* _sink;

	public:
		//
		StringSink(Chordia::stringer* sink) : _sink(sink)	{}
		//
		virtual ISink& operator<<(const char* arg)			{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const std::string& arg)	{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const std::wstring& arg)	{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const char& arg)			{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const int& arg)			{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const long& arg)			{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const unsigned int& arg)	{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const double& arg)		{ (*_sink) << arg; return (*this); }
		virtual ISink& operator<<(const bool& arg)			{ (*_sink) << arg; return (*this); }
	};

	//-------------------------------------------------------------------------
	// for serializing (writing) JSON

	class Writer : public Adapter
	{
		//---------------------------------------------------------------------
		ISink* _sink;

		//
		static const char* Quote() 
		{
			return "\"";
		}

	public:

		//---------------------------------------------------------------------
		//
		Writer(ISink* sink) : _sink(sink) {}

		//---------------------------------------------------------------------
		//
		virtual bool storing() { return true; }

		//---------------------------------------------------------------------
		// write a key/value pair with optional continuation
		virtual void serialize(const std::string& key,std::string& value,bool more) 
		{
			(*_sink) << "\"" << key << Quote() << ':' << Quote() << Chordia::escape(value) << Quote() << (more ? "," : "");
		}


		virtual void serialize(const std::string& key,std::wstring& value,bool more) 
		{
			(*_sink) << Quote() << key << Quote() << ':' << Quote() << Chordia::escape(Chordia::w2n(value)) << Quote() << (more ? "," : "");
		}

		virtual void serialize(const std::string& key,int& value,bool more) 
		{
			(*_sink) << Quote() << key << Quote() << ':' << value << (more ? "," : "");
		}
		
		virtual void serialize(const std::string& key,unsigned int& value,bool more) 
		{
			(*_sink) << Quote() << key << Quote() << ':' << value << (more ? "," : "");
		}

		virtual void serialize(const std::string& key,long& value,bool more) 
		{
			(*_sink) << Quote() << key << Quote() << ':' << value << (more ? "," : "");
		}

		virtual void serialize(const std::string& key,unsigned char& value,bool more)
		{
			(*_sink) << Quote() << key << Quote() << ':' << value << (more ? "," : "");
		}

		virtual void serialize(const std::string& key,double& value,bool more) 
		{
			(*_sink) << Quote() << key << Quote() << ':' << value << (more ? "," : "");
		}

		virtual void serialize(const std::string& key,bool& value,bool more) 
		{
			// literal true of false
			(*_sink) << Quote() << key << Quote() << ':' << value << (more ? "," : "");
		}

		//---------------------------------------------------------------------
		// write a literal
		virtual void serialize(const std::string& value)
		{
			(*_sink) << Quote() << value << Quote() ;
		}

		//---------------------------------------------------------------------
		// write a delimiter
		virtual void serialize(TokenType type)
		{
            bool ok = true;
			const char* delimiter = 0;
			switch (type)
			{
			case T_OBJ_BEGIN:
				delimiter = "{";
				break;
			case T_OBJ_END:
				delimiter = "}";
				break;
			case T_ARRAY_BEGIN:
				delimiter = "[";
				break;
			case T_ARRAY_END:
				delimiter = "]";
				break;
			case T_COMMA:
				delimiter = ",";
				break;
			case T_COLON:
				delimiter = ":";
				break;
            default:
                    ok = false;
			}
            if (ok)
            {
                (*_sink) << delimiter;
            }
		}


		//---------------------------------------------------------------------
		// serialize a string value. needs to handle escape sequences
		virtual void serialize(std::string& value)
		{
			(*_sink) << Quote() << Chordia::escape(value) << Quote() ;
		}

		//---------------------------------------------------------------------
		// needs to convert to JSON \u encoding for
		// any characters outside of UTF8

		virtual void serialize(std::wstring& value)
		{
			(*_sink) << Quote() << Chordia::escape(Chordia::w2n(value)) << Quote() ;
		}

		//---------------------------------------------------------------------
		virtual void serialize(int& value)
		{
			(*_sink) << value;
		}

		//---------------------------------------------------------------------
		virtual void serialize(unsigned int& value)
		{
			(*_sink) << value;
		}

		//---------------------------------------------------------------------
		virtual void serialize(long& value)
		{
			(*_sink) << value;
		}

		//---------------------------------------------------------------------
		virtual void serialize(double& value)
		{
			(*_sink) << value;
		}

		//---------------------------------------------------------------------
		virtual void serialize(bool& value)
		{
			(*_sink) << value;
		}
	};

	//-----------------------------------------------------------------------------
	// template the writer. usage is:
	// std::string json = JSON::producer<JSONExample>::convert(source);
	template <typename serializable_type, typename sink_type = StringSink>
	class producer
	{
		public:
		static std::string convert(serializable_type& source)
		{
			Chordia::stringer json;
			// by default serialize to a string
			sink_type ssi(&json);
			// create a writer
			JSON::Writer writer(&ssi);
			// serialize the instance
			source.serialize(writer);
			// and get the JSON string from the sink
			//
#ifdef _INTEGRATED
#ifdef _UNICODE
			return Chordia::convert_w(json.c_str());
#else
			return json.c_str();
#endif
#else
			return json.c_str();
#endif
		}
	};
}


#endif


