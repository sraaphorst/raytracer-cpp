/**
 * pattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "vector.h"

namespace raytracer {
    class Pattern {
    public:
        /**
         * The comparison operator compares types and then defers to doCompare for a more concrete implementation,
         * which should be - if necessary - implemented in subclasses. The default is a doCompare that returns true.
         */
        bool operator==(const Pattern &other) const noexcept;
        bool operator!=(const Pattern &other) const noexcept;

        /// Return the colour at a given point for this pattern.
        virtual const Colour colour_at(const Tuple&) const noexcept = 0;

    protected:
        /// Subclass comparison implementations should override this method.xs
        virtual bool doCompare(const Pattern &other) const noexcept;
    };
}
