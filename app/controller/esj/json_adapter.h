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



#ifndef JSON_ADAPTER_H
#define JSON_ADAPTER_H

//
#include <string>
#include <list>
#include <stack>
#include <stdint.h>

// requires lexer for token types.
#include "json_lexer.h"
#include "temperatureFormats.h"

namespace JSON
{

//-----------------------------------------------------------------------------
// Base class for serialization operations
class Adapter
{
	// scope count
	int _count;
	
	public:

	Adapter() : _count(0) {}
	virtual ~Adapter(){}

	// must be implemented
	virtual bool storing() = 0;

	// do some counting so we know if we are the root object
	int inc() 
	{ 
		int ret = _count; _count++; return ret; 
	}
	//
	int dec() 
	{ 
		return (--_count);
	}

	// mm. this is warty.
	virtual bool more() { return true; }

	// we need some look-ahead
	virtual bool peek(TokenType) { return false; }

	// lexical elements like '{'
	virtual void serialize(TokenType) = 0;

	// support for fundamental types so we can stream vector<T>
	virtual void serialize(const std::string&) = 0;
	virtual void serialize(std::wstring&) = 0;
	virtual void serialize(std::string&) = 0;
	virtual void serialize(int8_t&) = 0;
	virtual void serialize(int16_t&) = 0;
	virtual void serialize(int32_t&) = 0;
	virtual void serialize(uint8_t&) = 0;
    virtual void serialize(uint16_t&) = 0;
    virtual void serialize(uint32_t&) = 0;
#ifndef ESJ_DISABLE_DOUBLE
    virtual void serialize(double&) = 0;
#endif
	virtual void serialize(bool&) = 0;
	virtual void serialize(temp_t&) = 0;
	virtual void serialize(temp_precise_t&) = 0;
	virtual void serialize(temp_long_t&) = 0;
	
	// key/value pairs
	virtual void serialize(const std::string&,std::wstring&,bool) = 0;
	virtual void serialize(const std::string&,std::string&,bool) = 0;
	virtual void serialize(const std::string&,int8_t&,bool) = 0;
	virtual void serialize(const std::string&,int16_t&,bool) = 0;
	virtual void serialize(const std::string&,int32_t&,bool) = 0;
	virtual void serialize(const std::string&,uint8_t&,bool) = 0;
    virtual void serialize(const std::string&,uint16_t&,bool) = 0;
    virtual void serialize(const std::string&,uint32_t&,bool) = 0;
#ifndef ESJ_DISABLE_DOUBLE
    virtual void serialize(const std::string&,double&,bool) = 0;
#endif
	virtual void serialize(const std::string&,bool&,bool) = 0;
	virtual void serialize(const std::string&,temp_t&,bool) = 0;
	virtual void serialize(const std::string&,temp_precise_t&,bool) = 0;
	virtual void serialize(const std::string&,temp_long_t&,bool) = 0;

};

//-----------------------------------------------------------------------------
#define DBMSGV(arg)

#ifndef _ASTRING
#define _ASTRING(arg) arg
#endif
#ifndef _USTRING
#define _USTRING(arg) L##arg
#endif

//-----------------------------------------------------------------------------
// Some helper macros for brevity
#define JSON_C(json_adapter,class_type_name)  JSON::Class __cls(json_adapter,_ASTRING(#class_type_name))
// Serializable element with more elements to follow
#define JSON_E(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),class_member,true)
// Serializable terminator, keep things simple for the JSON writer. No stream manipulations required
#define JSON_T(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),class_member,false)
//

//-----------------------------------------------------------------------------
// for literals
inline void stream(Adapter& adapter,TokenType type)
{
	//
	adapter.serialize(type);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,const std::string& value)
{
	//
	adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,std::string& value)
{
	//
	adapter.serialize(value);
}

//-----------------------------------------------------------------------------
// fundamental types so we can directly serialize vectors of string/wstring/int/bool ...
inline void stream(Adapter& adapter,std::wstring& value)
{
	//
	adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,int8_t& value)
{
	//
	adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,int16_t& value)
{
    //
    adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,int32_t& value)
{
    //
    adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter, uint8_t& value)
{
    //
    adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter, uint16_t& value)
{
    //
    adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter, uint32_t& value)
{
    //
    adapter.serialize(value);
}

#ifndef ESJ_DISABLE_DOUBLE
//-----------------------------------------------------------------------------
inline void	stream(Adapter& adapter,double& value)
{
	//
	adapter.serialize(value);
}
#endif

//-----------------------------------------------------------------------------
inline void	stream(Adapter& adapter,bool& value)
{
	//
	adapter.serialize(value);
}

//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,temp_t& value)
{
    //
    adapter.serialize(value);
}
//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,temp_precise_t& value)
{
    //
    adapter.serialize(value);
}
//-----------------------------------------------------------------------------
inline void stream(Adapter& adapter,temp_long_t& value)
{
    //
    adapter.serialize(value);
}


//-----------------------------------------------------------------------------
// string
inline void	stream(Adapter& adapter,const std::string& key,std::string& value,bool more)
{
	//
	adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// wstring
inline void stream(Adapter& adapter,const std::string& key,std::wstring& value,bool more)
{
	//
	adapter.serialize(key,value,more);
}

#ifndef ESJ_DISABLE_DOUBLE
//-----------------------------------------------------------------------------
// doubles
inline void	stream(Adapter& adapter,const std::string& key,double& value,bool more)
{
	adapter.serialize(key,value,more);
}
#endif

//-----------------------------------------------------------------------------
// int8_t
inline void	stream(Adapter& adapter,const std::string& key,int8_t& value,bool more)
{
	adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// int16_t
inline void stream(Adapter& adapter,const std::string& key,int16_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// int32_t
inline void stream(Adapter& adapter,const std::string& key,int32_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// uint8_t
inline void stream(Adapter& adapter,const std::string& key,uint8_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// uint16_t
inline void stream(Adapter& adapter,const std::string& key,uint16_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// uint32_t
inline void stream(Adapter& adapter,const std::string& key,uint32_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// bool
inline void	stream(Adapter& adapter,const std::string& key,bool& value,bool more)
{
	adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// temp_t
inline void stream(Adapter& adapter,const std::string& key,temp_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// temp_precise_t
inline void stream(Adapter& adapter,const std::string& key,temp_precise_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// temp_long_t
inline void stream(Adapter& adapter,const std::string& key,temp_long_t& value,bool more)
{
    adapter.serialize(key,value,more);
}

//-----------------------------------------------------------------------------
// arbitrary type T. must implement serialize with the correct signature
template <typename T>
inline void stream(Adapter& adapter,T& arg)
{
	//
	arg.serialize(adapter);
}

//-----------------------------------------------------------------------------
// arbitrary type T*. must implement serialize with the correct signature
template <typename T>
inline void stream(Adapter& adapter,T* arg)
{
    //
    arg->serialize(adapter);
}

//-----------------------------------------------------------------------------
// serialize a single instance of T. 
// Highlights an asymmetry in JSON (or more likely Javascript ...)
template <typename T>
inline void stream(Adapter& adapter,const std::string& key,T& value,bool more)
{
    // use class pointer function below
    stream(adapter,key, &value, more);
}

//-----------------------------------------------------------------------------
// serialize a single instance of T, where T is a class pointer.
template <typename T>
inline void stream(Adapter& adapter,const std::string& key,T * value,bool more)
{
    // handle the value
    adapter.serialize(key);
    adapter.serialize(T_COLON);

    if(value != nullptr){
        adapter.serialize(T_OBJ_BEGIN);
        value->serialize(adapter);
        adapter.serialize(T_OBJ_END);
    }
    else{
        adapter.serialize(T_NULL);
    }


    if (more)
    {
        // handle the trailing comma if this is part of a sequence
        adapter.serialize(T_COMMA);
    }
}


//-----------------------------------------------------------------------------
// this means we get (with quotes removed for clarity) vectors of string etc
// written out as:
// array : [ e0, e1, ..., en ]
// rather than
// array : [ { e0 }, { e1 }, ..., { eN } ]
// 
template <typename T>
inline void stream_primitives(Adapter& adapter,const std::string& key,std::vector<T>& value,bool more)
{
	typedef typename::std::vector<T>::iterator iterator_t;
	if (adapter.storing())
	{
		adapter.serialize(key);
		adapter.serialize(T_COLON);
		adapter.serialize(T_ARRAY_BEGIN);
		iterator_t it = value.begin();
		if (it != value.end())
		{
			// VC2012 cannot disambiguate the type of T when a vector of bool is used.
			T* t = (it);
			stream(adapter,t);
			for (++it; it != value.end(); ++it)
			{
				adapter.serialize(T_COMMA);
				t = it;
				stream(adapter,t);
			}
		}
		adapter.serialize(T_ARRAY_END);
		if (more)
		{
			adapter.serialize(T_COMMA);
		}
	}
	else
	{
		// expecting "name" ':'
		adapter.serialize(key);
		//
		adapter.serialize(T_COLON);
		// '['
		adapter.serialize(T_ARRAY_BEGIN);
		// cope with empty arrays so we need look-ahead here
		if (adapter.peek(T_ARRAY_END))
		{
			// ']'
			adapter.serialize(T_ARRAY_END);
		}
		else
		{
			do 
			{
				// create a new instance
				T t;
				// read off adapter
				stream(adapter,t);
				// push back into vector
				value.push_back(t);
				// keep going if we have a ',', end if ']'
			} while (adapter.more());
		}
		// 
		if (more)
		{
			// there will be more ...
			adapter.serialize(T_COMMA);
		}
	}
}

//-----------------------------------------------------------------------------
// this is the serializer for any non-primitive types including your own ...
template <typename T>
inline void stream_classes(Adapter& adapter,const std::string& key,std::vector<T>& value,bool more)
{
	typedef typename::std::vector<T>::iterator iterator_t;
	if (adapter.storing())
	{
		adapter.serialize(key);
		adapter.serialize(T_COLON);
		adapter.serialize(T_ARRAY_BEGIN);
		iterator_t it = value.begin();
		if (it != value.end())
		{
			// VC2012 cannot disambiguate the type of T when a vector of bool is used.
			T t = (*it);
			adapter.serialize(T_OBJ_BEGIN); // <-- achtung baby. avoid when JSONizing strings or numbers (!)
			stream(adapter,t);
			adapter.serialize(T_OBJ_END);
			for (++it; it != value.end(); ++it)
			{
				adapter.serialize(T_COMMA);
				t = (*it);
				adapter.serialize(T_OBJ_BEGIN);
				stream(adapter,t);
				adapter.serialize(T_OBJ_END);
			}
		}
		adapter.serialize(T_ARRAY_END);
		if (more)
		{
			adapter.serialize(T_COMMA);
		}
	}
	else
	{
		// expecting "name" ':'
		adapter.serialize(key);
		//
		adapter.serialize(T_COLON);
		// '['
		adapter.serialize(T_ARRAY_BEGIN);
		// cope with empty arrays so we need look-ahead here
		if (adapter.peek(T_ARRAY_END))
		{
			// ']'
			adapter.serialize(T_ARRAY_END);
		}
		else
		{
			do 
			{
				// create a new instance
				T t;
				adapter.serialize(T_OBJ_BEGIN);
				// read off adapter
				stream(adapter,t);

				adapter.serialize(T_OBJ_END); 
				// push back into vector
				value.push_back(t);
				// keep going if we have a ',', end if ']'
			} while (adapter.more());
		}
		// 
		if (more)
		{
			// there will be more ...
			adapter.serialize(T_COMMA);
		}
	}
}

	// each C++ class which can be serialized needs to declare a 
	// single Class instance in the top-most scope of the 
	// serialize() function. This is to ensure correct JSON class
	// delimiters are written to/read from the stream
	class Class
	{
		// adapter instance
		Adapter* m_pAdapter;

	public:

		Class(Adapter& adapter,const std::string& type_name) : m_pAdapter(&adapter) 
		{
			// monitor the nesting level - this could be checked at runtime
			// to ensure nesting is not excessively deep to protect stack resources.
			int count = m_pAdapter->inc();
			//
			//DBMSG(type_name << " " << count);
			// is this the opening scope?
			if (count == 0)
			{
				m_pAdapter->serialize(T_OBJ_BEGIN);
			}
			
			// emit/check "type_name"
			m_pAdapter->serialize("class");
			m_pAdapter->serialize(T_COLON);
			m_pAdapter->serialize(type_name);
			m_pAdapter->serialize(T_COMMA);
			m_pAdapter->serialize("variables");
			m_pAdapter->serialize(T_COLON);
			// constructor emits opening scope
			m_pAdapter->serialize(T_OBJ_BEGIN);
		}

		// this is a bit nasty as it causes a double exception
		// if you get a type mismatch or somesuch in the lexer
		~Class()
		{
			// destructor emits closing scope(s)
			m_pAdapter->serialize(T_OBJ_END);

			// is this the final scope?
			int count = m_pAdapter->dec();
			if (count == 0)
			{
				m_pAdapter->serialize(T_OBJ_END);
			}
		}
	};


template <typename T>
inline void stream_selector(Adapter& adapter,const std::string& key,std::vector<T>& value,bool more, std::true_type)
{
    stream_primitives<T>(adapter,key,value,more);
}

template <typename T>
inline void stream_selector(Adapter& adapter,const std::string& key,std::vector<T>& value,bool more, std::false_type)
{
    stream_classes<T>(adapter,key,value,more);
}

//-----------------------------------------------------------------------------
// serialize a vector of T
template <typename T>
inline void stream(Adapter& adapter,const std::string& key,std::vector<T>& value,bool more)
{
    constexpr bool is_primitive = std::is_same<T,std::wstring>::value ||
            std::is_same<T,std::string>::value ||
            std::is_integral<T>::value ||
            std::is_floating_point<T>::value;
    std::integral_constant<bool, is_primitive> is_primitive_constant;
    stream_selector<T>(adapter,key,value,more, is_primitive_constant);
}

}	// JSON


#endif
