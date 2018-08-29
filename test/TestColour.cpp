/**
 * TestColour.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "colour.h"

using namespace raytracer;

TEST_CASE("colours are RGB tuples", "[colour][rgb]") {
    colour rgb{-0.5, 0.4, 1.7};
    REQUIRE(ALMOST_EQUALS(rgb[colour::r], -0.5));
    REQUIRE(ALMOST_EQUALS(rgb[colour::g],  0.4));
    REQUIRE(ALMOST_EQUALS(rgb[colour::b],  1.7));
}

TEST_CASE("colours can be added", "[colour][addition]") {
    colour c1{0.9, 0.6, 0.75};
    colour c2{0.7, 0.1, 0.25};
    REQUIRE(c1 + c2 == colour{1.6, 0.7, 1.0});
}

TEST_CASE("colours can be subtracted", "[colour][subtraction]") {
    colour c1{0.9, 0.6, 0.75};
    colour c2{0.7, 0.1, 0.25};
    REQUIRE(c1 - c2 == colour{0.2, 0.5, 0.5});
}

TEST_CASE("colours can be multiplied by a scalar", "[colour][scalar_multiplication]") {
    colour c{0.2, 0.3, 0.4};
    REQUIRE(2 * c == colour{0.4, 0.6, 0.8});
}

TEST_CASE("colours can be multiplied by the hadamard product", "[colour][hadamard_product]") {
    colour c1{1  , 0.2, 0.4};
    colour c2{0.9, 1,   0.1};
    REQUIRE(c1.hadamard_product(c2) == colour{0.9, 0.2, 0.04});
}