/**
 * cone.h
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
    /**
     * A double-napped cone, i.e. a cone with another cone perched above it, tip-to-tip, where the tips meet
     * at 0 and the cones extend in y infinitely.
     */
    class Cone final: public Shape {
    private:
        double minY;
        double maxY;
        bool capped;

    public:
        /// Uninstantiable outside of factory method.
        explicit Cone(dummy) noexcept;

        /// Factory method to create a cylinder of radius 1 that extends infinitely in y.
        static std::shared_ptr<Cone> createCone() noexcept;

        double getMinimumY() const noexcept;
        void setMinimumY(double) noexcept;
        double getMaximumY() const noexcept;
        void setMaximumY(double) noexcept;
        bool isCapped() const noexcept;
        void setCapped(bool) noexcept;

    private:
        bool checkCap(const impl::Ray&, double, double) const noexcept;
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
    };
}



