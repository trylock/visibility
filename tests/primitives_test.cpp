#include "catch.hpp"

#include <visibility/primitives.hpp>

using ray_type = geometry::ray<geometry::vec2>;
using segment_type = geometry::line_segment<geometry::vec2>;

TEST_CASE("Calculate orientation of 3 points in a plane", "[primitives]")
{
    using namespace geometry;

    REQUIRE(compute_orientation(vec2{ 0, 0 }, vec2{ 1, 0 }, vec2{ 2, 1 }) == orientation::left_turn);
    REQUIRE(compute_orientation(vec2{ 0, 0 }, vec2{ 1, 0 }, vec2{ 2, -1 }) == orientation::right_turn);
    REQUIRE(compute_orientation(vec2{ 0, 0 }, vec2{ 1, 0 }, vec2{ 2, 0 }) == orientation::collinear);
    REQUIRE(compute_orientation(vec2{ 0, 0 }, vec2{ 0, 0 }, vec2{ 4, 5 }) == orientation::collinear);
    REQUIRE(compute_orientation(vec2{ 0, 0 }, vec2{ 0, 0 }, vec2{ 0, 0 }) == orientation::collinear);
}

TEST_CASE("Calculate an intersection point of ray and line segment", "[primitives]")
{
    using namespace geometry;

    vec2 point;
    auto intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ -1, 1 }, vec2{ -1, -1 } }, point);
    REQUIRE_FALSE(intersects);

    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ -1e-3f, 1 }, vec2{ -1e-3f, -1 } }, point);
    REQUIRE_FALSE(intersects);

    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ -2, 0 }, vec2{ -1, 0 } }, point);
    REQUIRE_FALSE(intersects);

    // ray intersects the line segment in its origin
    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ 0, 1 }, vec2{ 0, -1 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ -1, 0 }, vec2{ 0, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ 0, 0 }, vec2{ -1, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 0);
    REQUIRE(point.y == 0);

    // ray intersects the line segment in the middle
    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ 2, 1 }, vec2{ 2, -1 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 2);
    REQUIRE(point.y == 0);

    // ray intersects the line segment in one of its endpoints
    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ 2, 0 }, vec2{ 3, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 2);
    REQUIRE(point.y == 0);

    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ 3, 0 }, vec2{ 2, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 2);
    REQUIRE(point.y == 0);

    intersects = ray_type{ vec2{ 0, 0 }, vec2{ 1, 0 } }
        .intersects(segment_type{ vec2{ 1, 0 }, vec2{ 1, -1 } }, point);
    REQUIRE(intersects);
    REQUIRE(point.x == 1);
    REQUIRE(point.y == 0);
}