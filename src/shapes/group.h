/**
 * group.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "shape.h"
#include "vec.h"

namespace raytracer::impl {
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    /// A collection of other conglomerated shapes into a single object.
    class Group: public Shape {
        std::vector<std::shared_ptr<Shape>> shapes;

    public:
        /// Uninstantiable outside of factory method.
        explicit Group(dummy d) noexcept;

        /// Factory method to create a group.
        static std::shared_ptr<Group> createGroup() noexcept;

        template<typename... Ts>
        void addAll(Ts&& ... ts) {
            (shapes.emplace_back(std::forward<Ts>(ts)),... );
        }

        const std::vector<std::shared_ptr<Shape>> getShapes() const noexcept;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const noexcept override;
        std::pair<double, double> checkAxis(const double origin, const double direction) const noexcept;
    };
}



