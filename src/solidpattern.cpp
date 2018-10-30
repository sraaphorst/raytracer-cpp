/**
 * solidpattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "pattern.h"
#include "solidpattern.h"
#include "vec.h"

namespace raytracer {
    SolidPattern::SolidPattern() noexcept: colour{predefined_colours::white} {}

    SolidPattern::SolidPattern(const Colour &c) noexcept: colour{c} {}

    SolidPattern::SolidPattern(Colour &&c) noexcept: colour{std::move(c)} {}

    const Colour SolidPattern::getColour() const noexcept {
        return colour;
    }

    void SolidPattern::setColour(const Colour &c) noexcept {
        colour = c;
    }

    bool SolidPattern::doCompare(const Pattern &other) const noexcept {
        const auto othersp = dynamic_cast<const SolidPattern* const>(&other);
        return othersp != nullptr && colour == othersp->colour;
    }
}