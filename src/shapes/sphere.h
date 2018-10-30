/**
 * sphere.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <vector>

#include "shape.h"
#include "vec.h"

namespace raytracer::impl {
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    class Sphere final: public Shape {
    public:
        /// Uninstantiable outside of factory method.
        explicit Sphere(dummy) noexcept;

        /// Factory method to create a radius 1 sphere at the origin.
        static std::shared_ptr<Sphere> createSphere() noexcept;

        // TODO: MOVE OR DELETE
        /// Factory method to create a radius 1 glassy sphere at the origin.
        static std::shared_ptr<Sphere> createGlassSphere() noexcept;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}
