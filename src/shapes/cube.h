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

// Class forwards
namespace raytracer::impl {
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    /// An Axis Aligned Bounding Box cube.
    class Cube: public Shape, public std::enable_shared_from_this<Cube> {
    public:
        Cube() = default;

        template <typename T>
        explicit Cube(T&& t): Shape{t} {}

        template <typename T, typename M>
        Cube(T&& t, M&& m): Shape{t, m} {}

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
        std::pair<double, double> checkAxis(const double origin, const double direction) const noexcept;
    };
}



