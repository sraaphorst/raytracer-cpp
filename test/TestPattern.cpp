/**
 * TestPattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

#include "pattern.h"
#include "stripepattern.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("Pattern: Creating a stripe pattern") {
    const StripePattern pattern{{predefined_colours::white, predefined_colours::black}};
    REQUIRE(pattern.getColours()[0] == predefined_colours::white);
    REQUIRE(pattern.getColours()[1] == predefined_colours::black);
}

TEST_CASE("Pattern: A stripe pattern is constant in y") {
    const StripePattern pattern{{predefined_colours::white, predefined_colours::black}};
    REQUIRE(pattern.colour_at(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colour_at(make_point(0, 1, 0)) == predefined_colours::white);
    REQUIRE(pattern.colour_at(make_point(0, 2, 0)) == predefined_colours::white);
}

TEST_CASE("Pattern: A stripe pattern is constant in z") {
    const StripePattern pattern{{predefined_colours::white, predefined_colours::black}};
    REQUIRE(pattern.colour_at(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colour_at(make_point(0, 0, 1)) == predefined_colours::white);
    REQUIRE(pattern.colour_at(make_point(0, 0, 2)) == predefined_colours::white);
}

TEST_CASE("Pattern: A stripe pattern alternates in x") {
    const StripePattern pattern{{predefined_colours::red, predefined_colours::green, predefined_colours::blue}};
    REQUIRE(pattern.colour_at(predefined_tuples::zero_point) == predefined_colours::red);
    REQUIRE(pattern.colour_at(make_point(1, 0, 0)) == predefined_colours::green);
    REQUIRE(pattern.colour_at(make_point(2, 0, 0)) == predefined_colours::blue);
    REQUIRE(pattern.colour_at(make_point(3, 0, 0)) == predefined_colours::red);
    REQUIRE(pattern.colour_at(make_point(4, 0, 0)) == predefined_colours::green);
    REQUIRE(pattern.colour_at(make_point(5, 0, 0)) == predefined_colours::blue);
}
