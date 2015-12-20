esj
===

###Extremely Simple JSON for C++

Enable bi-directional JSON serialization of C++ classes with via dependency-free header only library written in tested, portable C++ (Visual C++/Clang/GCC/ARM) 

Usage:

```
//-----------------------------------------------------------------------------
// Code support required for serialization.
class JSONExample
{
public:
    // to be JSON'ised
    std::string text;
public:
    // each class requires a public serialize function
    void serialize(JSON::Adapter& adapter)
    {
        // this pattern is required 
        JSON::Class root(adapter,"JSONExample");
        // this is the last member variable we serialize so use the _T variant
        JSON_T(adapter,text);
    }
};
```

```
// demonstrate how to first produce and then consume JSON strings
int main(int argc,char* argv[])
{
    // try/catch omitted for brevity
    // The JSON enabled class as above
    JSONExample source;
    source.text = "Hello JSON World";
    // create JSON from a producer
    std::string json = JSON::producer<JSONExample>::convert(source);
    // and then create a new instance from a consumer ...
    JSONExample sink = JSON::consumer<JSONExample>::convert(json);
    // we are done ...
}
```
produces:

```
{"JSONExample":{"text":"Hello JSON World"}}
```

###Notes

std::string maps to JSON string.

std::wstring maps to JSON string with support for \UXXXX encoding and decoding (UTF16 and UTF32).

int maps to JSON number (ignores the fractional part when de-serializing).

double also maps to JSON number.

bool maps to JSON true or false.

std::vector<T> maps directly to a JSON array. 

If T implements the correct serialize() function, then the serializer will work as expected for vectors of T.

The serializer will also correctly handle nested serializable instances, thus allowing fairly complex constructs to be easily transformed to and from JSON.

Single line macros used for brevity and stringizing only.

See /doc for more or visit: http://www.codeproject.com/script/Articles/View.aspx?aid=856277
