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
    class StripePattern final: Pattern {
        std::vector<Colour> colours;

    public:
        StripePattern() = delete;
        explicit StripePattern(const std::vector<Colour>&);
        explicit StripePattern(std::vector<Colour>&&);
        StripePattern(const StripePattern&) = default;
        StripePattern(StripePattern&&) = default;
        StripePattern &operator=(const StripePattern&) = default;
        StripePattern &operator=(StripePattern&&) = default;

        const Colour colour_at(const Tuple&) const noexcept override;

        const std::vector<Colour> getColours() const noexcept;
        void setColours(const std::vector<Colour>&) noexcept;
    private:
        bool doCompare(const Pattern &other) const noexcept override;
    };
}




