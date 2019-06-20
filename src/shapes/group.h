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
    class BoundingBox;
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

        template<typename T>
        void add(T& t) noexcept {
            shapes.emplace_back(t);
            t->setParent(shared_from_this());
        }

        template<typename... Ts>
        void addAll(Ts& ... ts) noexcept {
            (add(ts),... );
        }

        const std::vector<std::shared_ptr<Shape>> getShapes() const noexcept;

        /// We need to clear the shapes out of a group in order to not cause memory leaks.
        void clearShapes() noexcept;

        /// Get a bounding box.
        impl::BoundingBox bounds() const override;

    private:
        const std::vector<impl::Intersection> localIntersection(const impl::Ray&) const noexcept override;
        const Tuple localNormalAt(const Tuple&) const override;
    };
}



