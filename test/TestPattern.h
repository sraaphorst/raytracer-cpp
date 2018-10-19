/**
 * TestPattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "pattern.h"
#include "vector.h"

/**
 * This is a class that implements just enough of Pattern to be a concrete implementation
 * so as to be used to isolate and test the common characteristics of all subclasses of Pattern.
 */
struct TestPattern final: raytracer::Pattern {
    TestPattern() = default;

    template<typename T>
    explicit TestPattern(T&& t): Pattern(t) {}

    const raytracer::Colour colourAt(const raytracer::Tuple &tuple) const noexcept override {
        return raytracer::make_colour(tuple[raytracer::tuple_constants::x],
                                      tuple[raytracer::tuple_constants::y],
                                      tuple[raytracer::tuple_constants::z]);
    }
};
