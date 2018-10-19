/**
 * gradientpattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>

#include "gradientpattern.h"
#include "vector.h"

namespace raytracer {
    GradientPattern::GradientPattern() noexcept:
        colour1{predefined_colours::white},
        colour2{predefined_colours::black} {}

    const Colour GradientPattern::colourAt(const Tuple &point) const noexcept {
        return colour1 + (colour2 - colour1) * (point[tuple_constants::x] - std::floor(point[tuple_constants::x]));
    }

    bool GradientPattern::doCompare(const Pattern &other) const noexcept {
        const auto othergp = dynamic_cast<const GradientPattern* const>(&other);
        return othergp != nullptr && colour1 == othergp->colour1 && colour2 == othergp->colour2;
    }
}