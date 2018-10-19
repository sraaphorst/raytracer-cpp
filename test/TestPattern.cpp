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
#include "sphere.h"
#include "stripepattern.h"
#include "vector.h"
#include "TestPattern.h"

using namespace raytracer;

TEST_CASE("Pattern: Creating default stripe pattern") {
    const StripePattern pattern;
    REQUIRE(pattern.getColours()[0] == predefined_colours::white);
    REQUIRE(pattern.getColours()[1] == predefined_colours::black);
}

TEST_CASE("Pattern: A stripe pattern is constant in y") {
    const StripePattern pattern{{predefined_colours::white, predefined_colours::black}};
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 1, 0)) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 2, 0)) == predefined_colours::white);
}

TEST_CASE("Pattern: A stripe pattern is constant in z") {
    const StripePattern pattern;
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 0, 1)) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 0, 2)) == predefined_colours::white);
}

TEST_CASE("Pattern: A stripe pattern alternates in x") {
    const StripePattern pattern{{predefined_colours::red, predefined_colours::green, predefined_colours::blue}};
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::red);
    REQUIRE(pattern.colourAt(make_point(1, 0, 0)) == predefined_colours::green);
    REQUIRE(pattern.colourAt(make_point(2, 0, 0)) == predefined_colours::blue);
    REQUIRE(pattern.colourAt(make_point(3, 0, 0)) == predefined_colours::red);
    REQUIRE(pattern.colourAt(make_point(4, 0, 0)) == predefined_colours::green);
    REQUIRE(pattern.colourAt(make_point(5, 0, 0)) == predefined_colours::blue);
}

TEST_CASE("Pattern: Pattern with an object transformation") {
    const std::shared_ptr<Pattern> pattern = std::make_shared<TestPattern>();
    Material material;
    material.setPattern(pattern);
    const Sphere s{scale(2, 2, 2), material};
    const auto c = pattern->colourAtObject(s, make_point(2, 3, 4));
    REQUIRE(c == make_colour(1, 1.5, 2));
}

TEST_CASE("Pattern: Pattern with a pattern transformation") {
    const Sphere s;
    std::shared_ptr<Pattern> pattern = std::make_shared<TestPattern>();
    pattern->setTransformation(scale(2, 2, 2));
    const auto c = pattern->colourAtObject(s, make_point(2, 3, 4));
    REQUIRE(c == make_colour(1, 1.5, 2));

}

TEST_CASE("Pattern: Pattern with both an object and a pattern transformation") {
    const Sphere s{scale(2, 2, 2)};
    std::shared_ptr<Pattern> pattern = std::make_shared<TestPattern>();
    pattern->setTransformation(translation(0.5, 1, 1.5));
    const auto c = pattern->colourAtObject(s, make_point(2.5, 3, 3.5));
    REQUIRE(c == make_colour(0.75, 0.5, 0.25));
}
