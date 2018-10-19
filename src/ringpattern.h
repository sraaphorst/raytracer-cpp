/**
 * ringpattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vector>

#include "pattern.h"
#include "vector.h"

namespace raytracer {
    /// Depends on x and z to decide which colour to return, working similarly to stripes but using circular stripes.
    class RingPattern final: public Pattern {
        std::vector<Colour> colours;

    public:
        /// The default stripe pattern is alternating white and black.
        RingPattern() noexcept;
        explicit RingPattern(const std::vector<Colour>&);
        explicit RingPattern(std::vector<Colour>&&);
        RingPattern(const RingPattern&) = default;
        RingPattern(RingPattern&&) noexcept = default;
        RingPattern &operator=(const RingPattern&) noexcept = default;
        RingPattern &operator=(RingPattern&&) noexcept = default;

        const Colour colourAt(const Tuple &) const noexcept override;

        const std::vector<Colour> getColours() const noexcept;
        void setColours(const std::vector<Colour>&) noexcept;
    private:
        bool doCompare(const Pattern &other) const noexcept override;
    };
}



