#include "catch.hpp"

#include <visibility/vector2.hpp>

TEST_CASE("Create vector", "[vector]")
{
    using namespace geometry;

    vec2 result(1);
    REQUIRE(result.x == 1);
    REQUIRE(result.y == 1);

    result = vec2{ 3, 4 };
    REQUIRE(result.x == 3);
    REQUIRE(result.y == 4);
}

TEST_CASE("Add vectors", "[vector]")
{
    using namespace geometry;

    vec2 a{ 1, 2 }, b{ 3, 4 };
    REQUIRE((a + b).x == 4);
    REQUIRE((a + b).y == 6);

    a += b;
    REQUIRE(a.x == 4);
    REQUIRE(a.y == 6);
}

TEST_CASE("Subtract vectors", "[vector]")
{
    using namespace geometry;

    vec2 a{ 1, 2 }, b{ 3, 4 };
    REQUIRE((a - b).x == -2);
    REQUIRE((a - b).y == -2);

    a -= b;
    REQUIRE(a.x == -2);
    REQUIRE(a.y == -2);
}

TEST_CASE("Multiply vector", "[vector]")
{
    using namespace geometry;

    vec2 a{ 1, 2 };
    REQUIRE((a * 3 ).x == 3);
    REQUIRE((a * 3 ).y == 6);

    a *= 3;
    REQUIRE(a.x == 3);
    REQUIRE(a.y == 6);
}

TEST_CASE("Divide vector", "[vector]")
{
    using namespace geometry;

    vec2 a{ 2, 8 };
    REQUIRE((a / 2).x == 1);
    REQUIRE((a / 2).y == 4);

    a /= 2;
    REQUIRE(a.x == 1);
    REQUIRE(a.y == 4);
}

TEST_CASE("Negate vector", "[vector]")
{
    using namespace geometry;

    vec2 a{ 2, 8 };
    REQUIRE((-a).x == -2);
    REQUIRE((-a).y == -8);
}

TEST_CASE("Compare 2 vectors", "[vector]")
{
    using namespace geometry;

    vec2 a{ 1, 2 }, b{ 1, 2 };
    REQUIRE(a == b);
    REQUIRE_FALSE(a != b);

    b.x = 0;
    REQUIRE_FALSE(a == b);
    REQUIRE(a != b);
}

TEST_CASE("Calculate a dot product", "[vector]")
{
    using namespace geometry;

    REQUIRE(dot(vec2{ 1, 2 }, vec2{ 3, 4 }) == 11);
    REQUIRE(dot(vec2{ 1, 2 }, vec2{ 0, 0 }) == 0);
}

TEST_CASE("Calculate squared length of a vector", "[vector]")
{
    using namespace geometry;

    REQUIRE(length_squared(vec2{ 3, 4 }) == 25);
    REQUIRE(length_squared(vec2{ 0, 0 }) == 0);
}

TEST_CASE("Calculate square of an euclidean distance of 2 points", "[vector]")
{
    using namespace geometry;

    REQUIRE(distance_squared(vec2{ 3, 4 }, vec2{ 0, 1 }) == 18);
    REQUIRE(distance_squared(vec2{ 3, 4 }, vec2{ 3, 4 }) == 0);
}

TEST_CASE("Calculate a 2D normal vector", "[vector]")
{
    using namespace geometry;

    vec2 a{ 3, 4 };
    auto perp = normal(a);
    REQUIRE(perp.x == -4);
    REQUIRE(perp.y == 3);
}

TEST_CASE("Calculate a determinant", "[vector]")
{
    using namespace geometry;

    vec2 a{ 3, 4 }, b{ 1, 2};
    auto det = cross(a, b);
    REQUIRE(det == 2);
}

TEST_CASE("Normalize a floating point vector", "[vector]")
{
    using namespace geometry;

    vec2 a{ 3, 4 };
    auto normalized = normalize(a);
    REQUIRE(length_squared(normalized) == Approx(1));

    vec2 zero{ 0, 0 };
    normalized = normalize(zero);
    REQUIRE(normalized.x == 0);
    REQUIRE(normalized.y == 0);
}