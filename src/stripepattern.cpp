/**
 * stripepattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>
#include <stdexcept>
#include <vector>

#include "constmath.h"
#include "stripepattern.h"
#include "vector.h"

namespace raytracer {
    StripePattern::StripePattern(): colours{predefined_colours::white, predefined_colours::black} {}
    StripePattern::StripePattern(const std::vector<Colour> &cs): colours{cs} {
        if (colours.empty())
            throw std::invalid_argument("StripePattern requires at least one colour.");
    }
    StripePattern::StripePattern(std::vector<Colour> &&cs): colours{std::move(cs)} {
        if (colours.empty())
            throw std::invalid_argument("StripePattern requires at least one colour.");
    }

    const Colour StripePattern::colourAt(const Tuple &point) const noexcept {
        const size_t idx = static_cast<size_t>(std::floor(point[tuple_constants::x]));
        return colours[((idx % colours.size()) + colours.size()) % colours.size()];
    }

    const std::vector<Colour> StripePattern::getColours() const noexcept {
        return colours;
    }

    void StripePattern::setColours(const std::vector<Colour> &cs) noexcept {
        colours = cs;
    }

    bool StripePattern::doCompare(const Pattern &other) const noexcept {
        const auto othersp = dynamic_cast<const StripePattern* const>(&other);
        return othersp != nullptr && colours == othersp->colours;
    }
}