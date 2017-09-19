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
       Assumes: (1) the line segments are intersected by some ray from the origin
                (2) the line segments do not intersect except at their endpoints
                (3) no line segment is collinear with the origin
    */
    struct line_segment_dist_comparer
    {
        vec2 origin;

        explicit line_segment_dist_comparer(vec2 origin) : origin(origin) {}

        bool operator()(const line_segment& x, const line_segment& y) const
        {
            auto a = x.a, b = x.b;
            auto c = y.a, d = y.b;

            assert(
                compute_orientation(origin, a, b) != orientation::collinear && 
                "AB must not be collinear with the origin.");
            assert(
                compute_orientation(origin, c, d) != orientation::collinear &&
                "CD must not be collinear with the origin.");

            // sort the endpoints so that if there are common endpoints, it will be a and c 
            if (approx_equal(b, c) || approx_equal(b, d)) 
                std::swap(a, b);
            if (approx_equal(a, d)) std::swap(c, d);

            // cases with common endpoints
            if (approx_equal(a, c))
            {
                if (approx_equal(b, d) || compute_orientation(origin, a, d) != compute_orientation(origin, a, b)) 
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
            else if (cda == cdb || cda == orientation::collinear || cdb == orientation::collinear)
            {
                auto cdo = compute_orientation(c, d, origin);
                return cdo == cda || cdo == cdb;
            }
            else
            {
                return compute_orientation(a, b, origin) != compute_orientation(a, b, c);
            }
        }
    };

    // compare angles clockwise starting at the positive y axis
    struct angle_comparer
    {
        vec2 vertex;

        explicit angle_comparer(vec2 origin) : vertex(origin) {}

        bool operator()(const vec2& a, const vec2& b) const
        {
            auto is_a_left = strictly_less(a[0], vertex[0]);
            auto is_b_left = strictly_less(b[0], vertex[0]);
            if (is_a_left != is_b_left) 
                return is_b_left;

            if (approx_equal(a[0], vertex[0]) && approx_equal(b[0], vertex[0]))
            {
                if (!strictly_less(a[1], vertex[1]) || !strictly_less(b[1], vertex[1]))
                {
                    return strictly_less(b[1], a[1]);
                }
                return strictly_less(a[1], b[1]);
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

    struct visibility_event
    {
        // events used in the visibility polygon algorithm
        enum event_type
        {
            start_vertex,
            end_vertex
        };

        event_type type;
        line_segment segment;

        visibility_event() {}
        visibility_event(event_type type, const line_segment& segment) : type(type), segment(segment) {}

        inline const vec2& point() const { return segment.a; }
    };

    /*! Calculate visibility polygon vertices in clockwise order
        \param position of the observer
        \param list of arbitrary ordered line segments (line segments collinear with the point are ignored)
        \return vector of vertices of the visibility polygon
     */
    std::vector<vec2> visibility_polygon(vec2 point, const std::vector<line_segment>& segments)
    {
        line_segment_dist_comparer cmp_dist{ point };
        std::set<line_segment, line_segment_dist_comparer> state{ cmp_dist };
        std::vector<visibility_event> events;
        events.reserve(segments.size() * 2 + 1);

        for (auto&& segment : segments)
        {
            // Sort line segment endpoints and add them as events
            // Skip line segments collinear with the point
            if (compute_orientation(point, segment.a, segment.b) == orientation::collinear)
            {
                continue;
            }
            else if (compute_orientation(point, segment.a, segment.b) == orientation::right_turn)
            {
                events.emplace_back(visibility_event::start_vertex, segment);
                events.emplace_back(visibility_event::end_vertex, line_segment{ segment.b, segment.a });
            }
            else
            {
                events.emplace_back(visibility_event::start_vertex, line_segment{ segment.b, segment.a });
                events.emplace_back(visibility_event::end_vertex, segment);
            }

            // Initialize state by adding line segments that are intersected by vertical ray from the point
            auto a = segment.a, b = segment.b;
            if (a[0] > b[0]) 
                std::swap(a, b);

            auto abp = compute_orientation(a, b, point);
            if ((approx_equal(b[0], point[0]) || (a[0] < point[0] && point[0] < b[0])) && abp == orientation::right_turn)
            {
                state.insert(segment);
            }
        }

        // sort events by angle
        angle_comparer cmp_angle{ point };
        std::sort(events.begin(), events.end(), [&cmp_angle](auto&& a, auto&& b) 
        {
            // if the points are equal, sort end vertices first
            if (approx_equal(a.point(), b.point()))
                return a.type == visibility_event::end_vertex && b.type == visibility_event::start_vertex;
            return cmp_angle(a.point(), b.point());
        });

        // find the visibility polygon
        std::vector<vec2> vertices;
        for (auto&& event : events)
        {
            if (event.type == visibility_event::end_vertex) 
                state.erase(event.segment);

            if (state.empty())
            {
                vertices.push_back(event.point());
            }
            else if (cmp_dist(event.segment, *state.begin()))
            {
                vec2 intersection;
                auto result = ray{ point, event.point() - point }.intersects(*state.begin(), intersection);
                assert(result && "Ray intersects a line segment iff the line segment is in the state");

                if (event.type == visibility_event::start_vertex)
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

            if (event.type == visibility_event::start_vertex) 
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