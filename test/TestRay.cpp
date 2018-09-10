/**
 * TestRay.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "ray.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("Ray throws an exception if improper arguments are passed to it", "[Ray][constructor]") {
    REQUIRE_THROWS(Ray{predefined_tuples::zero_vector, predefined_tuples::zero_vector});
    REQUIRE_THROWS(Ray{predefined_tuples::zero_point, predefined_tuples::zero_point});
    REQUIRE_THROWS(Ray{predefined_tuples::zero_point, predefined_tuples::zero_vector, predefined_tuples::zero_vector});
}

TEST_CASE("Ray can calculate its position at time t", "[Ray][position]") {
    constexpr Ray ray{make_point(2, 3, 4), make_vector(1, 0, 0)};

    // Make sure position is constexpr.
    constexpr Tuple t = ray.position(1);
    REQUIRE(ray.position( 0 ) == make_point(2,   3, 4));
    REQUIRE(ray.position( 1 ) == make_point(3,   3, 4));
    REQUIRE(ray.position(-1 ) == make_point(1,   3, 4));
    REQUIRE(ray.position(2.5) == make_point(4.5, 3, 4));
}
