/**
 * TestShape.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "TestShape.h"

#include "affine_transform.h"
#include "constmath.h"
#include "material.h"
#include "matrix.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Shape: The default transformation") {
    const TestShape s;
    REQUIRE(s.getTransformation() == predefined_matrices::I<>);
}

TEST_CASE("Shape: Assigning a transformation") {
    TestShape s;
    s.setTransformation(translation(2, 3, 4));
    REQUIRE(s.getTransformation() == translation(2, 3, 4));
}

TEST_CASE("Shape: The default material") {
    const TestShape s;
    REQUIRE(s.getMaterial() == Material{});
}

TEST_CASE("Shape: Assigning a material") {
    TestShape s;
    Material m;
    m.setAmbient(1.0);
    s.setMaterial(m);

    Material m2;
    m2.setAmbient(1.0);

    REQUIRE(s.getMaterial() == m2);
    REQUIRE_FALSE(s.getMaterial() == Material{});
}

TEST_CASE("Shape: Intersecting a scaled shape with a ray") {
    const TestShape s{scale(2, 2, 2)};
    s.intersect(Ray{make_point(0, 0, -5), predefined_tuples::z1});
    REQUIRE(s.saved_ray.getOrigin() == make_point(0, 0, -2.5));
    REQUIRE(s.saved_ray.getDirection() == make_vector(0, 0, 0.5));
}

TEST_CASE("Shape: Intersecting a translated shape with a ray") {
    const TestShape s{translation(5, 0, 0)};
    s.intersect(Ray{make_point(0, 0, -5), predefined_tuples::z1});
    REQUIRE(s.saved_ray.getOrigin() == make_point(-5, 0, -5));
    REQUIRE(s.saved_ray.getDirection() == predefined_tuples::z1);
}

TEST_CASE("Shape: Computing the normal on a translated shape") {
    const TestShape s{translation(0, 1, 0)};
    const auto n = s.normalAt(make_point(0, 1.70711, -0.70711));
    REQUIRE(n == make_vector(0, 0.70711, -0.70711));
}

TEST_CASE("Shape: Computing the normal on a scaled shape") {
    const TestShape s{scale(1.0, 0.5, 1.0)};
    const auto sqrt2_by_2 = sqrtd(2) / 2;
    const auto n = s.normalAt(make_point(0, sqrt2_by_2, -sqrt2_by_2));
    REQUIRE(n == make_vector(0, 0.97014, -0.24254));
}
