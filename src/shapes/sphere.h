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
    class Sphere final: public Shape, public std::enable_shared_from_this<Sphere> {
    public:
        Sphere() = default;

        template<typename T>
        explicit Sphere(T&& t): Shape{t} {}

        template<typename T, typename M>
        Sphere(T&& t, M&& m): Shape{t, m} {}

        /// Create a specific instance of a glass sphere.
        static std::shared_ptr<Shape> createGlassSphere() noexcept;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}
