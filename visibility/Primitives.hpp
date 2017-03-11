#ifndef GEOMETRY_PRIMITIVES_HPP_
#define GEOMETRY_PRIMITIVES_HPP_

#include <limits>
#include <cmath>

#include "Vector.hpp"
#include "Floats.hpp"

namespace geometry
{
    enum class Orientation
    {
        LeftTurn = 1,
        RightTrun = -1,
        Collinear = 0
    };

    // compute orientation of 3 points in a plane 
    inline Orientation orientation(vec2 a, vec2 b, vec2 c)
    {
        auto det = cross(b - a, c - a);
        return static_cast<Orientation>(
            static_cast<int>(strictly_less(0.f, det)) - 
            static_cast<int>(strictly_less(det, 0.f))
        );
    }

    struct LineSegment
    {
        vec2 a, b;

        LineSegment() {}
        LineSegment(vec2 a, vec2 b) : a(a), b(b) {}
        LineSegment(const LineSegment&) = default;
        LineSegment& operator=(const LineSegment& segment) = default;
    };

    struct Ray
    {
        vec2 origin;
        vec2 direction;

        Ray() {}
        Ray(vec2 origin, vec2 direction) : origin(origin), direction(direction) {}

        /* Find the nearest intersection point of ray with line segment
            \param a line segment
            \param reference to a variable where the nearest intersection point will be stored
                   can be changed even when there is no intersection
            \return true <=> the ray intersects the line segment
        */
        bool Intersects(const LineSegment& segment, vec2& out_point) const
        {
            auto EPSILON = 1e-4f;
            auto ao = origin - segment.a;
            auto ab = segment.b - segment.a;
            auto det = cross(ab, direction);
            if (approx_equal(det, 0.f, EPSILON))
            {
                if (orientation(segment.a, segment.b, origin) != Orientation::Collinear) return false;
                auto dist_a = dot(ao, direction);
                auto dist_b = dot(origin - segment.b, direction);

                if (dist_a > 0 && dist_b > 0) return false;
                else if ((dist_a > 0) != (dist_b > 0)) out_point = origin;
                else if (dist_a > dist_b) out_point = segment.a; // at this point, both distances are negative
                else out_point = segment.b;
                return true;
            }

            auto u = cross(ao, direction) / det;
            if (strictly_less(u, 0.f, EPSILON) || strictly_less(1.f, u, EPSILON)) return false;

            auto t = -cross(ab, ao) / det;
            out_point = origin + t * direction;
            return approx_equal(t, 0.f, EPSILON) || t > 0;
        }
    };
}

#endif // GEOMETRY_PRIMITIVES_HPP_