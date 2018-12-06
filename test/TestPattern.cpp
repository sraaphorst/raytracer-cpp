/**
 * TestPattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <stdexcept>
#include <vector>

#include "patterns/checkerpattern.h"
#include "patterns/gradientpattern.h"
#include "patterns/pattern.h"
#include "patterns/ringpattern.h"
#include "shapes/sphere.h"
#include "patterns/stripepattern.h"
#include "vec.h"
#include "TestPattern.h"

using namespace raytracer;
using namespace raytracer::shapes;

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
    auto s = Sphere::createSphere();
    s->setTransformation(scale(2, 2, 2));
    
    const auto pattern = std::make_shared<TestPattern>();
    auto material = std::make_shared<Material>(pattern);
    s->setMaterial(material);
    
    const auto c = pattern->colourAtObject(s, make_point(2, 3, 4));
    REQUIRE(c == make_colour(1, 1.5, 2));
}

TEST_CASE("Pattern: Pattern with a pattern transformation") {
    std::shared_ptr<Pattern> pattern = std::make_shared<TestPattern>();
    pattern->setTransformation(scale(2, 2, 2));
    const auto c = pattern->colourAtObject(Sphere::createSphere(), make_point(2, 3, 4));
    REQUIRE(c == make_colour(1, 1.5, 2));

}

TEST_CASE("Pattern: Pattern with both an object and a pattern transformation") {
    auto s = Sphere::createSphere();
    s->setTransformation(scale(2, 2, 2));
    
    std::shared_ptr<Pattern> pattern = std::make_shared<TestPattern>();
    pattern->setTransformation(translation(0.5, 1, 1.5));
    
    const auto c = pattern->colourAtObject(s, make_point(2.5, 3, 3.5));
    REQUIRE(c == make_colour(0.75, 0.5, 0.25));
}

TEST_CASE("Pattern: Gradient linearly interpolates between colours") {
    GradientPattern pattern;
    REQUIRE(pattern.colourAt(make_point(0, 0, 0)) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0.25, 0, 0)) == make_colour(0.75, 0.75, 0.75));
    REQUIRE(pattern.colourAt(make_point(0.5, 0, 0)) == make_colour(0.5, 0.5, 0.5));
    REQUIRE(pattern.colourAt(make_point(0.75, 0, 0)) == make_colour(0.25, 0.25, 0.25));
}

TEST_CASE("Pattern: Ring should extend in both x and z") {
    RingPattern pattern;
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(1, 0, 0)) == predefined_colours::black);
    REQUIRE(pattern.colourAt(make_point(0, 0, 1)) == predefined_colours::black);
    // 0.708 just slightly more than sqrt(2)/2.
    REQUIRE(pattern.colourAt(make_point(0.708, 0, 0.708)) == predefined_colours::black);
}

TEST_CASE("Pattern: Checkers should repeat in x") {
    CheckerPattern pattern;
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0.99, 0, 0)) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(1.01, 0, 0)) == predefined_colours::black);
}

TEST_CASE("Pattern: Checkers should repeat in y") {
    CheckerPattern pattern;
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 0.99, 0)) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 1.01, 0)) == predefined_colours::black);
}

TEST_CASE("Pattern: Checkers should repeat in z") {
    CheckerPattern pattern;
    REQUIRE(pattern.colourAt(predefined_tuples::zero_point) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 0, 0.99)) == predefined_colours::white);
    REQUIRE(pattern.colourAt(make_point(0, 0, 1.01)) == predefined_colours::black);
}
