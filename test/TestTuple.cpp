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
    Tuple t1{1, 2, 3, Tuple::vector_flag};
    Tuple t2 = {1, 2, 3, Tuple::vector_flag};
    REQUIRE(t1 == t2);
}

TEST_CASE("Tuple should be indexable", "[Tuple][index]") {
    Tuple t = Tuple::point(3, 1, 2);
    REQUIRE(t[0] == 3);
    REQUIRE(t[1] == 1);
    REQUIRE(t[2] == 2);
}

TEST_CASE("Tuple with w=1 is a point", "[Tuple][point]") {
    Tuple t{1, 1, 1, Tuple::point_flag};
    REQUIRE(t.isPoint());
}

TEST_CASE("Tuple with w=0 is a vector", "[Tuple][vector]") {
    Tuple t{1, 1, 1, Tuple::vector_flag};
    REQUIRE(t.isVector());
}

TEST_CASE("Tuple should be able to be multiplied by a factor on the left", "[Tuple][scalar_multiplication]") {
    Tuple t{1, 2, 3, Tuple::vector_flag};
    Tuple t2{2, 4, 6, Tuple::vector_flag};

    REQUIRE(2 * t == t2);
}

TEST_CASE("Tuple should be able to be multiplied by a factor on the right", "[Tuple][scalar_multiplication]") {
    Tuple t{1, 2, 3, Tuple::vector_flag};
    Tuple t2{2, 4, 6, Tuple::vector_flag};

    REQUIRE(t * 2 == t2);
}

TEST_CASE("Tuple should be able to be added to itself", "[Tuple][addition]") {
    Tuple t{1, 2, 3, Tuple::vector_flag};
    REQUIRE(2 * t == t + t);
}

TEST_CASE("Tuple should be able to be negated", "[Tuple][negation]") {
    Tuple t{1, 2, 3, Tuple::vector_flag};
    REQUIRE(-t == -1 * t);
}

TEST_CASE("Tuple addition should be commutative", "[Tuple][addition][commutativity]") {
    Tuple t1{1, 2, 3, Tuple::vector_flag};
    Tuple t2{2, 4, 6, Tuple::vector_flag};

    REQUIRE(t1 + t2 == t2 + t1);
}

TEST_CASE("Tuple addition should be associative", "[Tuple][addition][associativity]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    REQUIRE(tx + (ty + tz) == (tx + ty) + tz);
}

TEST_CASE("Tuple should have inverses", "[Tuple][inverse]") {
    Tuple t{1, 2, 3, Tuple::vector_flag};
    REQUIRE(t - t == tuple_constants::zero_vector);
    REQUIRE(t + (-t) == tuple_constants::zero_vector);
}

TEST_CASE("Tuple subtraction should not be commutative", "[Tuple][subtraction][commutativity]") {
    Tuple t1{1, 2, 3, Tuple::vector_flag};
    Tuple t2{2, 4, 6, Tuple::vector_flag};

    REQUIRE(t1 - t2 != t2 - t1);
}

TEST_CASE("Tuple subtraction should not be associative", "[Tuple][subtraction][associativity]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

    REQUIRE(tx - (ty - tz) != (tx - ty) - tz);
}

TEST_CASE("Tuple subtraction of two points should result in a vector", "[Tuple][subtraction][point]") {
    Tuple t1 = Tuple::point(1, 0, 0);
    Tuple t2 = Tuple::point(0, 1, 0);
    REQUIRE((t1 - t2).isVector());
}

TEST_CASE("Tuple subtraction of a vector from a point should result in a point", "[Tuple][subtraction][point][vector]") {
    Tuple p = Tuple::point(1, 0, 0);
    Tuple v = Tuple::vector(0, 1, 0);
    REQUIRE((p - v).isPoint());
}

TEST_CASE("Tuple subtraction of two vectors should result in a vector", "[Tuple][subtraction][vector]") {
    Tuple v1 = Tuple::vector(1, 0, 0);
    Tuple v2 = Tuple::vector(0, 1, 0);
    REQUIRE((v1 - v2).isVector());
}

TEST_CASE("Tuple should be able to calculate dot product", "[Tuple][dot_product][vector]") {
    Tuple t1{1, 1, 1, Tuple::vector_flag};
    Tuple t2{2, 4, 8, Tuple::vector_flag};
    REQUIRE(t1.dot_product(t2) == 14);
}

TEST_CASE("Tuple dot product should be commutative", "[Tuple][dot_product][commutativity]") {
    Tuple t1{1, 2, 3, Tuple::vector_flag};
    Tuple t2{2, 4, 6, Tuple::vector_flag};
    REQUIRE(t1.dot_product(t2) == t2.dot_product(t1));
}

TEST_CASE("Tuple dot product should be distributive", "[Tuple][dot_product][distributivity]") {
    Tuple t1{1, 2, 3, Tuple::vector_flag};
    Tuple t2{2, 4, 6, Tuple::vector_flag};
    Tuple t3{-1, -2, -3, Tuple::vector_flag};
    REQUIRE(t1.dot_product(t2 + t3) == t1.dot_product(t2) + t1.dot_product(t3));
}

TEST_CASE("Tuple should be able to calculate cross product", "[Tuple][cross_product][vector]") {
    const Tuple &tx = tuple_constants::x1;
    const Tuple &ty = tuple_constants::y1;
    const Tuple &tz = tuple_constants::z1;

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
    Tuple t1 = Tuple::point(1, 0, 0);
    Tuple t2 = Tuple::point(1 - EPSILON/2, EPSILON/2, -EPSILON/2);
    REQUIRE(t1 == t2);
}

TEST_CASE("Tuple should calculate the magnitude of a vector", "[Tuple][magnitude]") {
    Tuple t = Tuple::vector(1, 4, 8);
    REQUIRE(t.magnitude() == 9);
}

TEST_CASE("Tuple normalization should produce a vector of magnitude 1", "[Tuple][normalization]") {
    Tuple t = Tuple::vector(1, 4, 8);
    REQUIRE(t.normalize().magnitude() == 1);
}
