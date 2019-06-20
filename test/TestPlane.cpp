/**
 * TestPlane.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "bounding_box.h"
#include "constmath.h"
#include "intersection.h"
#include "plane.h"
#include "ray.h"
#include "vec.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Plane: The normal of a plane is constant everywhere") {
    const auto p = Plane::createPlane();
    const auto n1 = p->normalAt(predefined_tuples::zero_point);
    const auto n2 = p->normalAt(make_point(10, 0, -10));
    const auto n3 = p->normalAt(make_point(-5, 0, 150));
    REQUIRE(n1 == predefined_tuples::y1);
    REQUIRE(n2 == predefined_tuples::y1);
    REQUIRE(n3 == predefined_tuples::y1);
}

TEST_CASE("Plane: Intersect with a ray parallel to the plane") {
    const auto p = Plane::createPlane();
    const Ray ray{make_point(0, 10, 0), predefined_tuples::z1};
    REQUIRE(p->intersect(ray).empty());
}

TEST_CASE("Plane: Intersect with a coplanar ray") {
    const auto p = Plane::createPlane();
    const Ray ray{predefined_tuples::zero_point, predefined_tuples::z1};
    REQUIRE(p->intersect(ray).empty());
}

TEST_CASE("Plane: A ray intersecting a plane from above") {
    const auto p = Plane::createPlane();
    const Ray ray{make_point(0, 1, 0), make_vector(0, -1, 0)};
    const auto xs = p->intersect(ray);
    REQUIRE(xs.size() == 1);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 1));
    REQUIRE(xs[0].getObject() == p);
}

TEST_CASE("Plane: A ray intersecting a plane from below") {
    const auto p = Plane::createPlane();
    const Ray ray{make_point(0, -1, 0), predefined_tuples::y1};
    const auto xs = p->intersect(ray);
    REQUIRE(xs.size() == 1);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 1));
    REQUIRE(xs[0].getObject() == p);
}

TEST_CASE("Plane: A plane has a bounding box") {
    const auto p = Plane::createPlane();
    const auto box = p->bounds();
    REQUIRE(box.getMinPoint() == make_point(math_constants::ninf<>, 0, math_constants::ninf<>));
    REQUIRE(box.getMaxPoint() == make_point(math_constants::inf<>, 0, math_constants::inf<>));

}