/**
 * TestMath.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include <cmath>

#include "constmath.h"

using namespace raytracer;

TEST_CASE("factorial should produce the correct result", "[math][factorial]") {
    // Make sure factorial is constexpr.
    constexpr auto f = factorial(10);
    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);
    REQUIRE(factorial(3) == 6);
    REQUIRE(factorial(4) == 24);
    REQUIRE(factorial(5) == 120);
    REQUIRE(factorial(6) == 720);
    REQUIRE(factorial(7) == 5040);
    REQUIRE(factorial(8) == 40320);
    REQUIRE(factorial(9) == 362880);
    REQUIRE(factorial(10) == f);
}

TEST_CASE("max_factorial should produce the largest factorial value possible", "[math][factorial]") {
    // Current implementation of max_factorial is not constexpr.
    constexpr auto mf = max_factorial();
    constexpr auto fmf = factorial(mf);
    constexpr auto fmfp1 = factorial(mf + 1);
    REQUIRE(factorial(mf) - factorial(mf + 1) < factorial(mf));
}