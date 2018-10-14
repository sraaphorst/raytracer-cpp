/**
 * TestRay.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>
#include <affine_transform.h>

#include "transformers.h"
#include "ray.h"
#include "vector.h"

using namespace raytracer;
using namespace raytracer::transformers;

TEST_CASE("Ray throws an exception if improper arguments are passed to it", "[Ray][constructor]") {
    REQUIRE_THROWS(Ray{predefined_tuples::zero_vector, predefined_tuples::zero_vector});
    REQUIRE_THROWS(Ray{predefined_tuples::zero_point, predefined_tuples::zero_point});
}

TEST_CASE("Ray can calculate its position at time t", "[Ray][position]") {
    Ray ray{make_point(2, 3, 4), make_vector(1, 0, 0)};
    REQUIRE(ray.position( 0 ) == make_point(2,   3, 4));
    REQUIRE(ray.position( 1 ) == make_point(3,   3, 4));
    REQUIRE(ray.position(-1 ) == make_point(1,   3, 4));
    REQUIRE(ray.position(2.5) == make_point(4.5, 3, 4));
}

TEST_CASE("Translating a ray") {
    const Ray r{make_point(1, 2, 3), make_vector(0, 1, 0)};
    const auto m = translation(3, 4, 5);
    const auto r2 = r.transform(m);
    REQUIRE(r2.getOrigin() == make_point(4, 6, 8));
    REQUIRE(r2.getDirection() == make_vector(0, 1, 0));
}

TEST_CASE("Scaling a ray") {
    const Ray r{make_point(1, 2, 3), make_vector(0, 1, 0)};
    const auto m = scale(2, 3, 4);
    const auto r2 = r.transform(m);
    REQUIRE(r2.getOrigin() == make_point(2, 6, 12));
    REQUIRE(r2.getDirection() == make_vector(0, 3, 0));
}
