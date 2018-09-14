#include <functional>

// clamp is C++17, we're on C++14. This is a copy of the std implementation

namespace std {

template <class T>
constexpr const T&
clamp(const T& v, const T& lo, const T& hi)
{
    return clamp(v, lo, hi, std::less<>());
}

template <class T, class Compare>
constexpr const T&
clamp(const T& v, const T& lo, const T& hi, Compare comp)
{
    return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}
}
