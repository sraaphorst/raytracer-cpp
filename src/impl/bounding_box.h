/**
 * bounding_box.h
 *
 * By Sebastian Raaphorst, 2019.
 */

#pragma once

#include "affine_transform.h"
#include "vec.h"

namespace raytracer::impl {
    class BoundingBox final {
        Tuple min_point;
        Tuple max_point;

    public:
        /**
         * The initial bounding box is empty.
         */
        constexpr BoundingBox() noexcept:
            min_point{predefined_tuples::min_point}, max_point{predefined_tuples::max_point} {};

        constexpr BoundingBox(Tuple min_point, Tuple max_point) noexcept:
            min_point{min_point}, max_point{max_point} {}

        constexpr BoundingBox(const BoundingBox&) = default;
        constexpr BoundingBox(BoundingBox&&) = default;

        constexpr bool operator==(const BoundingBox &o) const noexcept {
            return min_point == o.min_point && max_point == o.max_point;
        }

        constexpr auto getMinPoint() const noexcept {
            return min_point;
        }
        constexpr auto getMaxPoint() const noexcept {
            return max_point;
        }

        /// Adds a point to a bounding box.
        void addPoint(const Tuple&) noexcept;

        /// Merges the given bounding box to the original.
        void addBox(const BoundingBox&) noexcept;

        /// Determines if the bounding box contains a point.
        bool containsPoint(const Tuple&) const noexcept;

        /// Determines if the bounding box contains another.
        bool containsBox(const BoundingBox&) const noexcept;

        /**
         * Apply a transformation to this bounding box to get another bounding box
         * that contains all eight transformed points of the original box.
         */
        BoundingBox transform(const Transformation&) const noexcept;
    };
}
