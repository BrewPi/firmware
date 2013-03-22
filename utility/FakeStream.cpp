/*
Copyright (c) 2009-2013 Matthew Murdoch

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
#include "FakeStream.h"

FakeStream::FakeStream() : _nextByte(-1) {
}

FakeStream::~FakeStream() {
}

size_t FakeStream::write(uint8_t val) {
	
	_bytesWritten.reserve((_bytesWritten.length() + 32) & ~(32-1));
    _bytesWritten += (char) val;

    return size_t(1);
}

void FakeStream::flush() {
}

#if 0
size_t FakeStream::print(const char* s) {
	_bytesWritten += s;
	return strlen(s);
}

size_t FakeStream::print(const String& s) {
	_bytesWritten += s;
	return s.length();
}
#endif

const String& FakeStream::bytesWritten() {
    return _bytesWritten;
}

void FakeStream::clearBytesWritten() {
	_bytesWritten = NULL;
}

void FakeStream::setToEndOfStream() {
    _nextByte = -1;
}

void FakeStream::nextByte(byte b) {
    _nextByte = b;
}

int FakeStream::available()  {
    return _nextByte==-1 ? 0 : 1;
}

int FakeStream::read() {
    int b = _nextByte;
        _nextByte = -1;
    return b;
}

int FakeStream::peek() {
    return _nextByte;
}
