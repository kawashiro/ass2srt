#ifndef ASS2SRT_MATHUTILS_H
#define ASS2SRT_MATHUTILS_H

#include <cmath>
#include <limits>

namespace ass2srt::mathutils {
/**
 * Check that IEEE754-compat numbers are close to be equal
 */
template <typename T>
auto is_close(T first, T second) -> bool
{
    return std::fabs(first - second) < std::numeric_limits<T>::epsilon();
}
}

#endif
