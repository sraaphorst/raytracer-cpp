/**
 * cube.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <optional>
#include <tuple>
#include <vector>

#include "shape.h"
#include "vector.h"

namespace raytracer {
    class Intersection;
    class Ray;

    /// An Axis Aligned Bounding Box cube.
    class Cube: public Shape {
    public:
        Cube() = default;

        template <typename T>
        explicit Cube(T&& t): Shape{t} {}

        template <typename T, typename M>
        Cube(T&& t, M&& m): Shape{t, m} {}

    private:
        const std::vector<Intersection> localIntersection(const Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;

        /// Determine the intersections with a given axis. Defined only if the direction is not parallel.
        std::optional<std::pair<double, double>> checkAxis(const double origin, const double direction) const noexcept;
    };
}



