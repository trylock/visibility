#ifndef GEOMETRY_PRIMITIVES_HPP_
#define GEOMETRY_PRIMITIVES_HPP_

#include <limits>
#include <cmath>

#include "vector2.hpp"
#include "floats.hpp"

namespace geometry
{
    enum class orientation
    {
        left_turn = 1,
        right_turn = -1,
        collinear = 0
    };

    // compute orientation of 3 points in a plane 
    inline orientation compute_orientation(vec2 a, vec2 b, vec2 c)
    {
        auto det = cross(b - a, c - a);
        return static_cast<orientation>(
            static_cast<int>(strictly_less(0.f, det)) - 
            static_cast<int>(strictly_less(det, 0.f))
        );
    }

    struct line_segment
    {
        vec2 a, b;

        line_segment() {}
        line_segment(vec2 a, vec2 b) : a(a), b(b) {}
        line_segment(const line_segment&) = default;
        line_segment& operator=(const line_segment& segment) = default;
    };

    struct ray
    {
        vec2 origin;
        vec2 direction;

        ray() {}
        ray(vec2 origin, vec2 direction) : origin(origin), direction(direction) {}

        /* Find the nearest intersection point of ray and line segment
           \param a line segment
           \param reference to a variable where the nearest intersection point will be stored
                  can be changed even when there is no intersection
           \return true <=> the ray intersects the line segment
        */
        bool intersects(const line_segment& segment, vec2& out_point) const
        {
            auto EPSILON = 1e-4f;
            auto ao = origin - segment.a;
            auto ab = segment.b - segment.a;
            auto det = cross(ab, direction);
            if (approx_equal(det, 0.f, EPSILON))
            {
                if (compute_orientation(segment.a, segment.b, origin) != orientation::collinear) 
                    return false;
                auto dist_a = dot(ao, direction);
                auto dist_b = dot(origin - segment.b, direction);

                if (dist_a > 0 && dist_b > 0) 
                    return false;
                else if ((dist_a > 0) != (dist_b > 0)) 
                    out_point = origin;
                else if (dist_a > dist_b)  // at this point, both distances are negative
                    out_point = segment.a; // hence the nearest point is A
                else 
                    out_point = segment.b;
                return true;
            }

            auto u = cross(ao, direction) / det;
            if (strictly_less(u, 0.f, EPSILON) || strictly_less(1.f, u, EPSILON)) 
                return false;

            auto t = -cross(ab, ao) / det;
            out_point = origin + t * direction;
            return approx_equal(t, 0.f, EPSILON) || t > 0;
        }
    };
}

#endif // GEOMETRY_PRIMITIVES_HPP_