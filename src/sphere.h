/**
 * sphere.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <optional>
#include <tuple>

#include "vector.h"

namespace raytracer {
    class Sphere final {
        constexpr Sphere() noexcept = default;
        ~Sphere();

        constexpr std::optional<std::pair<Tuple, Tuple>> intersect(const Ray &r) noexcept {
            return {predefined_tuples::zero_point, predefined_tuples::zero_point};
        }
    };
}
