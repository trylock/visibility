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
        union 
        {
            struct { T x, y; };
            std::array<T, 2> components;
        };

        vector2() {}
        vector2(T x, T y) : x(x), y(y) {}
        explicit vector2(const T& scalar) : x(scalar), y(scalar) {}

        // allow copy
        vector2(const vector2<T>&) = default;
        vector2& operator=(const vector2<T>&) = default;

        // Array like element access
        T& element(std::size_t index) { return components[index]; }
        const T& element(std::size_t index) const { return components[index]; }

        T& operator[](std::size_t index) { return element(index); }
        const T& operator[](std::size_t index) const { return element(index); }

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

    // Calculate standard dot product
    template<typename T>
    T dot(vector2<T> a, vector2<T> b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // Calculate squared length of a vector
    template<typename T>
    T length_squared(vector2<T> vector) 
    { 
        return dot(vector, vector); 
    }

    // Squared distance of 2 points 
    template<typename T>
    T distance_squared(vector2<T> a, vector2<T> b) 
    { 
        return length_squared(a - b); 
    }

    // return perpendicular 2D vector 
    template<typename T>
    vector2<T> normal(vector2<T> vector) 
    { 
        return{ -vector.y, vector.x }; 
    }

    // calculate det([a_x, b.x; a_y, b_y])
    template<typename T>
    T cross(vector2<T> a, vector2<T> b) 
    { 
        return a.x * b.y - a.y * b.x; 
    }

    // normalize a floating point vector (return the argument if its close to zero)
    template<typename T>
    vector2<T> normalize(vector2<T> vector)
    {
        T length = std::sqrt(dot(vector, vector));
        if (std::abs(length) < std::numeric_limits<T>::epsilon()) 
            return vector;
        vector /= length;
        return vector;
    }

    // print a vector to the output stream
    template<typename T>
    std::ostream& operator<<(std::ostream& output, vector2<T> vector)
    {
        output << "[" << vector.x << ", " << vector.y << "]";
        return output;
    }

    using vec2 = vector2<float>;
}

#endif // GEOMETRY_VECTOR2_HPP_