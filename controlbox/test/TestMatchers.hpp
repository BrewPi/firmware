#pragma once

#include <catch.hpp>
#include <memory>
#include <sstream>

// LCOV_EXCL_START

template <typename T, int n>
class ArrayMatcher : public Catch::MatcherBase<T const*> {
    T target[n];

public:
    ArrayMatcher(const T*& _target)
    {
        for (int i = 0; i < n; i++) {
            target[i] = _target[i];
        }
    }
    virtual ~ArrayMatcher() = default;

    // Performs the test for this matcher
    virtual bool match(T const* const other) const override
    {
        for (int i = 0; i < n; i++) {
            if (other[i] != target[i]) {
                return false;
            }
        }
        return true;
    }

    virtual std::string describe() const override
    {
        std::stringstream ss;
        int i;
        for (i = 0; i < n - 1; i++) {
            ss << target[i] << ",";
        }
        ss << target[i] << " ==";
        return ss.str();
    }
};

// The builder function
template <typename T, int n>
inline ArrayMatcher<T, n>
equalsArray(const T* target)
{
    return ArrayMatcher<T, n>(target);
}

// LCOV_EXCL_STOP
