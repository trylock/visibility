#ifndef GEOMETRY_VISIBILITY_HPP_
#define GEOMETRY_VISIBILITY_HPP_

#include <set>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>

#include "Floats.hpp"
#include "Vector.hpp"
#include "Primitives.hpp"

namespace geometry
{
    /* Compare 2 line segments based on their distance from given point
       Assumes: (1) the line segments are intersected by some ray from the origin
                (2) the line segments do not intersect except at their endpoints
                (3) no line segment is collinear with the origin
    */
    struct LineSegmentDistComparator
    {
        vec2 origin;

        LineSegmentDistComparator(vec2 origin) : origin(origin) {}

        bool operator()(const LineSegment& x, const LineSegment& y) const
        {
            auto a = x.a, b = x.b;
            auto c = y.a, d = y.b;

            assert(orientation(origin, a, b) != Orientation::Collinear && "AB must not be collinear with the origin.");
            assert(orientation(origin, c, d) != Orientation::Collinear && "CD must not be collinear with the origin.");

            // sort the endpoints so that if there are common endpoints, it will be a and c 
            if (approx_equal(b, c) || approx_equal(b, d)) std::swap(a, b);
            if (approx_equal(a, d)) std::swap(c, d);

            // cases with common endpoints
            if (approx_equal(a, c))
            {
                if (approx_equal(b, d) || orientation(origin, a, d) != orientation(origin, a, b)) return false;
                return orientation(a, b, d) != orientation(a, b, origin);
            }

            // cases without common endpoints
            auto cda = orientation(c, d, a);
            auto cdb = orientation(c, d, b);
            if (cdb == Orientation::Collinear && cda == Orientation::Collinear)
            {
                return distance_squared(origin, a) < distance_squared(origin, c);
            }
            else if (cda == cdb || cda == Orientation::Collinear || cdb == Orientation::Collinear)
            {
                auto cdo = orientation(c, d, origin);
                return cdo == cda || cdo == cdb;
            }
            else
            {
                return orientation(a, b, origin) != orientation(a, b, c);
            }
        }
    };

    // compare angles clockwise starting at the positive y axis
    struct AngleComparator
    {
        vec2 vertex;

        AngleComparator(vec2 origin) : vertex(origin) {}

        bool operator()(const vec2& a, const vec2& b) const
        {
            auto is_a_left = strictly_less(a[0], vertex[0]);
            auto is_b_left = strictly_less(b[0], vertex[0]);
            if (is_a_left != is_b_left) return is_b_left;

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

    // events used in the visibility polygon algorithm
    enum class VisibilityEventType
    {
        StartVertex,
        EndVertex
    };

    struct VisibilityEvent
    {
        VisibilityEventType type;
        LineSegment segment;

        VisibilityEvent() {}
        VisibilityEvent(VisibilityEventType type, const LineSegment& segment) : type(type), segment(segment) {}

        inline const vec2& point() const { return segment.a; }
    };

    /*! Calculate visibility polygon vertices in clockwise order
        \param position of the observer
        \param list of arbitrary ordered line segments (line segments collinear with the point are ignored)
        \return vector of vertices of the visibility polygon
     */
    std::vector<vec2> VisibilityPolygon(vec2 point, const std::vector<LineSegment>& segments)
    {
        LineSegmentDistComparator cmp_dist{ point };
        std::set<LineSegment, LineSegmentDistComparator> state{ cmp_dist };
        std::vector<VisibilityEvent> events;
        events.reserve(segments.size() * 2 + 1);

        for (auto&& segment : segments)
        {
            // Sort line segment endpoints and add them as events
            // Skip line segments collinear with the point
            if (orientation(point, segment.a, segment.b) == Orientation::Collinear)
            {
                continue;
            }
            else if (orientation(point, segment.a, segment.b) == Orientation::RightTrun)
            {
                events.emplace_back(VisibilityEventType::StartVertex, segment);
                events.emplace_back(VisibilityEventType::EndVertex, LineSegment{ segment.b, segment.a });
            }
            else
            {
                events.emplace_back(VisibilityEventType::StartVertex, LineSegment{ segment.b, segment.a });
                events.emplace_back(VisibilityEventType::EndVertex, segment);
            }

            // Initialize state by adding line segments that are intersected by vertical ray from the point
            auto a = segment.a, b = segment.b;
            if (a[0] > b[0]) std::swap(a, b);

            auto abp = orientation(a, b, point);
            if ((approx_equal(b[0], point[0]) || (a[0] < point[0] && point[0] < b[0])) && abp == Orientation::RightTrun)
            {
                state.insert(segment);
            }
        }

        // sort events by angle
        AngleComparator cmp_angle{ point };
        std::sort(events.begin(), events.end(), [&cmp_angle](const VisibilityEvent& a, const VisibilityEvent& b) {
            return approx_equal(a.point(), b.point()) ?
                a.type == VisibilityEventType::EndVertex && b.type == VisibilityEventType::StartVertex : // if the points are equal, sort end vertices first
                cmp_angle(a.point(), b.point());
        });

        // find the visibility polygon
        std::vector<vec2> vertices;
        for (auto&& event : events)
        {
            if (event.type == VisibilityEventType::EndVertex) state.erase(event.segment);

            if (state.empty())
            {
                vertices.push_back(event.point());
            }
            else if (cmp_dist(event.segment, *state.begin()))
            {
                vec2 intersection;
                auto result = Ray{ point, event.point() - point }.Intersects(*state.begin(), intersection);
                assert(result && "Ray intersects a line segment iff the line segment is in the state");

                if (event.type == VisibilityEventType::StartVertex)
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

            if (event.type == VisibilityEventType::StartVertex) state.insert(event.segment);
        }

        // remove collinear points
        auto top = vertices.begin();
        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            auto prev = top == vertices.begin() ? vertices.end() - 1 : top - 1;
            auto next = it + 1 == vertices.end() ? vertices.begin() : it + 1;
            if (orientation(*prev, *it, *next) != Orientation::Collinear) *top++ = *it;
        }
        vertices.erase(top, vertices.end());
        return vertices;
    }
}

#endif // GEOMETRY_VISIBILITY_HPP_