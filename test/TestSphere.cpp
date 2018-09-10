/**
 * TestSphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>
#include "ray.h"
#include "sphere.h"
#include "vector.h"

using namespace raytracer;

/**
 * NOTE: because of constexpr, if the determinant is less than 0, the code will not compile.
 */

TEST_CASE("Sphere must return a pair of points if ray intersects it", "[Sphere][Ray][intersect]") {
    constexpr Sphere s;
    constexpr Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    constexpr auto xs = s.intersect(r);
    REQUIRE(xs.has_value());

    // Note: making this a reference makes the whole thing not constexpr.
    constexpr auto v = xs.value();
    REQUIRE(v[0] == 4);
    REQUIRE(v[1] == 6);
}

TEST_CASE("Sphere must return a pair of equal points if ray intersects at a tangent", "[Sphere][Ray][intersect]") {
    constexpr Sphere s;
    constexpr Ray r{make_point(0, 1, -5), make_vector(0, 0, 1)};
    constexpr auto xs = s.intersect(r);
    REQUIRE(xs.has_value());

    constexpr auto v = xs.value();
    REQUIRE(v[0] == 5);
    REQUIRE(v[1] == 5);
}

TEST_CASE("Sphere must return nullopt if a ray does not intersect it", "[Sphere][Ray][intersection]") {
    constexpr Sphere s;
    constexpr Ray r{make_point(0, 2, -5), make_vector(0, 0, 1)};
    constexpr auto xs = s.intersect(r);
    REQUIRE_FALSE(xs.has_value());
}

TEST_CASE("Sphere should return correct values if ray originates inside it", "[Sphere][Ray][intersection]") {
    constexpr Sphere s;
    constexpr Ray r{make_point(0, 0, 0), make_vector(0, 0, 1)};
    constexpr auto xs = s.intersect(r);
    REQUIRE(xs.has_value());

    constexpr auto v = xs.value();
    REQUIRE(v[0] == -1);
    REQUIRE(v[1] ==  1);
}

TEST_CASE("Sphere should return correct values if it is behind the ray", "[Sphere][Ray][intersect]") {
    constexpr Sphere s;
    constexpr Ray r{make_point(0, 0, 5), make_vector(0, 0, 1)};
    constexpr auto xs = s.intersect(r);
    REQUIRE(xs.has_value());

    constexpr auto v = xs.value();
    REQUIRE(v[0] == -6);
    REQUIRE(v[1] == -4);
}