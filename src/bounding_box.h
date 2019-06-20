/**
 * bounding_box.h
 *
 * By Sebastian Raaphorst, 2019.
 */

#pragma once

#include "vec.h"

namespace raytracer {
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

        void addPoint(const Tuple &) noexcept;
    };
}
