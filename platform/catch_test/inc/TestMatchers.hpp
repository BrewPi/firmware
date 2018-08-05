#pragma once

#include <memory>
#include <catch.hpp>

template<typename T, int n>
class ArrayMatcher : public Catch::MatcherBase<T const*> {
    T target[n];
public:
    ArrayMatcher(const T * & _target) {
        for(int i=0; i<n;i++){
            target[i] = _target[i];
        }

    }
    virtual ~ArrayMatcher() = default;

    // Performs the test for this matcher
    virtual bool match(T const * const other) const override {
        for(int i=0; i<n;i++){
            if(other[i] != target[i]){
                return false;
            }
        }
        return true;
    }

    virtual std::string describe() const override {
        std::stringstream ss;
        int i;
        for(i=0; i<n-1;i++){
            ss << target[i] << ",";
        }
        ss << target[i] << " ==";
        return ss.str();
    }
};

// The builder function
template<typename T, int n>
inline ArrayMatcher<T, n> equalsArray(const T * target) {
    return ArrayMatcher<T, n>(*&target);
}



template<typename T>
class PercentageMatcher : public Catch::MatcherBase<T> {
	T target;
	double maxError;
public:
    PercentageMatcher(T _target, double _maxError) : target(_target), maxError(_maxError){}
    virtual ~PercentageMatcher() = default;

    // Performs the test for this matcher
    virtual bool match( T const& v ) const override {
    	if(target == 0.0){
    		return true;
    	}
    	double e = double(100*(v-target)) / double(target);
        return (e >= 0.0) ? e <= maxError : e >= -maxError;
    }

    virtual std::string describe() const override {
        std::stringstream ss;
        ss << "is within " << maxError << "% of " << target;
        return ss.str();
    }
};

// The builder function
template<typename T>
inline PercentageMatcher<T> IsWithinPercentageOf(double maxError, T target) {
    return PercentageMatcher<T>(target, maxError );
}

template<typename T>
class MarginMatcher : public Catch::MatcherBase<T> {
	T target;
	double maxError;
public:
	MarginMatcher(T _target, T _maxError) : target(_target), maxError(_maxError){}
    virtual ~MarginMatcher() = default;

    // Performs the test for this matcher
    virtual bool match( T const& v ) const override {
    	T e = v-target;
        return (e > 0) ? e <= maxError : e >= -maxError;
    }

    virtual std::string describe() const override {
        std::stringstream ss;
        ss << "is within +/-" << maxError << " of " << target;
        return ss.str();
    }
};

// The builder function
template<typename T>
inline MarginMatcher<T> IsWithinOf(T maxError, T target) {
    return MarginMatcher<T>(target, maxError );
}
