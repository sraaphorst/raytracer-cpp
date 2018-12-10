/**
 * TestGroup.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <numeric>
#include <vector>

#include "intersection.h"
#include "ray.h"
#include "vec.h"

// A necessary evil to test the private members.
#define private public
#include "group.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Group: Creating a new group") {
    const auto g = Group::createGroup();
    REQUIRE(g->getShapes().empty());
    REQUIRE(g->getTransformation() == predefined_matrices::I<double, 4>);
}