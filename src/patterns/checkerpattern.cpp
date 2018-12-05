/**
 * checkerpattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>

#include "checkerpattern.h"
#include "vec.h"

namespace raytracer {
    CheckerPattern::CheckerPattern() noexcept:
            colour1{predefined_colours::white},
            colour2{predefined_colours::black} {}

    const Colour CheckerPattern::colourAt(const Tuple &point) const noexcept {
        const auto idx =
                static_cast<size_t>(floor(point[tuple_constants::x])) +
                static_cast<size_t>(floor(point[tuple_constants::y])) +
                static_cast<size_t>(floor(point[tuple_constants::z])) % 2;
        return idx == 0 ? colour1 : colour2;

    }

    bool CheckerPattern::doCompare(const Pattern &other) const noexcept {
        const auto othercp = dynamic_cast<const CheckerPattern* const>(&other);
        return othercp != nullptr && colour1 == othercp->colour1 && colour2 == othercp->colour2;
    }
}