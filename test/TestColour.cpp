/**
 * TestColour.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "common.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("Colours are RGB tuples", "[Colour][rgb]") {
    constexpr Colour rgb{-0.5, 0.4, 1.7};
    REQUIRE(ALMOST_EQUALS(rgb[colour_constants::r], -0.5));
    REQUIRE(ALMOST_EQUALS(rgb[colour_constants::g],  0.4));
    REQUIRE(ALMOST_EQUALS(rgb[colour_constants::b],  1.7));
}

TEST_CASE("Colours can be added", "[Colour][addition]") {
    constexpr Colour c1{0.9, 0.6, 0.75};
    constexpr Colour c2{0.7, 0.1, 0.25};
    constexpr auto res1 = c1 + c2;
    constexpr auto res2 = c1 + c2 == Colour{1.6, 0.7, 1.0};
    REQUIRE(c1 + c2 == Colour{1.6, 0.7, 1.0});
}

TEST_CASE("Colours can be subtracted", "[Colour][subtraction]") {
    constexpr Colour c1{0.9, 0.6, 0.75};
    constexpr Colour c2{0.7, 0.1, 0.25};
    constexpr auto res1 = c1 - c2;
    constexpr auto res2 = c1 - c2 == Colour{0.2, 0.5, 0.5};
    REQUIRE(c1 - c2 == Colour{0.2, 0.5, 0.5});
}

TEST_CASE("Colours can be multiplied by a scalar", "[Colour][scalar_multiplication]") {
    constexpr Colour c{0.2, 0.3, 0.4};
    constexpr auto res1 = 2 * c;
    constexpr auto res2 = 2 * c == Colour{0.4, 0.6, 0.8};
    REQUIRE(2 * c == Colour{0.4, 0.6, 0.8});
}

TEST_CASE("Colours can be multiplied by the hadamard product", "[Colour][hadamard_product]") {
    constexpr Colour c1{1  , 0.2, 0.4};
    constexpr Colour c2{0.9, 1,   0.1};
    constexpr auto res1 = c1 * c2;
    constexpr auto res2 = c1 * c2 == Colour{0.9, 0.2, 0.04};
    REQUIRE(c1 * c2 == Colour{0.9, 0.2, 0.04});
}

TEST_CASE("Colours should be able to determine when they are valid", "[Colour][validity]") {
    constexpr Colour c{0.5, 0, 1};
    constexpr auto res = c.isValidColour();
    REQUIRE(c.isValidColour());
}

TEST_CASE("Colours should be able to determine when they are invalid", "[Colour][validity]") {
    constexpr Colour c{0.5, 0, 1.1};
    constexpr auto res = c.isValidColour();
    REQUIRE(!c.isValidColour());
}