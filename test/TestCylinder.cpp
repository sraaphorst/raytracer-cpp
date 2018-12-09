/**
 * TestCylinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <vector>

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