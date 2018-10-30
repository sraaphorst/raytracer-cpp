/**
 * plane.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>

#include "shape.h"

namespace raytracer::impl {
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    class Plane final: public Shape, public std::enable_shared_from_this<Plane> {
    public:
        Plane() = default;

        template<typename T>
        explicit Plane(T&& t): Shape{t} {}

        template<typename T, typename M>
        Plane(T&& t, M&& m): Shape{t, m} {}

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}




