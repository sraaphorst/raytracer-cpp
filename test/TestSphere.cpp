/**
 * TestSphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>

#include "affine_transform.h"
#include "constmath.h"
#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("Sphere: Sphere must return a pair of points if ray intersects it") {
    const Sphere s;
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 4);
    REQUIRE(xs[1].getT() == 6);
}

TEST_CASE("Sphere: Sphere must return a pair of equal points if ray intersects at a tangent") {
    const Sphere s;
    const Ray r{make_point(0, 1, -5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 5);
    REQUIRE(xs[1].getT() == 5);
}

TEST_CASE("Sphere: Sphere must return empty vector if a ray does not intersect it") {
    const Sphere s;
    const Ray r{make_point(0, 2, -5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs.empty());
}

TEST_CASE("Sphere: Sphere should return correct values if ray originates inside it") {
    const Sphere s;
    const Ray r{make_point(0, 0, 0), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs[0].getT() == -1);
    REQUIRE(xs[1].getT() ==  1);
}

TEST_CASE("Sphere: Sphere should return correct values if it is behind the ray") {
    const Sphere s;
    const Ray r{make_point(0, 0, 5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs[0].getT() == -6);
    REQUIRE(xs[1].getT() == -4);
}

TEST_CASE("Sphere: Intersecting a scaled sphere with a ray") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    Sphere s;
    s.setTransformation(scale(2, 2, 2));
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 3);
    REQUIRE(xs[1].getT() == 7);
}

TEST_CASE("Sphere: Intersecting a translated sphere with a ray") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    Sphere s;
    s.setTransformation(translation(5, 0, 0));
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 0);
}

TEST_CASE("Sphere: The normal on a sphere at a point on the x-axis") {
    const Sphere s;
    const auto n = s.normalAt(make_point(1, 0, 0));
    REQUIRE(n == make_vector(1, 0, 0));
}

TEST_CASE("Sphere: The normal on a sphere at a point on the y-axis") {
    const Sphere s;
    const auto n = s.normalAt(make_point(0, 1, 0));
    REQUIRE(n == make_vector(0, 1, 0));
}

TEST_CASE("Sphere: The normal on a sphere at a point on the z-axis") {
    const Sphere s;
    const auto n = s.normalAt(make_point(0, 0, 1));
    REQUIRE(n == make_vector(0, 0, 1));
}

TEST_CASE("Sphere: The normal on a sphere at a non-axial point") {
    const Sphere s;
    const auto i = sqrtd(3) / 3;
    const auto n = s.normalAt(make_point(i, i, i));
    REQUIRE(n == make_vector(i, i, i));
}

TEST_CASE("Sphere: The normal is a normalized vector") {
    const Sphere s;
    const auto i = sqrtd(3) / 3;
    const auto n = s.normalAt(make_point(i, i, i));
    REQUIRE(n == n.normalize());
}

TEST_CASE("Sphere: A helper for producing a sphere with a glossy material") {
    const auto sphere_ptr = Sphere::createGlassySphere();
    REQUIRE(sphere_ptr->getTransformation() == predefined_matrices::I<>);
    REQUIRE(sphere_ptr->getMaterial().getTransparency() == 1);
    REQUIRE(sphere_ptr->getMaterial().getRefractiveIndex() == 1.5);
}

TEST_CASE("Sphere: Transparency and Refractive Index for the default material") {
    const Material m{};
    REQUIRE(m.getTransparency() == 0);
    REQUIRE(m.getRefractiveIndex() == 1.0);
}
