/**
 * checkerpattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "pattern.h"
#include "vec.h"

namespace raytracer {
    class CheckerPattern final: public Pattern {
        Colour colour1;
        Colour colour2;
        
    public:
        CheckerPattern() noexcept;
        CheckerPattern(const CheckerPattern&) noexcept = default;
        CheckerPattern(CheckerPattern&&) noexcept = default;

        template<typename T, typename R>
        CheckerPattern(T&& c1, R&& c2): colour1{c1}, colour2{c2} {}

        CheckerPattern &operator=(const CheckerPattern&) noexcept = default;
        CheckerPattern &operator=(CheckerPattern&&) noexcept = default;

        const Colour colourAt(const Tuple&) const noexcept override;

    private:
        bool doCompare(const Pattern &other) const noexcept;

    };
}




