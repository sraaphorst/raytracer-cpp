/**
 * pattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "affine_transform.h"
#include "matrix.h"
#include "pattern.h"
#include "shape.h"

namespace raytracer {
    Pattern::Pattern(): transformation{predefined_matrices::I<>} {}
    Pattern::Pattern(const Transformation &t): transformation{t} {}
    Pattern::Pattern(Transformation &&t): transformation{std::move(t)} {}

    const Transformation &Pattern::getTransformation() const noexcept {
        return transformation;
    }
    void Pattern::setTransformation(const Transformation &t) noexcept {
        transformation = t;
    }
    void Pattern::setTransformation(Transformation &&t) noexcept {
        transformation = std::move(t);
    }

    bool Pattern::operator==(const Pattern &other) const noexcept {
        return typeid(*this) == typeid(other) && doCompare(other);
    }

    bool Pattern::operator!=(const raytracer::Pattern &other) const noexcept {
        return !(*this == other);
    }

    const Colour Pattern::colourAtObject(const Shape &shape,
            const Tuple &world_point) const noexcept {
        const auto object_point = shape.getTransformation().invert() * world_point;
        const auto pattern_point = transformation.invert() * object_point;
        return colourAt(pattern_point);
    }

    bool Pattern::doCompare(const Pattern &other) const noexcept {
        return true;
    }
}