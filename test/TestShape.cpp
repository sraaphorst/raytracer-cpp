/**
 * TestShape.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "TestShape.h"

#include "affine_transform.h"
#include "material.h"
#include "matrix.h"

using namespace raytracer;

TEST_CASE("The default transformation") {
    const TestShape s;
    REQUIRE(s.getTransformation() == predefined_matrices::I<>);
}

TEST_CASE("Assigning a transformation") {
    TestShape s;
    s.setTransformation(translation(2, 3, 4));
    REQUIRE(s.getTransformation() == translation(2, 3, 4));
}

TEST_CASE("The default material") {
    const TestShape s;
    REQUIRE(s.getMaterial() == Material{});
}

TEST_CASE("Assigning a material") {
    TestShape s;
    Material m;
    m.setAmbient(1.0);
    s.setMaterial(m);

    Material m2;
    m2.setAmbient(1.0);

    REQUIRE(s.getMaterial() == m2);
    REQUIRE_FALSE(s.getMaterial() == Material{});
}
