/**
 * cube.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "shape.h"
#include "vec.h"

namespace raytracer::impl {
    class BoundingBox;
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    /// An Axis Aligned Bounding Box cube.
    class Cube: public Shape {
    public:
        /// Uninstantiable outside of factory method.
        explicit Cube(dummy d) noexcept;

        /// Factory method to create a cube at the origin extending +/-1 along all axes.
        static std::shared_ptr<Cube> createCube() noexcept;

        /// Get a bounding box.
        impl::BoundingBox bounds() const override;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}



