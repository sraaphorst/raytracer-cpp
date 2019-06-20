/**
 * TestCylinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <numeric>
#include <vector>

#include "bounding_box.h"
#include "constmath.h"
#include "intersection.h"
#include "ray.h"
#include "vec.h"

// A necessary evil to test the private members.
#define private public
#include "cylinder.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Cylinder: A ray intersecting a cylinder") {
    const auto c = Cylinder::createCylinder();

    std::array<Tuple, 3> origins{make_point(  1, 0, -5),
                                 make_point(  0, 0, -5),
                                 make_point(0.5, 0, -5)};

    std::array<Tuple, 3> directions{predefined_tuples::z1,
                                    predefined_tuples::z1,
                                    make_vector(0.1, 1, 1)};

    std::array<double, 3> t0s{5, 4, 6.80798};
    std::array<double, 3> t1s{5, 6, 7.08872};

    for (size_t i = 0; i < 3; ++i) {
        const auto &origin = origins[i];
        const auto direction = directions[i].normalize();
        const auto t0 = t0s[i];
        const auto t1 = t1s[i];

        const Ray ray{origin, direction};
        const auto xs = c->localIntersection(ray);
        REQUIRE(xs.size() == 2);
        REQUIRE(ALMOST_EQUALS(xs[0].getT(), t0));
        REQUIRE(ALMOST_EQUALS(xs[1].getT(), t1));
    }

}

TEST_CASE("Cylinder: A ray misses a cylinder") {
    const auto c = Cylinder::createCylinder();

    std::array<Tuple, 3> origins{make_point(1, 0, 0),
                                 predefined_tuples::zero_point,
                                 make_point(0, 0, -5)};

    std::array<Tuple, 3> directions{predefined_tuples::y1,
                                    predefined_tuples::y1,
                                    make_vector(1, 1, 1)};

    for (size_t i = 0; i < 3; ++i) {
        const auto &origin = origins[i];
        const auto direction = directions[i].normalize();

        const Ray ray{origin, direction};
        const auto xs = c->localIntersection(ray);
        REQUIRE(xs.empty());
    }
}

TEST_CASE("Cylinder: The normal vector on a cylinder") {
    const auto c = Cylinder::createCylinder();

    constexpr std::array<Tuple, 4> origins{make_point( 1,  0,  0),
                                           make_point( 0,  5, -1),
                                           make_point( 0, -2,  1),
                                           make_point(-1,  1,  0)};
    constexpr std::array<Tuple, 4> normals{predefined_tuples::x1,
                                           make_vector( 0,  0, -1),
                                           predefined_tuples::z1,
                                           make_vector(-1, 0,  0)};

    for (size_t i = 0; i < 4; ++i) {
        const auto &origin = origins[i];
        const auto &normal = c->localNormalAt(origin);
        REQUIRE(normal == normals[i]);
    }
}

TEST_CASE("Cylinder: The default minimum and maximum for a cylinder") {
    const auto c = Cylinder::createCylinder();
    REQUIRE(c->getMinimumY() == -std::numeric_limits<double>::infinity());
    REQUIRE(c->getMaximumY() ==  std::numeric_limits<double>::infinity());
}

TEST_CASE("Cylinder: Intersecting a constrained cylinder") {
    auto c = Cylinder::createCylinder();
    c->setMinimumY(1);
    c->setMaximumY(2);

    constexpr std::array<Tuple, 6> points{make_point(0, 1.5,  0),
                                          make_point(0, 3,   -5),
                                          make_point(0, 0,   -5),
                                          make_point(0, 2,   -5),
                                          make_point(0, 1,   -5),
                                          make_point(0, 1.5,  2)};
    constexpr std::array<Tuple, 6> directions{make_vector(0.1, 1, 0),
                                              predefined_tuples::z1,
                                              predefined_tuples::z1,
                                              predefined_tuples::z1,
                                              predefined_tuples::z1,
                                              predefined_tuples::z1};

    constexpr std::array<int, 6> count{0, 0, 0, 0, 0, 2};

    for (size_t i = 0; i < 6; ++i) {
        const auto direction = directions[i].normalize();
        const Ray ray{points[i], direction};
        const auto xs = c->localIntersection(ray);
        REQUIRE(xs.size() == count[i]);
    }
}

TEST_CASE("Cylinder: The default capped value for a cylinder") {
    const auto c = Cylinder::createCylinder();
    REQUIRE(!c->isCapped());
}

TEST_CASE("Cylinder: Intersecting the caps of a closed cylinder") {
    auto c = Cylinder::createCylinder();
    c->setMinimumY(1);
    c->setMaximumY(2);
    c->setCapped(true);

    constexpr std::array<Tuple, 5> points{make_point(0,  3,  0),
                                          make_point(0,  3, -2),
                                          make_point(0,  4, -2),
                                          make_point(0,  0, -2),
                                          make_point(0, -1, -2)};
    constexpr std::array<Tuple, 5> directions{make_vector(0, -1, 0),
                                              make_vector(0, -1, 2),
                                              make_vector(0, -1, 1),
                                              make_vector(0,  1, 2),
                                              make_vector(0,  1, 1)};

    constexpr std::array<int, 5> count{2, 2, 2, 2, 2};

    for (size_t i = 0; i < 5; ++i) {
        const auto direction = directions[i].normalize();
        const Ray ray{points[i], direction};
        const auto xs = c->localIntersection(ray);
        REQUIRE(xs.size() == count[i]);
    }
}

TEST_CASE("Cylinder: The normal vector on a cylinder's end caps") {
    auto c = Cylinder::createCylinder();
    c->setMinimumY(1);
    c->setMaximumY(2);
    c->setCapped(true);

    constexpr std::array<Tuple, 6> points{make_point(0,   1, 0),
                                          make_point(0.5, 1, 0),
                                          make_point(0,   1, 0.5),
                                          make_point(0,   2, 0),
                                          make_point(0.5, 2, 0),
                                          make_point(0,   2, 0.5)};
    constexpr std::array<Tuple, 6> normals{make_vector(0, -1, 0),
                                           make_vector(0, -1, 0),
                                           make_vector(0, -1, 0),
                                           predefined_tuples::y1,
                                           predefined_tuples::y1,
                                           predefined_tuples::y1};

    for (size_t i = 0; i < 6; ++i)
        REQUIRE(c->localNormalAt(points[i]) == normals[i]);
}

TEST_CASE("Cylinder: An unbounded cylinder has a bounding box") {
    const auto c = Cylinder::createCylinder();
    const auto box = c->bounds();
    REQUIRE(box.getMinPoint() == make_point(-1, math_constants::ninf<>, -1));
    REQUIRE(box.getMaxPoint() == make_point( 1, math_constants::inf<>,   1));
}