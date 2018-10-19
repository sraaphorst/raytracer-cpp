/**
 * pattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>

#include "affine_transform.h"
#include "vector.h"

namespace raytracer {
    class Shape;

    class Pattern {
        Transformation transformation;
    public:
        Pattern();
        Pattern(const Transformation&);
        Pattern(Transformation&&);
        Pattern(const Pattern&) = default;
        Pattern(Pattern&&) = default;
        Pattern &operator=(const Pattern&) = default;
        Pattern &operator=(Pattern&&) = default;

        const Transformation &getTransformation() const noexcept;
        void setTransformation(const Transformation&) noexcept;
        void setTransformation(Transformation&&) noexcept;

        /**
         * The comparison operator compares types and then defers to doCompare for a more concrete implementation,
         * which should be - if necessary - implemented in subclasses. The default is a doCompare that returns true.
         */
        bool operator==(const Pattern &other) const noexcept;
        bool operator!=(const Pattern &other) const noexcept;

        /// Return the colour at a given point for this pattern.
        virtual const Colour colourAt(const Tuple &) const noexcept = 0;

        /// Computes the colour for a specific object at a given world point.
        const Colour colourAtObject(const Shape&, const Tuple&) const noexcept;

    protected:
        /// Subclass comparison implementations should override this method.xs
        virtual bool doCompare(const Pattern &other) const noexcept;
    };
}
