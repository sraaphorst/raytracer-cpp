/**
 * TestCone.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <numeric>
#include <vector>

#include "bounding_box.h"
#include "intersection.h"
#include "ray.h"
#include "vec.h"

// A necessary evil to test the private members.
#define private public
#include "cone.h"

using namespace raytracer;
using namespace raytracer::impl;
using namespace raytracer::shapes;

TEST_CASE("Cone: A ray intersecting a cone") {
    const auto c = Cone::createCone();

    std::array<Tuple, 3> origins{make_point(0, 0, -5),
                                 make_point(0, 0, -5),
                                 make_point(1, 1, -5)};

    std::array<Tuple, 3> directions{predefined_tuples::z1,
                                    make_vector(1, 1, 1),
                                    make_vector(-0.5, -1, 1)};

    std::array<double, 3> t0s{5, 8.66025, 4.55006};
    std::array<double, 3> t1s{5, 8.66025, 49.44994};

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

TEST_CASE("Cone: The normal vector on a cone") {
    const auto c = Cone::createCone();

    constexpr std::array<Tuple, 3> origins{make_point(0, 0, 0),
                                           make_point(1, 1, 1),
                                           make_point(-1, -1, 0)};
    constexpr std::array<Tuple, 3> normals{make_vector(0, 0, 0),
                                           make_vector(1, -math_constants::sqrt2, 1),
                                           make_vector(-1, 1, 0)};

    for (size_t i = 0; i < 3; ++i) {
        const auto &origin = origins[i];
        const auto &normal = c->localNormalAt(origin);
        REQUIRE(normal == normals[i]);
    }
}

TEST_CASE("Cone: The default minimum and maximum for a cone") {
    const auto c = Cone::createCone();
    REQUIRE(c->getMinimumY() == -std::numeric_limits<double>::infinity());
    REQUIRE(c->getMaximumY() ==  std::numeric_limits<double>::infinity());
}

TEST_CASE("Cone: Intersecting a constrained cone") {
    auto c = Cone::createCone();
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

TEST_CASE("Cone: The default capped value for a cone") {
    const auto c = Cone::createCone();
    REQUIRE(!c->isCapped());
}

TEST_CASE("Cone: Intersecting the caps of a closed cone") {
    auto c = Cone::createCone();
    c->setMinimumY(-0.5);
    c->setMaximumY( 0.5);
    c->setCapped(true);

    constexpr std::array<Tuple, 3> points{make_point(0, 0, -5),
                                          make_point(0, 0, -0.25),
                                          make_point(0, 0, -0.25)};
    constexpr std::array<Tuple, 3> directions{predefined_tuples::y1,
                                              make_vector(0, 1, 1),
                                              predefined_tuples::y1};

    constexpr std::array<int, 3> count{0, 2, 4};

    for (size_t i = 0; i < 3; ++i) {
        const auto direction = directions[i].normalize();
        const Ray ray{points[i], direction};
        const auto xs = c->localIntersection(ray);
        REQUIRE(xs.size() == count[i]);
    }
}

TEST_CASE("Cone: The normal vector on a cone's end caps") {
    auto c = Cone::createCone();
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

TEST_CASE("Cone: A bounded cone has a bounding box") {
    auto c = Cone::createCone();
    c->setMinimumY(-5);
    c->setMaximumY(3);

    const auto box = c->boundsOf();
    REQUIRE(box.getMinPoint() == make_point(-5, -5, -5));
    REQUIRE(box.getMaxPoint() == make_point(5, 3, 5));
}