/**
 * sphere.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <vector>

#include "shape.h"
#include "vector.h"

namespace raytracer {
    class Intersection;
    class Ray;

    class Sphere final: public Shape {
    public:
        Sphere() = default;

        template<typename T>
        explicit Sphere(T&& t): Shape{t} {}

        template<typename T, typename M>
        Sphere(T&& t, M&& m): Shape{t, m} {}

        /// Create a specific instance of a glassy sphere.
        const static std::shared_ptr<Shape> createGlassySphere() noexcept;

    private:
        const std::vector<Intersection> localIntersection(const Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}
