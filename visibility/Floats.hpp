#ifndef GEOMETRY_FLOATS_HPP_
#define GEOMETRY_FLOATS_HPP_

#include <limits>
#include <algorithm>

#include "Vector.hpp"

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

    template<typename T, std::size_t num_components>
    bool approx_equal(
        const Vector<T, num_components>& a, 
        const Vector<T, num_components>& b, 
        T epsilon = std::numeric_limits<T>::epsilon())
    {
        for (std::size_t i = 0; i < num_components; i++)
        {
            if (!approx_equal(a[i], b[i], epsilon)) return false;
        }
        return true;
    }

    template<typename T, std::size_t num_components>
    bool strictly_less(
        const Vector<T, num_components>& a,
        const Vector<T, num_components>& b,
        T epsilon = std::numeric_limits<T>::epsilon())
    {
        for (std::size_t i = 0; i < num_components; i++)
        {
            if (!strictly_less(a[i], b[i], epsilon)) return false;
        }
        return true;
    }

}

#endif // GEOMETRY_FLOATS_HPP_