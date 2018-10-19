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

TEST_CASE("Pattern: Stripes with an object transformation") {
    const std::shared_ptr<Pattern> pattern = std::make_shared<StripePattern>();
    Material material;
    material.setPattern(pattern);
    const Sphere s{scale(2, 2, 2), material};
    REQUIRE(pattern->colourAtObject(s, make_point(1.5, 0, 0)) == predefined_colours::white);
}

TEST_CASE("Pattern: Stripes with a pattern transformation") {
    const Sphere s;
    std::shared_ptr<Pattern> pattern = std::make_shared<StripePattern>();
    pattern->setTransformation(scale(2, 2, 2));
    const auto c = pattern->colourAtObject(s, make_point(1.5, 0, 0));
    REQUIRE(c == predefined_colours::white);

}

TEST_CASE("Pattern: Stripes with both an object and a pattern transformation") {
    const Sphere s{scale(2, 2, 2)};
    std::shared_ptr<Pattern> pattern = std::make_shared<StripePattern>();
    pattern->setTransformation(translation(0.5, 0, 0));
    const auto c = pattern->colourAtObject(s, make_point(2.5, 0, 0));
    REQUIRE(c == predefined_colours::white);
}
