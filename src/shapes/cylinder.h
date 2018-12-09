/**
 * cylinder.h
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
    class Cylinder final: public Shape {
    public:
        /// Uninstantiable outside of factory method.
        explicit Cylinder(dummy) noexcept;

        /// Factory method to create a cylinder of radius 1 that extends infinitely in y.
        static std::shared_ptr<Cylinder> createCylinder() noexcept;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}




