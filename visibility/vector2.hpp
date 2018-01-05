#ifndef GEOMETRY_VECTOR2_HPP_
#define GEOMETRY_VECTOR2_HPP_

#include <array>
#include <algorithm>
#include <limits>
#include <string>
#include <ostream>
#include <cmath>
#include <cassert>
#include <type_traits>

namespace geometry
{
    // Simple 2d vector type
    template<typename T>
    struct vector2
    {
        struct { T x, y; };

        vector2() {}
        vector2(T x, T y) : x(x), y(y) {}
        explicit vector2(const T& scalar) : x(scalar), y(scalar) {}

        // allow copy
        vector2(const vector2<T>&) = default;
        vector2& operator=(const vector2<T>&) = default;

        // Mutable operations
        template<typename VectorType>
        vector2<T>& operator+=(VectorType&& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        template<typename VectorType>
        vector2<T>& operator-=(VectorType&& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        
        template<typename VectorType>
        auto operator*=(VectorType&& other) 
            -> typename std::enable_if<!std::is_scalar<typename std::decay<decltype(other)>::type>::value, vector2<T>&>::type
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }
        
        template<typename VectorType>
        auto operator/=(VectorType&& other) 
            -> typename std::enable_if<!std::is_scalar<typename std::decay<decltype(other)>::type>::value, vector2<T>&>::type
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }
        
        template<typename ScalarType>
        auto operator*=(ScalarType&& scalar) 
            -> typename std::enable_if<std::is_scalar<typename std::decay<decltype(scalar)>::type>::value, vector2<T>&>::type
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        
        template<typename ScalarType>
        auto operator/=(ScalarType&& scalar) 
            -> typename std::enable_if<std::is_scalar<typename std::decay<decltype(scalar)>::type>::value, vector2<T>&>::type
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // immutable operations
        template<typename VectorType>
        vector2<T> operator+(VectorType&& other) const 
        {
            return { x + other.x, y + other.y };
        }
        
        template<typename VectorType>
        vector2<T> operator-(VectorType&& other) const 
        {
            return { x - other.x, y - other.y };
        }
        
        template<typename VectorType>
        auto operator*(VectorType&& other) const 
            -> typename std::enable_if<!std::is_scalar<typename std::decay<decltype(other)>::type>::value, vector2<T>>::type
        {
            return { x * other.x, y * other.y };
        }
        
        template<typename VectorType>
        auto operator/(VectorType&& other) const 
            -> typename std::enable_if<!std::is_scalar<typename std::decay<decltype(other)>::type>::value, vector2<T>>::type
        {
            return { x / other.x, y / other.y };
        }
        
        template<typename ScalarType>
        auto operator*(ScalarType&& scalar) const 
            -> typename std::enable_if<std::is_scalar<typename std::decay<decltype(scalar)>::type>::value, vector2<T>>::type
        {
            return { x * scalar, y * scalar };
        }
        
        template<typename ScalarType>
        friend auto operator*(ScalarType&& scalar, vector2<T> vector) 
            -> typename std::enable_if<std::is_scalar<typename std::decay<decltype(scalar)>::type>::value, vector2<T>>::type
        {
            return { vector.x * scalar, vector.y * scalar };
        }
        
        template<typename ScalarType>
        auto operator/(ScalarType&& scalar) const 
            -> typename std::enable_if<std::is_scalar<typename std::decay<decltype(scalar)>::type>::value, vector2<T>>::type
        {
            return { x / scalar, y / scalar };
        }

        friend vector2<T> operator-(vector2<T> vec)
        {
            return { -vec.x, -vec.y };
        }

        // comparison operators
        template<typename VectorType>
        bool operator==(VectorType&& other) const 
        {
            return x == other.x && y == other.y;
        }

        template<typename VectorType>
        bool operator!=(VectorType&& other) const 
        {
            return x != other.x || y != other.y;
        }
    };

    /** Calculate standard dot product.
     * @param a vector
     * @param b vector
     * @return dot product of the 2 vectors
     */
    template<typename Vector>
    auto dot(Vector a, Vector b)
    {
        return a.x * b.x + a.y * b.y;
    }

    /** Calculate squared length of a vector.
     * @param vector
     * @return squared length of the vector
     */
    template<typename Vector>
    auto length_squared(Vector vector)
    { 
        return dot(vector, vector); 
    }

    /** Squared distance of 2 points.
     * @param a point
     * @param b point
     * @return squared distance of the 2 points
     */
    template<typename Vector>
    auto distance_squared(Vector a, Vector b)
    { 
        return length_squared(a - b); 
    }

    /** Return orthogonal 2D vector.
     * @param vector
     * @return vector orthogonal to the argument
     */
    template<typename Vector>
    Vector normal(Vector vector)
    { 
        return{ -vector.y, vector.x }; 
    }

    /** Calculate det([a_x, b.x; a_y, b_y]).
     * @param a vector
     * @param b vector
     * @return det([a_x, b.x; a_y, b_y])
     */
    template<typename Vector>
    auto cross(Vector a, Vector b)
    { 
        return a.x * b.y - a.y * b.x; 
    }

    /** Normalize a floating point vector to have an unit length.
     * @param vector to normalize
     * @return normalized vector.
     *         If the vector is 0, it will return a 0 vector.
     *         If the vector is non-zero, it will return a vector with the same 
     *         direction and unit length.
     */
    template<typename Vector>
    Vector normalize(Vector vector)
    {
        using value_type = typename std::decay<decltype(vector.x)>::type;

        value_type length = std::sqrt(dot(vector, vector));
        if (std::abs(length) < std::numeric_limits<value_type>::epsilon())
            return vector;
        vector /= length;
        return vector;
    }

    /** Print a vector to the output stream.
     * @param output stream
     * @param vector to print
     * @return reference to given output stream
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& output, vector2<T> vector)
    {
        output << "[" << vector.x << ", " << vector.y << "]";
        return output;
    }

    using vec2 = vector2<float>;
}

#endif // GEOMETRY_VECTOR2_HPP_