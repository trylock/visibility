#ifndef GEOMETRY_VISIBILITY_HPP_
#define GEOMETRY_VISIBILITY_HPP_

#include <set>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>

#include "floats.hpp"
#include "vector2.hpp"
#include "primitives.hpp"

namespace geometry
{
    /* Compare 2 line segments based on their distance from given point
     * Assumes: (1) the line segments are intersected by some ray from the origin
     *          (2) the line segments do not intersect except at their endpoints
     *          (3) no line segment is collinear with the origin
     */
    template<typename Vector>
    struct line_segment_dist_comparer
    {
        using segment_type = line_segment<Vector>;

        Vector origin;

        explicit line_segment_dist_comparer(Vector origin) : 
            origin(origin)
        {
        }

        /** Check whether the line segment x is closer to the origin than the 
         * line segment y.
         * @param x line segment: left hand side of the comparison operator
         * @param y line segment: right hand side of the comparison operator
         * @return true iff x < y (x is closer than y)
         */
        bool operator()(const segment_type& x, const segment_type& y) const
        {
            auto a = x.a, b = x.b;
            auto c = y.a, d = y.b;

            assert(
                compute_orientation(origin, a, b) != orientation::collinear && 
                "AB must not be collinear with the origin.");
            assert(
                compute_orientation(origin, c, d) != orientation::collinear &&
                "CD must not be collinear with the origin.");

            // sort the endpoints so that if there are common endpoints,
            // it will be a and c 
            if (approx_equal(b, c) || approx_equal(b, d)) 
                std::swap(a, b);
            if (approx_equal(a, d)) 
                std::swap(c, d);

            // cases with common endpoints
            if (approx_equal(a, c))
            {
                auto oad = compute_orientation(origin, a, d);
                auto oab = compute_orientation(origin, a, b);
                if (approx_equal(b, d) || oad != oab) 
                    return false;
                return compute_orientation(a, b, d) != compute_orientation(a, b, origin);
            }

            // cases without common endpoints
            auto cda = compute_orientation(c, d, a);
            auto cdb = compute_orientation(c, d, b);
            if (cdb == orientation::collinear && cda == orientation::collinear)
            {
                return distance_squared(origin, a) < distance_squared(origin, c);
            }
            else if (cda == cdb || 
                cda == orientation::collinear || 
                cdb == orientation::collinear) 
            {
                auto cdo = compute_orientation(c, d, origin);
                return cdo == cda || cdo == cdb;
            }
            else
            {
                auto abo = compute_orientation(a, b, origin);
                return abo != compute_orientation(a, b, c);
            }
        }
    };

    // compare angles clockwise starting at the positive y axis
    template<typename Vector>
    struct angle_comparer
    {
        Vector vertex;

        explicit angle_comparer(Vector origin) : vertex(origin) {}

        bool operator()(const Vector& a, const Vector& b) const
        {
            auto is_a_left = strictly_less(a.x, vertex.x);
            auto is_b_left = strictly_less(b.x, vertex.x);
            if (is_a_left != is_b_left) 
                return is_b_left;

            if (approx_equal(a.x, vertex.x) && approx_equal(b.x, vertex.x))
            {
                if (!strictly_less(a.y, vertex.y) || 
                    !strictly_less(b.y, vertex.y))
                {
                    return strictly_less(b.y, a.y);
                }
                return strictly_less(a.y, b.y);
            }

            auto oa = a - vertex;
            auto ob = b - vertex;
            auto det = cross(oa, ob);
            if (approx_equal(det, 0.f))
            {
                return length_squared(oa) < length_squared(ob);
            }
            return det < 0;
        }
    };

    template<typename Vector>
    struct visibility_event
    {
        // events used in the visibility polygon algorithm
        enum event_type
        {
            start_vertex,
            end_vertex
        };

        event_type type;
        line_segment<Vector> segment;

        visibility_event() {}
        visibility_event(event_type type, const line_segment<Vector>& segment) :
            type(type), 
            segment(segment) {}

        const auto& point() const { return segment.a; }
    };

    /** Calculate visibility polygon vertices in clockwise order.
     * Endpoints of the line segments (obstacles) can be ordered arbitrarily.
     * Line segments collinear with the point are ignored.
     * @param point - position of the observer
     * @param begin iterator of the list of line segments (obstacles)
     * @param end iterator of the list of line segments (obstacles)
     * @return vector of vertices of the visibility polygon
     */
    template<typename Vector, typename InputIterator>
    std::vector<Vector> visibility_polygon(
        Vector point, 
        InputIterator begin,
        InputIterator end)
    {
        using segment_type = line_segment<Vector>;
        using event_type = visibility_event<Vector>;
        using segment_comparer_type = line_segment_dist_comparer<Vector>;

        segment_comparer_type cmp_dist{ point };
        std::set<segment_type, segment_comparer_type> state{ cmp_dist };
        std::vector<event_type> events;

        for (; begin != end; ++begin)
        {
            auto segment = *begin;

            // Sort line segment endpoints and add them as events
            // Skip line segments collinear with the point
            auto pab = compute_orientation(point, segment.a, segment.b);
            if (pab == orientation::collinear)
            {
                continue;
            }
            else if (pab == orientation::right_turn)
            {
                events.emplace_back(event_type::start_vertex, segment);
                events.emplace_back(
                    event_type::end_vertex, 
                    segment_type{ segment.b, segment.a });
            }
            else
            {
                events.emplace_back(
                    event_type::start_vertex, 
                    segment_type{ segment.b, segment.a });
                events.emplace_back(event_type::end_vertex, segment);
            }

            // Initialize state by adding line segments that are intersected
            // by vertical ray from the point
            auto a = segment.a, b = segment.b;
            if (a.x > b.x) 
                std::swap(a, b);

            auto abp = compute_orientation(a, b, point);
            if (abp == orientation::right_turn && 
                (approx_equal(b.x, point.x) ||
                (a.x < point.x && point.x < b.x)))
            {
                state.insert(segment);
            }
        }

        // sort events by angle
        angle_comparer<Vector> cmp_angle{ point };
        std::sort(events.begin(), events.end(), [&cmp_angle](auto&& a, auto&& b) 
        {
            // if the points are equal, sort end vertices first
            if (approx_equal(a.point(), b.point()))
                return a.type == event_type::end_vertex && 
                       b.type == event_type::start_vertex;
            return cmp_angle(a.point(), b.point());
        });

        // find the visibility polygon
        std::vector<Vector> vertices;
        for (auto&& event : events)
        {
            if (event.type == event_type::end_vertex) 
                state.erase(event.segment);

            if (state.empty())
            {
                vertices.push_back(event.point());
            }
            else if (cmp_dist(event.segment, *state.begin()))
            {
                // Nearest line segment has changed
                // Compute the intersection point with this segment
                vec2 intersection;
                auto intersects = ray<Vector>{ point, event.point() - point }
                    .intersects(*state.begin(), intersection);
                assert(intersects && 
                    "Ray intersects line segment L iff L is in the state");

                if (event.type == event_type::start_vertex)
                {
                    vertices.push_back(intersection);
                    vertices.push_back(event.point());
                }
                else
                {
                    vertices.push_back(event.point());
                    vertices.push_back(intersection);
                }
            }

            if (event.type == event_type::start_vertex) 
                state.insert(event.segment);
        }

        // remove collinear points
        auto top = vertices.begin();
        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            auto prev = top == vertices.begin() ? vertices.end() - 1 : top - 1;
            auto next = it + 1 == vertices.end() ? vertices.begin() : it + 1;
            if (compute_orientation(*prev, *it, *next) != orientation::collinear) 
                *top++ = *it;
        }
        vertices.erase(top, vertices.end());
        return vertices;
    }
}

#endif // GEOMETRY_VISIBILITY_HPP_