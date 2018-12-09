/**
 * ringpattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>
#include <stdexcept>
#include <vector>

#include "constmath.h"
#include "pattern.h"
#include "ringpattern.h"
#include "vec.h"

namespace raytracer {
    RingPattern::RingPattern() noexcept: colours{predefined_colours::white, predefined_colours::black} {}

    RingPattern::RingPattern(const std::vector<Colour> &cs): colours{cs} {
        if (colours.empty())
            throw std::invalid_argument("RingPattern requires at least one colour.");
    }
    RingPattern::RingPattern(std::vector<Colour> &&cs): colours{std::move(cs)} {
        if (colours.empty())
            throw std::invalid_argument("StripePattern requires at least one colour.");
    }

    const Colour RingPattern::colourAt(const Tuple &point) const noexcept {
        const auto px = point[tuple_constants::x];
        const auto pz = point[tuple_constants::z];
        const auto idx = static_cast<size_t>(std::floor(const_sqrtd(px * px + pz * pz)));
        return colours[((idx % colours.size()) + colours.size()) % colours.size()];
    }

    const std::vector<Colour> RingPattern::getColours() const noexcept {
        return colours;
    }

    void RingPattern::setColours(const std::vector<Colour> &cs) noexcept {
        colours = cs;
    }

    bool RingPattern::doCompare(const Pattern &other) const noexcept {
        const auto otherrp = dynamic_cast<const RingPattern* const>(&other);
        return otherrp != nullptr && colours == otherrp->colours;
    }
}