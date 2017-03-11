#ifndef GEOMETRY_VECTOR_HPP_
#define GEOMETRY_VECTOR_HPP_

#include <array>
#include <algorithm>
#include <limits>
#include <string>
#include <ostream>
#include <cmath>
#include <cassert>

namespace geometry
{
    // Fixed-size vector
    template<typename T, std::size_t num_components>
    struct Vector
    {
        std::array<T, num_components> data;

        Vector() {}
        Vector(const std::array<T, num_components>& data) : data(data) {}
        explicit Vector(const T& scalar) { for (std::size_t i = 0; i < num_components; i++) data[i] = scalar; }
        Vector(const Vector<T, num_components>&) = default;
        Vector& operator=(const Vector<T, num_components>&) = default;
        Vector(const std::initializer_list<T>& list) 
        { 
            assert(list.size() == num_components);
            std::size_t index = 0;
            for (auto value : list) data[index++] = value; 
        }

        // Element access
        inline T& element(std::size_t index) { return data[index]; }
        inline const T& element(std::size_t index) const { return data[index]; }

        inline T& operator[](std::size_t index) { return element(index); }
        inline const T& operator[](std::size_t index) const { return element(index); }

        // Mutable operations
        Vector<T, num_components>& operator+=(const Vector<T, num_components>& vector)
        {
            for (std::size_t i = 0; i < num_components; i++) data[i] += vector.data[i];
            return *this;
        }
        
        Vector<T, num_components>& operator-=(const Vector<T, num_components>& vector)
        {
            for (std::size_t i = 0; i < num_components; i++) data[i] -= vector.data[i];
            return *this;
        }
        
        Vector<T, num_components>& operator*=(const T& scalar)
        {
            for (auto&& comp : data) comp *= scalar;
            return *this;
        }

        Vector<T, num_components>& operator/=(const T& scalar)
        {
            for (auto&& comp : data) comp /= scalar;
            return *this;
        }

        // Immutable operations
        Vector<T, num_components> operator+(const Vector<T, num_components>& vector) const
        {
            Vector<T, num_components> result;
            for (std::size_t i = 0; i < num_components; i++) result.data[i] = data[i] + vector.data[i];
            return result;
        }
        
        Vector<T, num_components> operator-(const Vector<T, num_components>& vector) const
        {
            Vector<T, num_components> result;
            for (std::size_t i = 0; i < num_components; i++) result.data[i] = data[i] - vector.data[i];
            return result;
        }
        
        Vector<T, num_components> operator*(const T& scalar) const
        {
            Vector<T, num_components> result;
            for (std::size_t i = 0; i < num_components; i++) result.data[i] = data[i] * scalar;
            return result;
        }

        Vector<T, num_components> operator/(const T& scalar) const
        {
            Vector<T, num_components> result;
            for (std::size_t i = 0; i < num_components; i++) result.data[i] = data[i] / scalar;
            return result;
        }

        Vector<T, num_components> operator-() const
        {
            return *this * (-1);
        }

        // comparison

        bool operator==(const Vector<T, num_components>& vector) const
        {
            for (std::size_t i = 0; i < num_components; i++)
            {
                if (vector[i] != element(i)) return false;
            }
            return true;
        }

        bool operator!=(const Vector<T, num_components>& vector) const
        {
            return !operator==(vector);
        }
    };

    template<typename T, std::size_t num_components>
    Vector<T, num_components> operator*(T scalar, const Vector<T, num_components>& vector) { return vector * scalar; }

    template<typename T, std::size_t num_components>
    T dot(const Vector<T, num_components>& a, const Vector<T, num_components>& b)
    {
        T dot{ 0 };
        for (std::size_t i = 0; i < num_components; i++) dot += a.data[i] * b.data[i];
        return dot;
    }

    template<typename T, std::size_t num_components>
    T length_squared(const Vector<T, num_components>& vector) { return dot(vector, vector); }

    // squared distance of 2 points 
    template<typename T, std::size_t num_components>
    T distance_squared(const Vector<T, num_components>& a, const Vector<T, num_components>& b) { return length_squared(a - b); }

    // return perpendicular 2D vector 
    template<typename T>
    Vector<T, 2> normal(const Vector<T, 2>& vector) { return{ -vector[1], vector[0] }; }

    // calculate det([a_x, b.x; a_y, b_y])
    template<typename T>
    T cross(const Vector<T, 2>& a, const Vector<T, 2>& b) { return a[0] * b[1] - a[1] * b[0]; }

    // normalize a floating point vector
    template<typename T, std::size_t num_components>
    Vector<T, num_components> normalize(const Vector<T, num_components>& vector)
    {
        T length = std::sqrt(dot(vector, vector));
        if (std::abs(length) < std::numeric_limits<T>::epsilon()) return vector;
        return vector * (T(1) / length);
    }

    // print a vector to the output stream
    template<typename T, std::size_t num_components>
    std::ostream& operator<<(std::ostream& output, const Vector<T, num_components>& vector)
    {
        output << "[";
        for (std::size_t i = 0; i < num_components - 1; i++)
        {
            output << vector[i] << ", ";
        }
        output << vector[num_components - 1] << "]";
        return output;
    }

    using vec2 = Vector<float, 2>;
}

#endif // GEOMETRY_VECTOR_HPP_