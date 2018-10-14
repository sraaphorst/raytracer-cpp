/**
 * plane.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "shape.h"

namespace raytracer {
    class Plane final: public Shape {
    public:
        Plane() = default;

        template<typename T>
        explicit Plane(T&& t): Shape{t} {}

        template<typename T, typename M>
        Plane(T&& t, M&& m): Shape{t, m} {}

    private:
        const std::vector<Intersection> localIntersection(const Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}




