/**
 * TestTuple.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "common.h"
#include "tuple.h"

using namespace raytracer;

TEST_CASE("Tuple should be constructible via initializer list", "[Tuple][initializer_list]") {
    constexpr Vector<double, 4> v1{0,1,2,3};
    constexpr Vector<double, 4> v2{1,2,3,4};
    constexpr auto a = v1.cross_product(v2);

    // As expected, this does not compile since cross_product only exists for N=4.
    //Vector<double, 5> v3{0,1,2,3,4};
    //Vector<double, 5> v4{1,2,3,4,5};
    //v3.cross_product(v4);

    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2 = {1, 2, 3, Tuple::vector_flag};

    // Make sure == is constexpr.
    constexpr auto res = t1 == t2;
    REQUIRE(t1 == t2);
}

TEST_CASE("Tuple should be indexable", "[Tuple][index]") {
    constexpr Tuple t = Tuple::point(3, 1, 2);
    REQUIRE(t[0] == 3);
    REQUIRE(t[1] == 1);
    REQUIRE(t[2] == 2);
}

TEST_CASE("Tuple with w=1 is a point", "[Tuple][point]") {
    constexpr Tuple t{1, 1, 1, Tuple::point_flag};

    // Make sure isPoint is constexpr.
    constexpr auto res = t.isPoint();
    REQUIRE(t.isPoint());
}

TEST_CASE("Tuple with w=0 is a vector", "[Tuple][vector]") {
    constexpr Tuple t{1, 1, 1, Tuple::vector_flag};

    // Make sure isVector is constexpr.
    constexpr auto res = t.isVector();
    REQUIRE(t.isVector());
}

TEST_CASE("Tuple should be able to be multiplied by a factor on the left", "[Tuple][scalar_multiplication]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};

    // Make sure scalar multiplication is constexpr.
    constexpr auto res = 2 * t1;
    REQUIRE(2 * t1 == t2);
}

TEST_CASE("Tuple should be able to be multiplied by a factor on the right", "[Tuple][scalar_multiplication]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};

    // Make sure scalar multiplication is constexpr.
    constexpr auto res1 = t1 * 2;
    constexpr auto res2 = res1 == t2;
    REQUIRE(t1 * 2 == t2);
}

TEST_CASE("Tuple should be able to calculate Hadamard product", "[Tuple][hadamard_prouct]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};

    // Make sure hadamard product is constexpr.
    constexpr auto res1 = t1 * t2;
    constexpr auto res2 = t1 * t2 == Tuple(2, 8, 18, Tuple::vector_flag);
    REQUIRE(t1 * t2 == Tuple(2, 8, 18, Tuple::vector_flag));
}

TEST_CASE("Tuple should be divisible by a scalar", "[Tuple][scalar_division") {
    constexpr Tuple t{2, 4, 6, Tuple::vector_flag};

    // Make sure scalar division is constexpr.
    constexpr auto res = t / 2;
    REQUIRE(t / 2 == Tuple{1, 2, 3, Tuple::vector_flag});
}

TEST_CASE("Tuple should be divisible by a tuple", "[Tuple][division]") {
    constexpr Tuple t1{2, 8, 32, 128};
    constexpr Tuple t2{2, 4,  8,  16};

    // Make sure vector division is constexpr.
    constexpr auto res = t1 / t2;
    REQUIRE(t1 / t2 == Tuple{1, 2, 4, 8});
}

TEST_CASE("Tuple should be able to be added to itself", "[Tuple][addition]") {
    constexpr Tuple t{1, 2, 3, Tuple::vector_flag};

    // Make sure vector addition is constexpr.
    constexpr auto res = t + t;
    REQUIRE(2 * t == t + t);
}

TEST_CASE("Tuple should be able to be negated", "[Tuple][negation]") {
    constexpr Tuple t{1, 2, 3, Tuple::vector_flag};

    // Make sure negation is constexpr.
    constexpr auto res = -t;
    REQUIRE(-t == -1 * t);
}

TEST_CASE("Tuple addition should be commutative", "[Tuple][addition][commutativity]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};

    constexpr auto res = t1 + t2 == t2 + t1;
    REQUIRE(t1 + t2 == t2 + t1);
}

TEST_CASE("Tuple addition should be associative", "[Tuple][addition][associativity]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    constexpr auto res = tx + (ty + tz) == (tx + ty) + tz;
    REQUIRE(tx + (ty + tz) == (tx + ty) + tz);
}

TEST_CASE("Tuple should have inverses", "[Tuple][inverse]") {
    constexpr Tuple t{1, 2, 3, Tuple::vector_flag};
    REQUIRE(t - t == tuple_constants::zero_vector);
    REQUIRE(t + (-t) == tuple_constants::zero_vector);
}

TEST_CASE("Tuple subtraction should not be commutative", "[Tuple][subtraction][commutativity]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};

    REQUIRE(t1 - t2 != t2 - t1);
}

TEST_CASE("Tuple subtraction should not be associative", "[Tuple][subtraction][associativity]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    REQUIRE(tx - (ty - tz) != (tx - ty) - tz);
}

TEST_CASE("Tuple subtraction of two points should result in a vector", "[Tuple][subtraction][point]") {
    constexpr Tuple t1 = Tuple::point(1, 0, 0);
    constexpr Tuple t2 = Tuple::point(0, 1, 0);
    REQUIRE((t1 - t2).isVector());
}

TEST_CASE("Tuple subtraction of a vector from a point should result in a point", "[Tuple][subtraction][point][vector]") {
    constexpr Tuple p = Tuple::point(1, 0, 0);
    constexpr Tuple v = Tuple::vector(0, 1, 0);
    REQUIRE((p - v).isPoint());
}

TEST_CASE("Tuple subtraction of two vectors should result in a vector", "[Tuple][subtraction][vector]") {
    constexpr Tuple v1 = Tuple::vector(1, 0, 0);
    constexpr Tuple v2 = Tuple::vector(0, 1, 0);
    REQUIRE((v1 - v2).isVector());
}

TEST_CASE("Tuple should be able to calculate dot product", "[Tuple][dot_product][vector]") {
    constexpr Tuple t1{1, 1, 1, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 8, Tuple::vector_flag};

    // Make sure dot product is constexpr.
    constexpr auto res = t1.dot_product(t2);
    REQUIRE(t1.dot_product(t2) == 14);
}

TEST_CASE("Tuple dot product should be commutative", "[Tuple][dot_product][commutativity]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};
    REQUIRE(t1.dot_product(t2) == t2.dot_product(t1));
}

TEST_CASE("Tuple dot product should be distributive", "[Tuple][dot_product][distributivity]") {
    constexpr Tuple t1{1, 2, 3, Tuple::vector_flag};
    constexpr Tuple t2{2, 4, 6, Tuple::vector_flag};
    constexpr Tuple t3{-1, -2, -3, Tuple::vector_flag};
    REQUIRE(t1.dot_product(t2 + t3) == t1.dot_product(t2) + t1.dot_product(t3));
}

TEST_CASE("Tuple should be able to calculate cross product", "[Tuple][cross_product][vector]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    // Make sure cross product is constexpr.
    constexpr auto res = tx.cross_product(ty);
    REQUIRE(tx.cross_product(ty) == tz);
    REQUIRE(ty.cross_product(tx) == -tz);
    REQUIRE(tx.cross_product(tz) == -ty);
    REQUIRE(tz.cross_product(tx) == ty);
    REQUIRE(ty.cross_product(tz) == tx);
    REQUIRE(tz.cross_product(ty) == -tx);
}

TEST_CASE("Tuple cross product should be self-inverting", "[Tuple][cross_product][inverse]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    REQUIRE(tx.cross_product(tx) == tuple_constants::zero_vector);
    REQUIRE(ty.cross_product(ty) == tuple_constants::zero_vector);
    REQUIRE(tz.cross_product(tz) == tuple_constants::zero_vector);
}

TEST_CASE("Tuple cross product is not associative", "[Tuple][cross_product][associativity]") {
    const Tuple txy = tuple_constants::x1 + tuple_constants::y1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    REQUIRE(txy.cross_product(ty.cross_product(tz)) != txy.cross_product(ty).cross_product(tz));
}

TEST_CASE("Tuple cross product is not commutative", "[Tuple][cross_product][commutativity]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    REQUIRE(tx.cross_product(ty) != ty.cross_product(tx));
    REQUIRE(tx.cross_product(tz) != tz.cross_product(tx));
    REQUIRE(ty.cross_product(tz) != tz.cross_product(ty));
}

TEST_CASE("Tuple point should not be able to calculate cross product", "[Tuple][cross_product][point]") {
    REQUIRE_THROWS(Tuple{1, 0, 0, Tuple::vector_flag}.cross_product(Tuple{0, 1, 0, Tuple::point_flag}));
}

TEST_CASE("Tuple point should create a point", "[Tuple][point]") {
    REQUIRE(Tuple::point(0, 0, 0).isPoint());
}

TEST_CASE("Tuple vector should create a vector", "[Tuple][vector]") {
    REQUIRE(Tuple::vector(0, 0, 0).isVector());
}

TEST_CASE("Tuple comparison equality should be able to tolerate slight offsets", "[Tuple][equality]") {
    constexpr Tuple t1 = Tuple::point(1, 0, 0);
    constexpr Tuple t2 = Tuple::point(1 - EPSILON/2, EPSILON/2, -EPSILON/2);

    REQUIRE(t1 == t2);
}

TEST_CASE("Tuple should calculate the magnitude of a vector", "[Tuple][magnitude]") {
    constexpr Tuple t = Tuple::vector(1, 4, 8);

    // Make sure magnitude is constexpr.
    constexpr auto res = t.magnitude();
    REQUIRE(t.magnitude() == 9);
}

TEST_CASE("Tuple normalization should produce a vector of magnitude 1", "[Tuple][normalization]") {
    constexpr Tuple t = Tuple::vector(1, 4, 8);

    // Make sure normalization is constexpr.
    constexpr auto res = t.normalize();
    REQUIRE(t.normalize().magnitude() == 1);
}
