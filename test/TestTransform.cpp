/**
 * TestTransform.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "affine_transform.h"
#include "matrix.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("translation should produce the desired matrix", "[Transformation][translation]") {
    constexpr auto trans = translation(5, -3, 2);
    constexpr Transformation tExpect = {{1, 0, 0,  5},
                                        {0, 1, 0, -3},
                                        {0, 0, 1,  2},
                                        {0, 0, 0,  1}};
    REQUIRE(trans == tExpect);
}

TEST_CASE("translation should be able to move points", "[Transformation][translation]") {
    constexpr auto trans = translation(5, -3, 2);
    constexpr auto p = make_point(-3, 4, 5);
    REQUIRE(trans * p == make_point(2, 1, 7));
}

TEST_CASE("translation should have an inverse that reverses the translation", "[Transformation][translation]") {
    constexpr auto trans = translation(5, -3, 2);
    constexpr auto tinv = translation(-5, 3, -2);
    constexpr auto p = make_point(2, 1, 7);
    REQUIRE(trans.invert() == tinv);
    REQUIRE(trans.invert() * p == make_point(-3, 4, 5));
    REQUIRE(trans.invert() * trans == predefined_matrices::I<double, 4>);
}

TEST_CASE("translation should not affect vectors", "[Transformation][translation]") {
    constexpr auto trans = translation(5, -3, 2);
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
    constexpr auto half_quarter = rotation_x(math_constants::pi_by_four<>);
    constexpr auto full_quarter = rotation_x(math_constants::pi_by_two<>);
    constexpr auto sqrt2_by_2 = sqrtd(2)/2;
    REQUIRE(half_quarter * p == make_point(0, sqrt2_by_2, sqrt2_by_2));
    REQUIRE(full_quarter * p == make_point(0, 0, 1));
}

TEST_CASE("rotation_x inverse should rotate in the opposite direction", "[Transformation][rotation]") {
    constexpr auto p = make_point(0, 1, 0);
    constexpr auto half_quarter = rotation_x(math_constants::pi_by_four<>);
    constexpr auto sqrt2_by_2 = sqrtd(2)/2;
    REQUIRE(half_quarter * half_quarter.invert() == predefined_matrices::I<double, 4>);
    REQUIRE(half_quarter.invert() * p == make_point(0, sqrt2_by_2, -sqrt2_by_2));
}

TEST_CASE("rotation_y should rotate a point around the y axis", "[Transformation][rotation]") {
    constexpr auto p = make_point(0, 0, 1);
    constexpr auto half_quarter = rotation_y(math_constants::pi_by_four<>);
    constexpr auto full_quarter = rotation_y(math_constants::pi_by_two<>);
    constexpr auto sqrt2_by_2 = sqrtd(2) / 2;
    REQUIRE(half_quarter * p == make_point(sqrt2_by_2, 0, sqrt2_by_2));
    REQUIRE(full_quarter * p == make_point(1, 0, 0));
}

TEST_CASE("rotation_z should rotate a point around the z axis", "[Transformation][rotiation]") {
    constexpr auto p = make_point(0, 1, 0);
    constexpr auto half_quarter = rotation_z(math_constants::pi_by_four<>);
    constexpr auto full_quarter = rotation_z(math_constants::pi_by_two<>);
    constexpr auto sqrt2_by_2 = sqrtd(2) / 2;
    REQUIRE(half_quarter * p == make_point(-sqrt2_by_2, sqrt2_by_2, 0));
    REQUIRE(full_quarter * p == make_point(-1, 0, 0));
}

TEST_CASE("skew should move x in proportion to y", "[Transformation][skew]") {
    constexpr auto m = skew(1, 0, 0, 0, 0, 0);
    constexpr auto p = make_point(2, 3, 4);
    REQUIRE(m * p == make_point(5, 3, 4));
}

TEST_CASE("skew should move x in proportion to z", "[Transformation][skew]") {
    constexpr auto m = skew(0, 1, 0, 0, 0, 0);
    constexpr auto p = make_point(2, 3, 4);
    REQUIRE(m * p == make_point(6, 3, 4));
}

TEST_CASE("skew should move y in proportion to x", "[Transformation][skew]") {
    constexpr auto m = skew(0, 0, 1, 0, 0, 0);
    constexpr auto p = make_point(2, 3, 4);
    REQUIRE(m * p == make_point(2, 5, 4));
}

TEST_CASE("skew should move y in proportion to z", "[Transformation][skew]") {
    constexpr auto m = skew(0, 0, 0, 1, 0, 0);
    constexpr auto p = make_point(2, 3, 4);
    REQUIRE(m * p == make_point(2, 7, 4));
}

TEST_CASE("skew should move z in proportion to x", "[Transformation][skew]") {
    constexpr auto m = skew(0, 0, 0, 0, 1, 0);
    constexpr auto p = make_point(2, 3, 4);
    REQUIRE(m * p == make_point(2, 3, 6));
}

TEST_CASE("skew should move z in proportion to y", "[Transformation][skew]") {
    constexpr auto m = skew(0, 0, 0, 0, 0, 1);
    constexpr auto p = make_point(2, 3, 4);
    REQUIRE(m * p == make_point(2, 3, 7));
}

TEST_CASE("individual transformations should applied in sequence", "[Transformation]") {
    constexpr auto p = make_point(1, 0, 1);
    constexpr auto A = rotation_x(math_constants::pi_by_two<>);
    constexpr auto B = scale(5, 5, 5);
    constexpr auto C = translation(10, 5, 7);
    constexpr auto q = A * p;
    REQUIRE(q == make_point(1, -1, 0));
    constexpr auto r = B * q;
    REQUIRE(r == make_point(5, -5, 0));
    constexpr auto s = C * r;
    REQUIRE(s == make_point(15, 0, 7));
}

TEST_CASE("chained transformations should be applied in reverse order", "[Transformation]") {
    constexpr auto p = make_point(1, 0, 1);
    constexpr auto A = rotation_x(math_constants::pi_by_two<>);
    constexpr auto B = scale(5, 5, 5);
    constexpr auto C = translation(10, 5, 7);
    constexpr auto T = C * B * A;
    REQUIRE(T * p == make_point(15, 0, 7));
}

TEST_CASE("andThen should allow us to apply transformations in the desired order", "[Transformation]") {
    constexpr auto p = make_point(1, 0, 1);
    constexpr auto transform = predefined_matrices::I<double, 4>
            .andThen(rotation_x(math_constants::pi_by_two<>))
            .andThen(scale(5, 5, 5))
            .andThen(translation(10, 5, 7));
    REQUIRE(transform * p == make_point(15, 0, 7));
}

TEST_CASE("andThen should be the same as multiplying the transformation matrices in reverse order", "[Transfomation]") {
    constexpr auto A = translation(1, 2, 3);
    constexpr auto B = skew(1, 1.5, -2, -3, 1, -1);
    constexpr auto C = rotation_x(math_constants::pi_by_four<>);
    constexpr auto D = scale(0.1, -1.0, 2);
    constexpr auto res1 = D * C * B * A;
    constexpr auto res2 = D * C * B * A == A.andThen(B).andThen(C).andThen(D);
    REQUIRE(D * C * B * A == A.andThen(B).andThen(C).andThen(D));
}