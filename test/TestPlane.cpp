/**
 * TestPlane.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "intersection.h"
#include "plane.h"
#include "ray.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("The normal of a plane is constant everywhere") {
    const Plane p;
    const auto n1 = p.normalAt(predefined_tuples::zero_point);
    const auto n2 = p.normalAt(make_point(10, 0, -10));
    const auto n3 = p.normalAt(make_point(-5, 0, 150));
    REQUIRE(n1 == predefined_tuples::y1);
    REQUIRE(n2 == predefined_tuples::y1);
    REQUIRE(n3 == predefined_tuples::y1);
}

TEST_CASE("Intersect with a ray parallel to the plane") {
    const Plane p;
    const Ray ray{make_point(0, 10, 0), predefined_tuples::z1};
    REQUIRE(p.intersect(ray).empty());
}

TEST_CASE("Intersect with a coplanar ray") {
    const Plane p;
    const Ray ray{predefined_tuples::zero_point, predefined_tuples::z1};
    REQUIRE(p.intersect(ray).empty());
}

TEST_CASE("A ray intersecting a plane from above") {
    const Plane p;
    const Ray ray{make_point(0, 1, 0), make_vector(0, -1, 0)};
    const auto xs = p.intersect(ray);
    REQUIRE(xs.size() == 1);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 1));
    REQUIRE(xs[0].getObject() == p);
}

TEST_CASE("A ray intersecting a plane from below") {
    const Plane p;
    const Ray ray{make_point(0, -1, 0), predefined_tuples::y1};
    const auto xs = p.intersect(ray);
    REQUIRE(xs.size() == 1);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 1));
    REQUIRE(xs[0].getObject() == p);
}
