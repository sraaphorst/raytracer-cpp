/**
 * TestBounds.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include <catch.hpp>

#include "common.h"
#include "vec.h"
#include "bounding_box.h"

using namespace raytracer;

TEST_CASE("BoundingBox: Creating an empty BoundingBox") {
    constexpr BoundingBox box{};
    REQUIRE(box.getMinPoint() == predefined_tuples::min_point);
    REQUIRE(box.getMaxPoint() == predefined_tuples::max_point);
}

TEST_CASE("BoundingBox: Create a BoundingBow with volume") {
    constexpr BoundingBox box{make_point(-1, -2, -3), make_point(3, 2, 1)};
    REQUIRE(box.getMinPoint() == make_point(-1, -2, -3));
    REQUIRE(box.getMaxPoint() == make_point(3, 2, 1));
}

TEST_CASE("BoundingBox: Adding points to an empty BoundingBox") {
    BoundingBox box{};
    box.addPoint(make_point(-5, 2, 0));
    box.addPoint(make_point(7, 0, -3));
    REQUIRE(box.getMinPoint() == make_point(-5, 0, -3));
    REQUIRE(box.getMaxPoint() == make_point(7, 2, 0));
}