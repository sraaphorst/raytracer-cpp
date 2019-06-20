/**
 * TestSphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>

#include "affine_transform.h"
#include "bounding_box.h"
#include "constmath.h"
#include "impl/intersection.h"
#include "impl/ray.h"
#include "shapes/sphere.h"
#include "vec.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Sphere: Sphere must return a pair of points if ray intersects it") {
    const auto s = Sphere::createSphere();
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const auto xs = s->intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 4);
    REQUIRE(xs[1].getT() == 6);
}

TEST_CASE("Sphere: Sphere must return a pair of equal points if ray intersects at a tangent") {
    const auto s = Sphere::createSphere();
    const Ray r{make_point(0, 1, -5), make_vector(0, 0, 1)};
    const auto xs = s->intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 5);
    REQUIRE(xs[1].getT() == 5);
}

TEST_CASE("Sphere: Sphere must return empty vector if a ray does not intersect it") {
    const auto s = Sphere::createSphere();
    const Ray r{make_point(0, 2, -5), make_vector(0, 0, 1)};
    const auto xs = s->intersect(r);
    REQUIRE(xs.empty());
}

TEST_CASE("Sphere: Sphere should return correct values if ray originates inside it") {
    const auto s = Sphere::createSphere();
    const Ray r{make_point(0, 0, 0), make_vector(0, 0, 1)};
    const auto xs = s->intersect(r);
    REQUIRE(xs[0].getT() == -1);
    REQUIRE(xs[1].getT() ==  1);
}

TEST_CASE("Sphere: Sphere should return correct values if it is behind the ray") {
    const auto s = Sphere::createSphere();
    const Ray r{make_point(0, 0, 5), make_vector(0, 0, 1)};
    const auto xs = s->intersect(r);
    REQUIRE(xs[0].getT() == -6);
    REQUIRE(xs[1].getT() == -4);
}

TEST_CASE("Sphere: Intersecting a scaled sphere with a ray") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    auto s = Sphere::createSphere();
    s->setTransformation(scale(2, 2, 2));
    const auto xs = s->intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 3);
    REQUIRE(xs[1].getT() == 7);
}

TEST_CASE("Sphere: Intersecting a translated sphere with a ray") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    auto s = Sphere::createSphere();
    s->setTransformation(translation(5, 0, 0));
    const auto xs = s->intersect(r);
    REQUIRE(xs.size() == 0);
}

TEST_CASE("Sphere: The normal on a sphere at a point on the x-axis") {
    const auto s = Sphere::createSphere();
    const auto n = s->normalAt(make_point(1, 0, 0));
    REQUIRE(n == make_vector(1, 0, 0));
}

TEST_CASE("Sphere: The normal on a sphere at a point on the y-axis") {
    const auto s = Sphere::createSphere();
    const auto n = s->normalAt(make_point(0, 1, 0));
    REQUIRE(n == make_vector(0, 1, 0));
}

TEST_CASE("Sphere: The normal on a sphere at a point on the z-axis") {
    const auto s = Sphere::createSphere();
    const auto n = s->normalAt(make_point(0, 0, 1));
    REQUIRE(n == make_vector(0, 0, 1));
}

TEST_CASE("Sphere: The normal on a sphere at a non-axial point") {
    const auto s = Sphere::createSphere();
    const auto i = const_sqrtd(3) / 3;
    const auto n = s->normalAt(make_point(i, i, i));
    REQUIRE(n == make_vector(i, i, i));
}

TEST_CASE("Sphere: The normal is a normalized vector") {
    const auto s = Sphere::createSphere();
    const auto i = const_sqrtd(3) / 3;
    const auto n = s->normalAt(make_point(i, i, i));
    REQUIRE(n == n.normalize());
}

TEST_CASE("Sphere: Computing the normal on a translated sphere") {
    auto s = Sphere::createSphere();
    s->setTransformation(translation(0, 1, 0));
    auto n = s->normalAt(make_point(0, 1.70711, -0.70711));
    REQUIRE(n == make_vector(0, 0.70711, -0.70711));
}

TEST_CASE("Sphere: Computing the normal on a transformed sphere") {
    auto s = Sphere::createSphere();
    s->setTransformation(scale(1, 0.5, 1) * rotation_z(math_constants::pi<> / 5));
    auto n = s->normalAt(make_point(0, math_constants::sqrt2_by_2, -math_constants::sqrt2_by_2));
    REQUIRE(n == make_vector(0, 0.97014, -0.24254));
}

TEST_CASE("Sphere: A helper for producing a sphere with a glossy material") {
    const auto s = Sphere::createGlassSphere();
    REQUIRE(s->getTransformation() == predefined_matrices::I<>);
    REQUIRE(s->getMaterial()->getTransparency() == 1);
    REQUIRE(s->getMaterial()->getRefractiveIndex() == 1.5);
}

TEST_CASE("Sphere: Transparency and Refractive Index for the default material") {
    const auto s = Sphere::createSphere();
    const Material m{};
    REQUIRE(s->getMaterial()->getTransparency() == 0);
    REQUIRE(s->getMaterial()->getRefractiveIndex() == 1.0);
}

TEST_CASE("Sphere: A sphere has a bounding box") {
    const auto s = Sphere::createSphere();
    const auto box = s->boundsOf();
    REQUIRE(box.getMinPoint() == make_point(-1, -1, -1));
    REQUIRE(box.getMaxPoint() == make_point(1, 1, 1));
}