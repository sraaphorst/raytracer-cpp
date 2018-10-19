/**
 * stripepattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vector>

#include "pattern.h"
#include "vector.h"

namespace raytracer {
    class StripePattern final: public Pattern {
        std::vector<Colour> colours;

    public:
        /// The default stripe pattern is alternating white and black.
        StripePattern() noexcept;
        explicit StripePattern(const std::vector<Colour>&);
        explicit StripePattern(std::vector<Colour>&&);
        StripePattern(const StripePattern&) = default;
        StripePattern(StripePattern&&) noexcept = default;
        StripePattern &operator=(const StripePattern&) noexcept = default;
        StripePattern &operator=(StripePattern&&) noexcept = default;

        const Colour colourAt(const Tuple &) const noexcept override;

        const std::vector<Colour> getColours() const noexcept;
        void setColours(const std::vector<Colour>&) noexcept;
    private:
        bool doCompare(const Pattern &other) const noexcept override;
    };
}




