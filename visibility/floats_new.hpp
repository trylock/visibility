#ifndef GEOMETRY_FLOATS_HPP_
#define GEOMETRY_FLOATS_HPP_

#include <limits>
#include <algorithm>

#include "vector2.hpp"

namespace geometry
{ 
    inline bool approx_equal(float a, float b, float epsilon = std::numeric_limits<float>::epsilon())
    {
        return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * epsilon;
    }

    inline bool strictly_less(float a, float b, float epsilon = std::numeric_limits<float>::epsilon())
    {
        return (b - a) > std::max(std::abs(a), std::abs(b)) * epsilon;
    }

    template<typename T>
    bool approx_equal(vector2<T> a, vector2<T> b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return approx_equal(a.x, b.x, epsilon) && approx_equal(a.y, b.y, epsilon);
    }

    template<typename T>
    bool strictly_less(vector2<T>& a, vector2<T> b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return strictly_less(a.x, b.x, epsilon) && strictly_less(a.y, b.y, epsilon);
    }
}

#endif // GEOMETRY_FLOATS_HPP_