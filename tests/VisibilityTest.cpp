#include "catch.hpp"
#include <visibility/Visibility.hpp>

void test_line_segment_is_closer(
    const geometry::LineSegmentDistComparator& cmp, 
    geometry::vec2 a, geometry::vec2 b,
    geometry::vec2 c, geometry::vec2 d)
{
    REQUIRE(cmp({ a, b }, { c, d }));
    REQUIRE(cmp({ b, a }, { c, d }));
    REQUIRE(cmp({ a, b }, { d, c }));
    REQUIRE(cmp({ b, a }, { d, c }));

    REQUIRE_FALSE(cmp({ c, d }, { a, b }));
    REQUIRE_FALSE(cmp({ d, c }, { a, b }));
    REQUIRE_FALSE(cmp({ c, d }, { b, a }));
    REQUIRE_FALSE(cmp({ d, c }, { b, a }));
}

void test_line_segments_are_equal(
    const geometry::LineSegmentDistComparator& cmp,
    geometry::vec2 a, geometry::vec2 b,
    geometry::vec2 c, geometry::vec2 d)
{
    REQUIRE_FALSE(cmp({ a, b }, { c, d }));
    REQUIRE_FALSE(cmp({ b, a }, { c, d }));
    REQUIRE_FALSE(cmp({ a, b }, { d, c }));
    REQUIRE_FALSE(cmp({ b, a }, { d, c }));

    REQUIRE_FALSE(cmp({ c, d }, { a, b }));
    REQUIRE_FALSE(cmp({ d, c }, { a, b }));
    REQUIRE_FALSE(cmp({ c, d }, { b, a }));
    REQUIRE_FALSE(cmp({ d, c }, { b, a }));
}

TEST_CASE("Compare 2 line segments with no common endpoints", "[visibility][dist_comp]")
{
    geometry::LineSegmentDistComparator cmp{ { 0, 0 } };

    test_line_segment_is_closer(cmp, { 1, 1 }, { 1, -1 }, { 2, 1 }, { 2, -1 });
    test_line_segment_is_closer(cmp, { 1, 1 }, { 1, -1 }, { 2, 2 }, { 2, 3 });
}

TEST_CASE("Compare 2 line segments with common endpoints", "[visibility][dist_comp]")
{
    geometry::LineSegmentDistComparator cmp{ { 0, 0 } };

    test_line_segments_are_equal(cmp, { 1, 1 }, { 1, 0 }, { 1, 0 }, { 1, -1 });
    test_line_segments_are_equal(cmp, { 1, 1 }, { 1, 0 }, { 1, 0 }, { 1, 1 });
    test_line_segment_is_closer(cmp, { 2, 0 }, { 1, 1 }, { 2, 1 }, { 2, 0 });
    test_line_segment_is_closer(cmp, { 2, 1 }, { 2, 0 }, { 2, 0 }, { 3, 1 });
}

TEST_CASE("Compare angle with 2 points in general position", "[visibility][angle_comp]")
{
    geometry::AngleComparator cmp{ { 0, 0 } };

    REQUIRE(cmp({ 0, 1 }, { 1, 1 }));
    REQUIRE_FALSE(cmp({ 1, 1 }, { 0, 1 }));

    REQUIRE(cmp({ 1, 1 }, { 1, -1 }));
    REQUIRE_FALSE(cmp({ 1, -1 }, { 1, 1 }));

    REQUIRE(cmp({ 1, 0 }, { -1, -1 }));
    REQUIRE_FALSE(cmp({ -1, -1 }, { 1, 0 }));

    REQUIRE(cmp({ 0, 1 }, { 0, -1 }));
    REQUIRE_FALSE(cmp({ 0, -1 }, { 0, 1 }));
}

TEST_CASE("Compare angle with 2 points if they are collinear with the origin", "[visibility][angle_comp]")
{
    geometry::AngleComparator cmp{ { 0, 0 } };

    REQUIRE(cmp({ 1, 0 }, { 2, 0 }));
    REQUIRE_FALSE(cmp({ 2, 0 }, { 1, 0 }));

    REQUIRE_FALSE(cmp({ 1, 0 }, { 1, 0 }));
    REQUIRE_FALSE(cmp({ 0, 0 }, { 0, 0 }));
}

TEST_CASE("Calculate visibility polygon with no line segments", "[visibility]")
{
    auto poly = geometry::VisibilityPolygon({ 0, 0 }, {});
    REQUIRE(poly.size() == 0);
}

TEST_CASE("Calculate visibility polygon with no obstaces apart from the boundary", "[visibility]")
{
    using namespace geometry;
    std::vector<LineSegment> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } }
    };

    auto poly = VisibilityPolygon({ 0, 0 }, segments);
    REQUIRE(poly.size() == 4);
    REQUIRE(approx_equal(poly[0], { 250, 250 }));
    REQUIRE(approx_equal(poly[1], { 250, -250 }));
    REQUIRE(approx_equal(poly[2], { -250, -250 }));
    REQUIRE(approx_equal(poly[3], { -250, 250 }));
}

TEST_CASE("Calculate visibility polygon with a polyline as an obstacle", "[visibility]")
{
    using namespace geometry;
    std::vector<LineSegment> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } },

        { { -50, 50 },{ 50, 50 } },
        { { 50, 50 },{ 50, -50 } },
    };

    auto poly = VisibilityPolygon({ 0, 0 }, segments);
    REQUIRE(poly.size() == 6);
    REQUIRE(approx_equal(poly[0], { 50, 50 }));
    REQUIRE(approx_equal(poly[1], { 50, -50 }));
    REQUIRE(approx_equal(poly[2], { 250, -250 }));
    REQUIRE(approx_equal(poly[3], { -250, -250 }));
    REQUIRE(approx_equal(poly[4], { -250, 250 }));
    REQUIRE(approx_equal(poly[5], { -50, 50 }));
}

TEST_CASE("Calculate visibility polygon with a convex polygon as an obstacle", "[visibility]")
{
    using namespace geometry;
    std::vector<LineSegment> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } },

        { { -50, 50 }, { 50, 50 } },
        { { 50, 50 }, { 50, 100 } },
        { { 50, 100 }, { -50, 100 } },
        { { -50, 100 }, { -50, 50 } },
    };

    auto poly = VisibilityPolygon({ 0, 0 }, segments);
    REQUIRE(poly.size() == 6);
    REQUIRE(approx_equal(poly[0], { 50, 50 }));
    REQUIRE(approx_equal(poly[1], { 250, 250 }));
    REQUIRE(approx_equal(poly[2], { 250, -250 }));
    REQUIRE(approx_equal(poly[3], { -250, -250 }));
    REQUIRE(approx_equal(poly[4], { -250, 250 }));
    REQUIRE(approx_equal(poly[5], { -50, 50 }));
}

TEST_CASE("Calculate visibility polygon with a concave polygon as an obstacle", "[visibility]")
{
    using namespace geometry;
    std::vector<LineSegment> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } },

        { { -50, 50 },{ 0, 100 } },
        { { 0, 100 }, { 50, 50 } },
        { { 50, 50 },{ 0, 200 } },
        { { 0, 200 },{ -50, 50 } },
    };

    auto poly = VisibilityPolygon({ 0, 0 }, segments);
    REQUIRE(poly.size() == 7);
    REQUIRE(approx_equal(poly[0], { 0, 100 }));
    REQUIRE(approx_equal(poly[1], { 50, 50 }));
    REQUIRE(approx_equal(poly[2], { 250, 250 }));
    REQUIRE(approx_equal(poly[3], { 250, -250 }));
    REQUIRE(approx_equal(poly[4], { -250, -250 }));
    REQUIRE(approx_equal(poly[5], { -250, 250 }));
    REQUIRE(approx_equal(poly[6], { -50, 50 }));
}