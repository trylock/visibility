#include "catch.hpp"

#include <visibility/visibility.hpp>

using vector_type = geometry::vec2;
using segment_type = geometry::line_segment<vector_type>;
using segment_comparer_type = geometry::line_segment_dist_comparer<vector_type>;
using angle_comparer_type = geometry::angle_comparer<vector_type>;

void test_line_segment_is_closer(
    const segment_comparer_type& cmp,
    vector_type a, vector_type b,
    vector_type c, vector_type d)
{
    REQUIRE(cmp(segment_type{ a, b }, { c, d }));
    REQUIRE(cmp(segment_type{ b, a }, { c, d }));
    REQUIRE(cmp(segment_type{ a, b }, { d, c }));
    REQUIRE(cmp(segment_type{ b, a }, { d, c }));

    REQUIRE_FALSE(cmp(segment_type{ c, d }, { a, b }));
    REQUIRE_FALSE(cmp(segment_type{ d, c }, { a, b }));
    REQUIRE_FALSE(cmp(segment_type{ c, d }, { b, a }));
    REQUIRE_FALSE(cmp(segment_type{ d, c }, { b, a }));
}

void test_line_segments_are_equal(
    const segment_comparer_type& cmp,
    vector_type a, vector_type b,
    vector_type c, vector_type d)
{
    REQUIRE_FALSE(cmp(segment_type{ a, b }, { c, d }));
    REQUIRE_FALSE(cmp(segment_type{ b, a }, { c, d }));
    REQUIRE_FALSE(cmp(segment_type{ a, b }, { d, c }));
    REQUIRE_FALSE(cmp(segment_type{ b, a }, { d, c }));

    REQUIRE_FALSE(cmp(segment_type{ c, d }, { a, b }));
    REQUIRE_FALSE(cmp(segment_type{ d, c }, { a, b }));
    REQUIRE_FALSE(cmp(segment_type{ c, d }, { b, a }));
    REQUIRE_FALSE(cmp(segment_type{ d, c }, { b, a }));
}

TEST_CASE("Compare 2 line segments with no common endpoints", "[visibility][dist_comp]")
{
    segment_comparer_type cmp{ { 0, 0 } };

    test_line_segment_is_closer(cmp, { 1, 1 }, { 1, -1 }, { 2, 1 }, { 2, -1 });
    test_line_segment_is_closer(cmp, { 1, 1 }, { 1, -1 }, { 2, 2 }, { 2, 3 });
}

TEST_CASE("Compare 2 line segments with common endpoints", "[visibility][dist_comp]")
{
    segment_comparer_type cmp{ { 0, 0 } };

    test_line_segments_are_equal(cmp, { 1, 1 }, { 1, 0 }, { 1, 0 }, { 1, -1 });
    test_line_segments_are_equal(cmp, { 1, 1 }, { 1, 0 }, { 1, 0 }, { 1, 1 });
    test_line_segment_is_closer(cmp, { 2, 0 }, { 1, 1 }, { 2, 1 }, { 2, 0 });
    test_line_segment_is_closer(cmp, { 2, 1 }, { 2, 0 }, { 2, 0 }, { 3, 1 });
}

TEST_CASE("Compare angle with 2 points in general position", "[visibility][angle_comp]")
{
    angle_comparer_type cmp{ { 0, 0 } };

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
    angle_comparer_type cmp{ { 0, 0 } };

    REQUIRE(cmp({ 1, 0 }, { 2, 0 }));
    REQUIRE_FALSE(cmp({ 2, 0 }, { 1, 0 }));

    REQUIRE_FALSE(cmp({ 1, 0 }, { 1, 0 }));
    REQUIRE_FALSE(cmp({ 0, 0 }, { 0, 0 }));
}

TEST_CASE("Calculate visibility polygon with no line segments", "[visibility]")
{
    std::vector<segment_type> segments;
    auto poly = geometry::visibility_polygon(vector_type{ 0, 0 }, segments.begin(), segments.end());
    REQUIRE(poly.size() == 0);
}

TEST_CASE("Calculate visibility polygon with no obstaces apart from the boundary", "[visibility]")
{
    using namespace geometry;
    std::vector<segment_type> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } }
    };

    auto poly = visibility_polygon(vector_type{ 0, 0 }, segments.begin(), segments.end());
    REQUIRE(poly.size() == 4);
    REQUIRE(approx_equal(poly[0], { 250, 250 }));
    REQUIRE(approx_equal(poly[1], { 250, -250 }));
    REQUIRE(approx_equal(poly[2], { -250, -250 }));
    REQUIRE(approx_equal(poly[3], { -250, 250 }));
}

TEST_CASE("Calculate visibility polygon with a polyline as an obstacle", "[visibility]")
{
    using namespace geometry;
    std::vector<segment_type> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } },

        { { -50, 50 },{ 50, 50 } },
        { { 50, 50 },{ 50, -50 } },
    };

    auto poly = visibility_polygon(vector_type{ 0, 0 }, segments.begin(), segments.end());
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
    std::vector<segment_type> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } },

        { { -50, 50 }, { 50, 50 } },
        { { 50, 50 }, { 50, 100 } },
        { { 50, 100 }, { -50, 100 } },
        { { -50, 100 }, { -50, 50 } },
    };

    auto poly = visibility_polygon(vector_type{ 0, 0 }, segments.begin(), segments.end());
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
    std::vector<segment_type> segments{
        { { -250, -250 },{ -250, 250 } },
        { { -250, 250 },{ 250, 250 } },
        { { 250, 250 },{ 250, -250 } },
        { { 250, -250 },{ -250, -250 } },

        { { -50, 50 },{ 0, 100 } },
        { { 0, 100 }, { 50, 50 } },
        { { 50, 50 },{ 0, 200 } },
        { { 0, 200 },{ -50, 50 } },
    };

    auto poly = visibility_polygon(vector_type{ 0, 0 }, segments.begin(), segments.end());
    REQUIRE(poly.size() == 7);
    REQUIRE(approx_equal(poly[0], { 0, 100 }));
    REQUIRE(approx_equal(poly[1], { 50, 50 }));
    REQUIRE(approx_equal(poly[2], { 250, 250 }));
    REQUIRE(approx_equal(poly[3], { 250, -250 }));
    REQUIRE(approx_equal(poly[4], { -250, -250 }));
    REQUIRE(approx_equal(poly[5], { -250, 250 }));
    REQUIRE(approx_equal(poly[6], { -50, 50 }));
}