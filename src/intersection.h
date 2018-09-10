/**
 * intersection.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <initializer_list>
#include <stdexcept>

#include "transformers.h"
#include "vector.h"

namespace raytracer {
    using namespace raytracer::transformers;

    /// Represents the intersection of a ray with an object.
    class Intersection final {
    private:
        const std::array<Tuple, 2> intersections;
        const std::size_t num_intersections;

    public:
        constexpr Intersection(): intersections{predefined_tuples::zero_point, predefined_tuples::zero_point}, num_intersections{0} {}
        constexpr explicit Intersection(const Tuple &t): intersections{t, predefined_tuples::zero_point}, num_intersections{1} {}
        constexpr Intersection(const Tuple &i1, const Tuple &i2): intersections{i1, i2}, num_intersections{2} {}
        constexpr Intersection(const Intersection &other) = default;
        constexpr Intersection(Intersection&& other) = default;

        constexpr Intersection(std::initializer_list<Tuple> points): intersections{points}, num_intersections{points.size()} {
            if (size > 2)
                throw std::length_error("Intersection takes at most two parameters.")
            if (!(intersections[0].isPoint() && intersections[1].isPoint())
                throw std::invalid_argument("Intersection requires its parameters to be points.")
        }
    };
}