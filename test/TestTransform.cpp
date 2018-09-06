/**
 * TestTransform.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>

#include "catch.hpp"

#include "affine_transform.h"
#include "matrix.h"
#include "vector.h"

using namespace raytracer;

constexpr auto trans = translate(5, -3, 2);

TEST_CASE("translate should produce the desired matrix", "[Transformation][translate]") {
    constexpr Transformation tExpect = {{1, 0, 0,  5},
                                        {0, 1, 0, -3},
                                        {0, 0, 1,  2},
                                        {0, 0, 0,  1}};
    REQUIRE(trans == tExpect);
}

TEST_CASE("translate should be able to move points", "[Transformation][translate]") {
    constexpr auto p = make_point(-3, 4, 5);
    REQUIRE(trans * p == make_point(2, 1, 7));
}

TEST_CASE("translate should have an inverse that reverses the translation", "[Transformation][translate]") {
    constexpr auto tinv = translate(-5, 3, -2);
    constexpr auto p = make_point(2, 1, 7);
    REQUIRE(trans.invert() == tinv);
    REQUIRE(trans.invert() * p == make_point(-3, 4, 5));
    REQUIRE(trans.invert() * trans == predefined_matrices::I<double, 4>);
}

TEST_CASE("translate should not affect vectors", "[Transformation][translate]") {
    constexpr auto v = make_vector(1, 1, 1);
    REQUIRE(trans * v == v);
}

