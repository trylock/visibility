#include "catch.hpp"
#include <visibility/Primitives.hpp>

TEST_CASE("Calculate orientation of 3 points in a plane", "[primitives]")
{
    using namespace geometry;

    REQUIRE(orientation(vec2{ 0, 0 }, vec2{ 1, 0 }, vec2{ 2, 1 }) == Orientation::LeftTurn);
    REQUIRE(orientation(vec2{ 0, 0 }, vec2{ 1, 0 }, vec2{ 2, -1 }) == Orientation::RightTrun);
    REQUIRE(orientation(vec2{ 0, 0 }, vec2{ 1, 0 }, vec2{ 2, 0 }) == Orientation::Collinear);
    REQUIRE(orientation(vec2{ 0, 0 }, vec2{ 0, 0 }, vec2{ 4, 5 }) == Orientation::Collinear);
    REQUIRE(orientation(vec2{ 0, 0 }, vec2{ 0, 0 }, vec2{ 0, 0 }) == Orientation::Collinear);
}

TEST_CASE("Calculate an intersection point of ray and line segment", "[primitives]")
{
    using namespace geometry;

    vec2 point;
    auto intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ -1, 1 }, vec2{ -1, -1 } }, point);
    REQUIRE_FALSE(intersects);

    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ -1e-3f, 1 }, vec2{ -1e-3f, -1 } }, point);
    REQUIRE_FALSE(intersects);

    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ -2, 0 }, vec2{ -1, 0 } }, point);
    REQUIRE_FALSE(intersects);

    // ray intersects the line segment in its origin
    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ 0, 1 }, vec2{ 0, -1 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 0);
    REQUIRE(point[1] == 0);

    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ -1, 0 }, vec2{ 0, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 0);
    REQUIRE(point[1] == 0);

    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ 0, 0 }, vec2{ -1, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 0);
    REQUIRE(point[1] == 0);

    // ray intersects the line segment in the middle
    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ 2, 1 }, vec2{ 2, -1 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 2);
    REQUIRE(point[1] == 0);

    // ray intersects the line segment in one of its endpoints
    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ 2, 0 }, vec2{ 3, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 2);
    REQUIRE(point[1] == 0);

    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ 3, 0 }, vec2{ 2, 0 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 2);
    REQUIRE(point[1] == 0);

    intersects = Ray{ vec2{ 0, 0 }, vec2{ 1, 0 } }.Intersects({ vec2{ 1, 0 }, vec2{ 1, -1 } }, point);
    REQUIRE(intersects);
    REQUIRE(point[0] == 1);
    REQUIRE(point[1] == 0);
}