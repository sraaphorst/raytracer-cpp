/**
 * TestMath.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include <cmath>

#include "common.h"
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

TEST_CASE("normalize_radians should map radians to appropriate value in [-pi,pi]") {
    constexpr auto pi = math_details::pi<double>;
    constexpr auto res1 = normalize_radians(pi);
    REQUIRE(ALMOST_EQUALS(normalize_radians(pi), pi));

    constexpr auto res2 = normalize_radians(-pi);
    REQUIRE(ALMOST_EQUALS(normalize_radians(-pi), -pi));

    constexpr auto res3 = normalize_radians(2 * pi + pi / 2);
    REQUIRE(ALMOST_EQUALS(normalize_radians(2 * pi + pi / 2), pi / 2));

    constexpr auto res4 = normalize_radians(-(2 * pi + pi / 2));
    REQUIRE(ALMOST_EQUALS(normalize_radians(-(2 * pi + pi / 2)), -pi / 2));
}

TEST_CASE("Check sinc approximation is off by most EPSILON") {
    constexpr double res1 = sinc(math_details::pi<double>);
    REQUIRE(ALMOST_EQUALS(sinc(math_details::pi<double>), 0));

    constexpr double res2 = sinc(-math_details::pi<double>);
    REQUIRE(ALMOST_EQUALS(sinc(-math_details::pi<double>), 0));

    constexpr double res3 = sinc(math_details::half_pi<double>);
    REQUIRE(ALMOST_EQUALS(sinc(math_details::half_pi<double>), 1));

    constexpr double res4 = sinc(-math_details::half_pi<double>);
    REQUIRE(ALMOST_EQUALS(sinc(-math_details::half_pi<double>), -1));

    constexpr double res5 = sinc<double>(0);
    REQUIRE(sinc<double>(0) == 0);
}

TEST_CASE("Check cosc approximation is off by most EPSILON") {
    constexpr double res1 = cosc(math_details::pi<double>);
    REQUIRE(ALMOST_EQUALS(cosc(math_details::pi<double>), -1));

    constexpr double res2 = cosc(-math_details::pi<double>);
    REQUIRE(ALMOST_EQUALS(cosc(-math_details::pi<double>), -1));

    constexpr double res3 = cosc(math_details::half_pi<double>);
    REQUIRE(ALMOST_EQUALS(cosc(math_details::half_pi<double>), 0));

    constexpr double res4 = cosc(-math_details::half_pi<double>);
    REQUIRE(ALMOST_EQUALS(cosc(-math_details::half_pi<double>), 0));

    constexpr double res5 = cosc<double>(0);
    REQUIRE(cosc<double>(0) == 1);
}