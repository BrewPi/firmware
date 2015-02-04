/**
 * Copyright 2014  Matthew McGowan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GENERATORS_H
#define	GENERATORS_H

#include <vector>

class Generator {
public:
    virtual uint8_t next()=0;    
    
    void skip(unsigned int count) {
        while (count-->0) 
            next();
    }
};


class ValueGenerator : public Generator {
protected:    
    uint8_t _value;
public:
    ValueGenerator(uint8_t value) : _value(value) {}
    
    uint8_t next() { return _value; }
};


class SequenceGenerator : public ValueGenerator {
public:    
    SequenceGenerator(uint8_t startValue) : ValueGenerator(startValue) {}
    uint8_t next() { return _value++; }
};


#ifdef __GXX_EXPERIMENTAL_CXX0X__

#include <random>
class RandomGenerator : public Generator {
    std::default_random_engine generator;
    std::uniform_int_distribution<uint8_t> distribution;
public:
    RandomGenerator(int seed) : generator(seed), distribution(0,255) {}    
    uint8_t next() { return distribution(generator); }
};
#endif


class PushBackGenerator : public Generator {
    
    Generator& delegate;
    std::vector<uint8_t> pushback;
    
public:

    PushBackGenerator(Generator& source) : delegate(source) {}
    
    uint8_t next() {
        if (pushback.empty()) {
            return delegate.next();
        }
        else {
            uint8_t result = pushback.back();
            pushback.pop_back();
            return result;
        }
    }
    
    void push_back(uint8_t value) {
        pushback.push_back(value);
    }
};


#endif	/* GENERATORS_H */

