/**
 * TestColour.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "colour.h"

using namespace raytracer;

TEST_CASE("Colours are RGB tuples", "[Colour][rgb]") {
    Colour rgb{-0.5, 0.4, 1.7};
    REQUIRE(ALMOST_EQUALS(rgb[Colour::r], -0.5));
    REQUIRE(ALMOST_EQUALS(rgb[Colour::g],  0.4));
    REQUIRE(ALMOST_EQUALS(rgb[Colour::b],  1.7));
}

TEST_CASE("Colours can be added", "[Colour][addition]") {
    Colour c1{0.9, 0.6, 0.75};
    Colour c2{0.7, 0.1, 0.25};
    REQUIRE(c1 + c2 == Colour{1.6, 0.7, 1.0});
}

TEST_CASE("Colours can be subtracted", "[Colour][subtraction]") {
    Colour c1{0.9, 0.6, 0.75};
    Colour c2{0.7, 0.1, 0.25};
    REQUIRE(c1 - c2 == Colour{0.2, 0.5, 0.5});
}

TEST_CASE("Colours can be multiplied by a scalar", "[Colour][scalar_multiplication]") {
    Colour c{0.2, 0.3, 0.4};
    REQUIRE(2 * c == Colour{0.4, 0.6, 0.8});
}

TEST_CASE("Colours can be multiplied by the hadamard product", "[Colour][hadamard_product]") {
    Colour c1{1  , 0.2, 0.4};
    Colour c2{0.9, 1,   0.1};
    REQUIRE(c1 * c2 == Colour{0.9, 0.2, 0.04});
}