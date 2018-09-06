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

TEST_CASE("translate should produce the desired matrix", "[Transformation][translate]") {
    constexpr auto trans = translate(5, -3, 2);
    constexpr Transformation tExpect = {{1, 0, 0,  5},
                                        {0, 1, 0, -3},
                                        {0, 0, 1,  2},
                                        {0, 0, 0,  1}};
    REQUIRE(trans == tExpect);
}

TEST_CASE("translate should be able to move points", "[Transformation][translate]") {
    constexpr auto trans = translate(5, -3, 2);
    constexpr auto p = make_point(-3, 4, 5);
    REQUIRE(trans * p == make_point(2, 1, 7));
}

TEST_CASE("translate should have an inverse that reverses the translation", "[Transformation][translate]") {
    constexpr auto trans = translate(5, -3, 2);
    constexpr auto tinv = translate(-5, 3, -2);
    constexpr auto p = make_point(2, 1, 7);
    REQUIRE(trans.invert() == tinv);
    REQUIRE(trans.invert() * p == make_point(-3, 4, 5));
    REQUIRE(trans.invert() * trans == predefined_matrices::I<double, 4>);
}

TEST_CASE("translate should not affect vectors", "[Transformation][translate]") {
    constexpr auto trans = translate(5, -3, 2);
    constexpr auto v = make_vector(1, 1, 1);
    REQUIRE(trans * v == v);
}

TEST_CASE("scale should scale points", "[Transformation][scale]") {
    constexpr auto m1 = scale(2, 3, 4);
    constexpr auto m2 = scale(-0.5, 0.5, 0.5);
    constexpr auto p = make_point(-4, 6, 8);

    REQUIRE(m1 * p == make_point(-8, 18, 32));
    REQUIRE(m2 * p == make_point(2, 3, 4));
}

TEST_CASE("scale should scale vectors", "[Transformation][scale]") {
    constexpr auto m1 = scale(2, 3, 4);
    constexpr auto p = make_vector(-4, 6, 8);
    REQUIRE(m1 * p == make_vector(-8, 18, 32));
}

TEST_CASE("scale by inverse of scale matrix should move points and vectors back to original location", "[Transformation][scale]") {
    constexpr auto m = scale(2, 3, 4);
    constexpr auto minv = m.invert();
    constexpr auto p = make_point(-4, 6, 8);
    constexpr auto v = make_vector(8, -12, -16);

    REQUIRE(minv * (m * p) == p);
    REQUIRE(minv * (m * v) == v);
}

TEST_CASE("scale should be able to represent reflections", "[Transformation][scale]") {
    constexpr auto m = scale(-1, 1, 1);
    constexpr auto p = make_point(-4, 6, 8);
    constexpr auto v = make_vector(8, -12, -16);

    REQUIRE(m * p == make_point(4, 6, 8));
    REQUIRE(m * v == -make_vector(8, 12, 16));
}

TEST_CASE("rotation_x should rotate around the x axis", "[Transformation][rotation]") {
    constexpr auto p = make_point(0, 1, 0);
    constexpr auto half_quarter = rotation_x(math_constants::pi_by_four<double>);
    constexpr auto full_quarter = rotation_x(math_constants::pi_by_two<double>);
    constexpr auto sqrt2_by_2 = sqrtd(2)/2;
    REQUIRE(half_quarter * p == make_point(0, sqrt2_by_2, sqrt2_by_2));
    REQUIRE(full_quarter * p == make_point(0, 0, 1));
}

TEST_CASE("rotation_x inverse should rotate in the opposite direction", "[Transformation][rotation]") {
    constexpr auto p = make_point(0, 1, 0);
    constexpr auto half_quarter = rotation_x(math_constants::pi_by_four<double>);
    constexpr auto sqrt2_by_2 = sqrtd(2)/2;
    REQUIRE(half_quarter * half_quarter.invert() == predefined_matrices::I<double, 4>);
    REQUIRE(half_quarter.invert() * p == make_point(0, sqrt2_by_2, -sqrt2_by_2));
}

TEST_CASE("rotation_y should rotate a point around the y axis", "[Transformation][rotation]") {
    constexpr auto p = make_point(0, 0, 1);
    constexpr auto half_quarter = rotation_y(math_constants::pi_by_four<double>);
    constexpr auto full_quarter = rotation_y(math_constants::pi_by_two<double>);
    constexpr auto sqrt2_by_2 = sqrtd(2) / 2;
    REQUIRE(half_quarter * p == make_point(sqrt2_by_2, 0, sqrt2_by_2));
    REQUIRE(full_quarter * p == make_point(1, 0, 0));
}

TEST_CASE("rotation_z should rotate a point around the z axis", "[Transformation][rotiation]") {
    constexpr auto p = make_point(0, 1, 0);
    constexpr auto half_quarter = rotation_z(math_constants::pi_by_four<double>);
    constexpr auto full_quarter = rotation_z(math_constants::pi_by_two<double>);
    constexpr auto sqrt2_by_2 = sqrtd(2) / 2;
    REQUIRE(half_quarter * p == make_point(-sqrt2_by_2, sqrt2_by_2, 0));
    REQUIRE(full_quarter * p == make_point(-1, 0, 0));
}