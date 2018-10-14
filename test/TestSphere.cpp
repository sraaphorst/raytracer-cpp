/**
 * TestSphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>

#include "affine_transform.h"
#include "constmath.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("Sphere must return a pair of points if ray intersects it", "[Sphere][Ray][intersect]") {
    const Sphere s;
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 4);
    REQUIRE(xs[1].getT() == 6);
}

TEST_CASE("Sphere must return a pair of equal points if ray intersects at a tangent", "[Sphere][Ray][intersect]") {
    const Sphere s;
    const Ray r{make_point(0, 1, -5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 5);
    REQUIRE(xs[1].getT() == 5);
}

TEST_CASE("Sphere must return empty vector if a ray does not intersect it", "[Sphere][Ray][intersection]") {
    const Sphere s;
    const Ray r{make_point(0, 2, -5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs.empty());
}

TEST_CASE("Sphere should return correct values if ray originates inside it", "[Sphere][Ray][intersection]") {
    const Sphere s;
    const Ray r{make_point(0, 0, 0), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs[0].getT() == -1);
    REQUIRE(xs[1].getT() ==  1);
}

TEST_CASE("Sphere should return correct values if it is behind the ray", "[Sphere][Ray][intersect]") {
    const Sphere s;
    const Ray r{make_point(0, 0, 5), make_vector(0, 0, 1)};
    const auto xs = s.intersect(r);
    REQUIRE(xs[0].getT() == -6);
    REQUIRE(xs[1].getT() == -4);
}

SCENARIO("A sphere's default translation") {
    GIVEN("s is a sphere") {
        const Sphere s;
        THEN("s.tranform is the identity matrix") {
            REQUIRE(s.getTransformation() == predefined_matrices::I<double, 4>);
        }
    }
}

SCENARIO("Changing a sphere's transformation") {
    GIVEN("s is a mutable sphere") {
        Sphere s;
        GIVEN("t is the translation(2, 3, 4)") {
            const auto t = translation(2, 3, 4);
            WHEN("t is set to the transformation of s") {
                s.setTransformation(t);
                THEN("s.transform == t") {
                    REQUIRE(s.getTransformation() == t);
                }
            }
        }
    }
}

TEST_CASE("Intersecting a scaled sphere with a ray") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    Sphere s;
    s.setTransformation(scale(2, 2, 2));
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getT() == 3);
    REQUIRE(xs[1].getT() == 7);
}

TEST_CASE("Intersecting a translated sphere with a ray") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    Sphere s;
    s.setTransformation(translation(5, 0, 0));
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 0);
}

TEST_CASE("The normal on a sphere at a point on the x-axis") {
    const Sphere s;
    const auto n = s.normalAt(make_point(1, 0, 0));
    REQUIRE(n == make_vector(1, 0, 0));
}

TEST_CASE("The normal on a sphere at a point on the y-axis") {
    const Sphere s;
    const auto n = s.normalAt(make_point(0, 1, 0));
    REQUIRE(n == make_vector(0, 1, 0));
}

TEST_CASE("The normal on a sphere at a point on the z-axis") {
    const Sphere s;
    const auto n = s.normalAt(make_point(0, 0, 1));
    REQUIRE(n == make_vector(0, 0, 1));
}

TEST_CASE("The normal on a sphere at a non-axial point") {
    const Sphere s;
    const auto i = sqrtd(3) / 3;
    const auto n = s.normalAt(make_point(i, i, i));
    REQUIRE(n == make_vector(i, i, i));
}

TEST_CASE("The normal is a normalized vector") {
    const Sphere s;
    const auto i = sqrtd(3) / 3;
    const auto n = s.normalAt(make_point(i, i, i));
    REQUIRE(n == n.normalize());
}

TEST_CASE("Computing the normal on a translated sphere") {
    const Sphere s{translation(0, 1, 0)};
    const auto n = s.normalAt(make_point(0, 1.70711, -0.70711));
    REQUIRE(n == make_vector(0, .70711, -0.70711));
}

TEST_CASE("Sphere has a default material") {
    const Sphere s;
    REQUIRE(s.getMaterial() == Material{});
}

TEST_CASE("A sphere may be assigned a material") {
    Sphere s;
    Material m;
    m.setAmbient(1);
    s.setMaterial(m);
    REQUIRE(s.getMaterial() == m);
}
