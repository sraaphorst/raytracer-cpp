/**
 * TestCube.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <array>
#include <vector>

#include "intersection.h"
#include "ray.h"
#include "vector.h"

// A necessary evil to test the private members.
#define private public
#include "cube.h"

using namespace raytracer;

TEST_CASE("Cube: A ray intersecting a cube") {
    const Cube cube;

    /**
     * Cases:
     * 0. +x
     * 1. -x
     * 2. +y
     * 3. -y
     * 4. +z
     * 5. -z
     * 6. inside
     */

    constexpr std::array<Tuple, 7> origins{make_point( 5,    0.5,  0),
                                           make_point(-5,    0.5,  0),
                                           make_point( 0.5,  5,    0),
                                           make_point( 0.5, -5,    0),
                                           make_point( 0.5,  0,    5),
                                           make_point( 0.5,  0,   -5),
                                           make_point( 0,    0.5,  0)};
    constexpr std::array<Tuple, 7> directions{make_vector(-1,  0,  0),
                                              make_vector( 1,  0,  0),
                                              make_vector( 0, -1,  0),
                                              make_vector( 0,  1,  0),
                                              make_vector( 0,  0, -1),
                                              make_vector( 0,  0,  1),
                                              make_vector( 0,  0,  1)};
    std::array<double, 7> t0s{4, 4, 4, 4, 4, 4, -1};
    std::array<double, 7> t1s{6, 6, 6, 6, 6, 6,  1};

    for (size_t i = 0; i < 7; ++i) {
        const auto &origin = origins[i];
        const auto &direction = directions[i];
        const auto t0 = t0s[i];
        const auto t1 = t1s[i];

        const Ray ray{origin, direction};
        const auto xs = cube.localIntersection(ray);
        REQUIRE(xs.size() == 2);
        REQUIRE(xs[0].getT() == t0);
        REQUIRE(xs[1].getT() == t1);
    }
}

TEST_CASE("Cube: A ray misses a cube") {
    const Cube cube;

    constexpr std::array<Tuple, 6> origins{make_point(-2,  0,  0),
                                           make_point( 0, -2,  0),
                                           make_point( 0,  0, -2),
                                           make_point( 2,  0,  2),
                                           make_point( 0,  2,  2),
                                           make_point( 2,  2,  0)};
    constexpr std::array<Tuple, 6> directions{make_vector(0.2673, 0.5345, 0.8018),
                                              make_vector(0.8018, 0.2673, 0.5345),
                                              make_vector(0.5345, 0.8018, 0.2673),
                                              make_vector( 0, 0,  -1),
                                              make_vector( 0, -1,  0),
                                              make_vector(-1,  0,  0)};

    for (size_t i = 0; i < 6; ++i) {
        const auto &origin = origins[i];
        const auto &direction = directions[i];

        const Ray ray{origin, direction};
        const auto xs = cube.localIntersection(ray);
        REQUIRE(xs.empty());
    }
}

TEST_CASE("Cube: The normal on the surface of a cube") {
    const Cube cube;

    constexpr std::array<Tuple, 8> origins{make_point( 1,    0.5, -0.8),
                                           make_point(-1,   -0.2,  0.9),
                                           make_point(-0.4,  1,   -0.1),
                                           make_point( 0.3, -1,   -0.7),
                                           make_point(-0.6,  0.3,  1),
                                           make_point( 0.4,  0.4, -1),
                                           make_point( 1,    1,     1),
                                           make_point(-1,   -1,    -1)};
    constexpr std::array<Tuple, 8> normals{make_vector( 1,  0,  0),
                                           make_vector(-1,  0,  0),
                                           make_vector( 0,  1,  0),
                                           make_vector( 0, -1,  0),
                                           make_vector( 0,  0,  1),
                                           make_vector( 0,  0, -1),
                                           make_vector( 1,  0,  0),
                                           make_vector(-1,  0,  0)};

    for (size_t i = 0; i < 8; ++i) {
        const auto &origin = origins[i];
        const auto &normal = cube.localNormalAt(origin);
        REQUIRE(normal == normals[i]);
    }
}
