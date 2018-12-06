/**
 * TestTuple.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "common.h"
#include "vec.h"

using namespace raytracer;

TEST_CASE("Tuple: Tuple should be constructible via initializer list") {
    constexpr Vector<double, 4> v1{0,1,2,3};
    constexpr Vector<double, 4> v2{1,2,3,4};
    constexpr auto a = v1.cross_product(v2);

    // As expected, this does not compile since cross_product only exists for N=4.
    //Vector<double, 5> v3{0,1,2,3,4};
    //Vector<double, 5> v4{1,2,3,4,5};
    //v3.cross_product(v4);

    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2 = {1, 2, 3, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = t1 == t2;
    REQUIRE(t1 == t2);
}

TEST_CASE("Tuple: Tuple should be indexable") {
    constexpr Tuple t = make_point(3, 1, 2);
    REQUIRE(t[0] == 3);
    REQUIRE(t[1] == 1);
    REQUIRE(t[2] == 2);
}

TEST_CASE("Tuple: Tuple with w=1 is a point") {
    constexpr Tuple t{1, 1, 1, tuple_constants::point_flag};
    [[maybe_unused]] constexpr auto res = t.isPoint();
    REQUIRE(t.isPoint());
}

TEST_CASE("Tuple: Tuple with w=0 is a vector") {
    constexpr Tuple t{1, 1, 1, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = t.isVector();
    REQUIRE(t.isVector());
}

TEST_CASE("Tuple: Tuple should be able to be multiplied by a factor on the left") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = 2 * t1;
    REQUIRE(2 * t1 == t2);
}

TEST_CASE("Tuple: Tuple should be able to be multiplied by a factor on the right") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res1 = t1 * 2;
    [[maybe_unused]] constexpr auto res2 = res1 == t2;
    REQUIRE(t1 * 2 == t2);
}

TEST_CASE("Tuple: Tuple should be able to calculate Hadamard product") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res1 = t1 * t2;
    [[maybe_unused]] constexpr auto res2 = t1 * t2 == Tuple{2, 8, 18, tuple_constants::vector_flag};
    REQUIRE(t1 * t2 == Tuple{2, 8, 18, tuple_constants::vector_flag});
}

TEST_CASE("Tuple: Tuple should be divisible by a scalar", "[Tuple][scalar_division") {
    constexpr Tuple t{2, 4, 6, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = t / 2;
    REQUIRE(t / 2 == Tuple{1, 2, 3, tuple_constants::vector_flag});
}

TEST_CASE("Tuple: Tuple should be divisible by a tuple") {
    constexpr Tuple t1{2, 8, 32, 128};
    constexpr Tuple t2{2, 4,  8,  16};
    [[maybe_unused]] constexpr auto res = t1 / t2;
    REQUIRE(t1 / t2 == Tuple{1, 2, 4, 8});
}

TEST_CASE("Tuple: Tuple should be able to be added to itself") {
    constexpr Tuple t{1, 2, 3, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = t + t;
    REQUIRE(2 * t == t + t);
}

TEST_CASE("Tuple: Tuple should be able to be negated") {
    constexpr Tuple t{1, 2, 3, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = -t;
    REQUIRE(-t == -1 * t);
}

TEST_CASE("Tuple: Tuple addition should be commutative") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = t1 + t2 == t2 + t1;
    REQUIRE(t1 + t2 == t2 + t1);
}

TEST_CASE("Tuple: Tuple addition should be associative") {
    const Tuple &tx = predefined_tuples::x1;
    const Tuple &ty = predefined_tuples::y1;
    const Tuple &tz = predefined_tuples::z1;
    [[maybe_unused]] constexpr auto res = tx + (ty + tz) == (tx + ty) + tz;
    REQUIRE(tx + (ty + tz) == (tx + ty) + tz);
}

TEST_CASE("Tuple: Tuple should have inverses") {
    constexpr Tuple t{1, 2, 3, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res1 = t - t;
    [[maybe_unused]] constexpr auto res2 = t + (-t);
    REQUIRE(t - t == predefined_tuples::zero_vector);
    REQUIRE(t + (-t) == predefined_tuples::zero_vector);
}

TEST_CASE("Tuple: Tuple subtraction should not be commutative") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res1 = t1 - t2;
    [[maybe_unused]] constexpr auto res2 = t2 - t1;
    [[maybe_unused]] constexpr auto res3 = t1 - t2 != t2 - t1;
    REQUIRE(t1 - t2 != t2 - t1);
}

TEST_CASE("Tuple: Tuple subtraction should not be associative") {
    const Tuple &tx = predefined_tuples::x1;
    const Tuple &ty = predefined_tuples::y1;
    const Tuple &tz = predefined_tuples::z1;
    REQUIRE(tx - (ty - tz) != (tx - ty) - tz);
}

TEST_CASE("Tuple: Tuple subtraction of two points should result in a vector") {
    constexpr Tuple t1 = make_point(1, 0, 0);
    constexpr Tuple t2 = make_point(0, 1, 0);
    REQUIRE((t1 - t2).isVector());
}

TEST_CASE("Tuple: Tuple subtraction of a vector from a point should result in a point") {
    constexpr Tuple p = make_point(1, 0, 0);
    constexpr Tuple v = make_vector(0, 1, 0);
    REQUIRE((p - v).isPoint());
}

TEST_CASE("Tuple: Tuple subtraction of two vectors should result in a vector") {
    constexpr Tuple v1 = make_vector(1, 0, 0);
    constexpr Tuple v2 = make_vector(0, 1, 0);
    REQUIRE((v1 - v2).isVector());
}

TEST_CASE("Tuple: Tuple should be able to calculate dot product") {
    constexpr Tuple t1{1, 1, 1, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 8, tuple_constants::vector_flag};
    [[maybe_unused]] constexpr auto res = t1.dot_product(t2);
    REQUIRE(t1.dot_product(t2) == 14);
}

TEST_CASE("Tuple: Tuple dot product should be commutative") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    REQUIRE(t1.dot_product(t2) == t2.dot_product(t1));
}

TEST_CASE("Tuple: Tuple dot product should be distributive") {
    constexpr Tuple t1{1, 2, 3, tuple_constants::vector_flag};
    constexpr Tuple t2{2, 4, 6, tuple_constants::vector_flag};
    constexpr Tuple t3{-1, -2, -3, tuple_constants::vector_flag};
    REQUIRE(t1.dot_product(t2 + t3) == t1.dot_product(t2) + t1.dot_product(t3));
}

TEST_CASE("Tuple: Tuple should be able to calculate cross product") {
    const Tuple &tx = predefined_tuples::x1;
    const Tuple &ty = predefined_tuples::y1;
    const Tuple &tz = predefined_tuples::z1;

    [[maybe_unused]] constexpr auto res = tx.cross_product(ty);
    REQUIRE(tx.cross_product(ty) == tz);
    REQUIRE(ty.cross_product(tx) == -tz);
    REQUIRE(tx.cross_product(tz) == -ty);
    REQUIRE(tz.cross_product(tx) == ty);
    REQUIRE(ty.cross_product(tz) == tx);
    REQUIRE(tz.cross_product(ty) == -tx);
}

TEST_CASE("Tuple: Tuple cross product should be self-inverting") {
    constexpr Tuple tx = predefined_tuples::x1;
    constexpr Tuple ty = predefined_tuples::y1;
    constexpr Tuple tz = predefined_tuples::z1;

    REQUIRE(tx.cross_product(tx) == predefined_tuples::zero_vector);
    REQUIRE(ty.cross_product(ty) == predefined_tuples::zero_vector);
    REQUIRE(tz.cross_product(tz) == predefined_tuples::zero_vector);
}

TEST_CASE("Tuple: Tuple cross product is not associative") {
    constexpr Tuple txy = predefined_tuples::x1 + predefined_tuples::y1;
    constexpr Tuple ty  = predefined_tuples::y1;
    constexpr Tuple tz  = predefined_tuples::z1;

    [[maybe_unused]] constexpr auto res1 = txy.cross_product(ty.cross_product(tz));
    [[maybe_unused]] constexpr auto res2 = txy.cross_product(ty).cross_product(tz);
    REQUIRE(txy.cross_product(ty.cross_product(tz)) != txy.cross_product(ty).cross_product(tz));
}

TEST_CASE("Tuple: Tuple cross product is not commutative") {
    constexpr Tuple tx = predefined_tuples::x1;
    constexpr Tuple ty = predefined_tuples::y1;
    constexpr Tuple tz = predefined_tuples::z1;

    REQUIRE(tx.cross_product(ty) != ty.cross_product(tx));
    REQUIRE(tx.cross_product(tz) != tz.cross_product(tx));
    REQUIRE(ty.cross_product(tz) != tz.cross_product(ty));
}

TEST_CASE("Tuple: Tuple point should create a point") {
    [[maybe_unused]] constexpr auto res = make_point(0, 0, 0).isPoint();
    REQUIRE(make_point(0, 0, 0).isPoint());
}

TEST_CASE("Tuple: Tuple vector should create a vector") {
    [[maybe_unused]] constexpr auto res = make_vector(0, 0, 0).isVector();
    REQUIRE(make_vector(0, 0, 0).isVector());
}

TEST_CASE("Tuple: Tuple comparison equality should be able to tolerate slight offsets") {
    constexpr Tuple t1 = make_point(1, 0, 0);
    constexpr Tuple t2 = make_point(1 - EPSILON/2, EPSILON/2, -EPSILON/2);
    REQUIRE(t1 == t2);
}

TEST_CASE("Tuple: Tuple should calculate the magnitude of a vector") {
    constexpr Tuple t = make_vector(1, 4, 8);
    [[maybe_unused]] constexpr auto res = t.magnitude();
    REQUIRE(t.magnitude() == 9);
}

TEST_CASE("Tuple: Tuple normalization should produce a vector of magnitude 1") {
    constexpr Tuple t = make_vector(1, 4, 8);
    [[maybe_unused]] constexpr auto res = t.normalize();
    REQUIRE(t.normalize().magnitude() == 1);
}

TEST_CASE("Tuple: Reflecting a vector approaching at 45 degrees") {
    const auto v = make_vector(1, -1, 0);
    const auto n = make_vector(0, 1, 0);
    const auto r = v.reflect(n);
    REQUIRE(r == make_vector(1, 1, 0));
}

TEST_CASE("Tuple: Reflecting a vector off a slanted surface") {
    const auto v = make_vector(0, -1, 0);
    const auto sqrt2by2 = sqrtd(2) / 2;
    const auto n = make_vector(sqrt2by2, sqrt2by2, 0);
    const auto r = v.reflect(n);
    REQUIRE(r == make_vector(1, 0, 0));

}
