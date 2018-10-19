/**
 * gradientpattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "pattern.h"
#include "vector.h"

namespace raytracer {
    class GradientPattern final: public Pattern {
        Colour colour1;
        Colour colour2;

    public:
        GradientPattern() noexcept;
        GradientPattern(const GradientPattern&) noexcept = default;
        GradientPattern(GradientPattern&&) noexcept = default;

        template<typename T, typename R>
        GradientPattern(T&& c1, R&& c2): colour1{c1}, colour2{c2} {}

        GradientPattern &operator=(const GradientPattern&) noexcept = default;
        GradientPattern &operator=(GradientPattern&&) noexcept = default;

        const Colour colourAt(const Tuple&) const noexcept override;
    };
}




