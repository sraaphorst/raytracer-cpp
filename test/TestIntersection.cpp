/**
 * TestIntersection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "intersection.h"
#include "sphere.h"

using namespace raytracer;

TEST_CASE("Intersection can be created and initialized", "[Intersection][constructor]") {
    constexpr Sphere s;
    constexpr Intersection<Sphere> it{0.5, s};
    constexpr auto s2 = it.getObject().actual();
    REQUIRE(it.getObject() == s);
    REQUIRE(it.getT() == 0.5);
}