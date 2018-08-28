/**
 * TestTuple.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "tup.h"

using namespace raytracer;

TEST_CASE("tup should not accept a parameter other than 0 or 1 in w", "[tup][point][vector]") {
    REQUIRE_THROWS(tup{1, 2, 3, 4});
}

TEST_CASE("tup should be constructible via initializer list", "[tup][initializer_list]") {
    tup t1{1, 2, 3, 1};
    tup t2 = {1, 2, 3, 1};
    REQUIRE(t1 == t2);
}

TEST_CASE("tup should be able to be multiplied by a factor on the left", "[tup][scalar_multiplication]") {
    tup t{1, 2, 3, 1};
    tup t2{2, 4, 6, 1};

    REQUIRE(2 * t == t2);
}

TEST_CASE("tup should be able to be multiplied by a factor on the right", "[tup][scalar_multiplication]") {
    tup t{1, 2, 3, 1};
    tup t2{2, 4, 6, 1};

    REQUIRE(t * 2 == t2);
}

TEST_CASE("tup should be able to be added to itself", "[tup][addition]") {
    tup t{1, 2, 3, 1};
    REQUIRE(2 * t == t + t);
}

TEST_CASE("tup should be able to be negated", "[tup][negation]") {
    tup t{1, 2, 3, 1};
    REQUIRE(-t == -1 * t);
}

TEST_CASE("tup addition should be commutative", "[tup][addition][commutativity]") {
    tup t1{1, 2, 3, 1};
    tup t2{2, 4, 6, 1};

    REQUIRE(t1 + t2 == t2 + t1);
}

TEST_CASE("tup addition should be associative", "[tup][addition][associativity]") {
    tup tx{1, 0, 0, 1};
    tup ty{0, 1, 0, 1};
    tup tz{0, 0, 1, 1};

    REQUIRE(tx + (ty + tz) == (tx + ty) + tz);
}

TEST_CASE("tup should have inverses", "[tup][inverse]") {
    tup t{1, 2, 3, 1};
    REQUIRE(t - t == tup::zero_vector);
    REQUIRE(t + (-t) == tup::zero_vector);
}

TEST_CASE("tup subtraction should not be commutative", "[tup][subtraction][commutativity]") {
    tup t1{1, 2, 3, 1};
    tup t2{2, 4, 6, 1};

    REQUIRE(t1 - t2 != t2 - t1);
}

TEST_CASE("tup subtraction should not be associative", "[tup][subtraction][associativity]") {
    tup tx{1, 0, 0, 1};
    tup ty{0, 1, 0, 1};
    tup tz{0, 0, 1, 1};

    REQUIRE(tx - (ty - tz) != (tx - ty) - tz);
}

TEST_CASE("tup should be able to calculate dot product", "[tup][dot_product][vector]") {
    tup t1{1, 1, 1, 1};
    tup t2{2, 4, 8, 1};
    REQUIRE(t1 * t2 == 14);
}

TEST_CASE("tup dot product should be commutative", "[tup][dot_product][commutativity]") {
    tup t1{1, 2, 3, 1};
    tup t2{2, 4, 6, 1};
    REQUIRE(t1 * t2 == t2 * t1);
}

TEST_CASE("tup dot product should be distributive", "[tup][dot_product][distributivity]") {
    tup t1{1, 2, 3, 1};
    tup t2{2, 4, 6, 1};
    tup t3{-1, -2, -3, 1};
    REQUIRE(t1 * (t2 + t3) == t1 * t2 + t1 * t3);
}

TEST_CASE("tup point should not be able to calculate dot product", "[tup][dot_product][point]") {
    REQUIRE_THROWS(tup{1, 2, 3, 1} * tup{2, 4, 8, 0});
}

TEST_CASE("tup should be able to calculate cross product", "[tup][cross_product][vector]") {
    tup tx{1, 0, 0, 1};
    tup ty{0, 1, 0, 1};
    tup tz{0, 0, 1, 1};

    REQUIRE(tx % ty == tz);
    REQUIRE(ty % tx == -tz);
    REQUIRE(tx % tz == -ty);
    REQUIRE(tz % tx == ty);
    REQUIRE(ty % tz == tx);
    REQUIRE(tz % ty == -tx);
}

TEST_CASE("tup cross product should be self-inverting", "[tup][cross_product][inverse]") {
    tup tx{1, 0, 0, 1};
    tup ty{0, 1, 0, 1};
    tup tz{0, 0, 1, 1};

    REQUIRE(tx % tx == tup::zero_vector);
    REQUIRE(ty % ty == tup::zero_vector);
    REQUIRE(tz % tz == tup::zero_vector);
}

TEST_CASE("tup cross product is not associative", "[tup][cross_product][associativity]") {
    tup txy{1, 1, 0, 1};
    tup ty{0, 1, 0, 1};
    tup tz{0, 0, 1, 1};

    REQUIRE(txy % (ty % tz) != (txy % ty) % tz);
}

TEST_CASE("tup cross product is not commutative", "[tup][cross_product][commutativity]") {
    tup tx{1, 0, 0, 1};
    tup ty{0, 1, 0, 1};
    tup tz{0, 0, 1, 1};

    REQUIRE(tx % ty != ty % tx);
    REQUIRE(tx % tz != tz % tx);
    REQUIRE(ty % tz != tz % ty);
}

TEST_CASE("tup point should not be able to calculate cross product", "[tup][cross_product][point]") {
    REQUIRE_THROWS(tup{1, 0, 0, 1} % tup{0, 1, 0, 0});
}
