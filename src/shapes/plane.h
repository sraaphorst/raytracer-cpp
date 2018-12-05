/**
 * plane.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "shape.h"

namespace raytracer::impl {
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    class Plane final: public Shape {
    public:
        /// Uninstantiable outside of factory method.
        explicit Plane(dummy) noexcept;

        /// Factory method to create an x-z plane through the origin.
        static std::shared_ptr<Plane> createPlane() noexcept;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}




