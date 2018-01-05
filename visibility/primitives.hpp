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

    /** Compute orientation of 3 points in a plane.
     * @param a first point
     * @param b second point
     * @param c third point
     * @return orientation of the points in the plane (left turn, right turn 
     *         or collinear)
     */
    template<typename Vector>
    orientation compute_orientation(Vector a, Vector b, Vector c)
    {
        auto det = cross(b - a, c - a);
        return static_cast<orientation>(
            static_cast<int>(strictly_less(0.f, det)) - 
            static_cast<int>(strictly_less(det, 0.f))
        );
    }

    template<typename Vector>
    struct line_segment
    {
        Vector a, b;

        line_segment() {}
        line_segment(Vector a, Vector b) : a(a), b(b) {}
        line_segment(const line_segment&) = default;
        line_segment& operator=(const line_segment& segment) = default;
    };

    template<typename Vector>
    struct ray
    {
        Vector origin;
        Vector direction;

        ray() {}
        ray(Vector origin, Vector direction) : 
            origin(origin), 
            direction(direction) {}

        /** Find the nearest intersection point of ray and line segment.
         * @param segment 
         * @param out_point reference to a variable where the nearest 
         *        intersection point will be stored (can be changed even 
         *        when there is no intersection)
         * @return true iff the ray intersects the line segment
         */
        bool intersects(
            const line_segment<Vector>& segment, 
            Vector& out_point) const
        {
            auto ao = origin - segment.a;
            auto ab = segment.b - segment.a;
            auto det = cross(ab, direction);
            if (approx_equal(det, 0.f))
            {
                auto abo = compute_orientation(segment.a, segment.b, origin);
                if (abo != orientation::collinear) 
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
            if (strictly_less(u, 0.f) || 
                strictly_less(1.f, u)) 
                return false;

            auto t = -cross(ab, ao) / det;
            out_point = origin + t * direction;
            return approx_equal(t, 0.f) || t > 0;
        }
    };
}

#endif // GEOMETRY_PRIMITIVES_HPP_