/**
 * sphere.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <constmath.h>
#include <optional>
#include <tuple>

#include "object.h"
#include "ray.h"
#include "vector.h"

namespace raytracer {
    class Sphere final: public Object {
    public:

        constexpr Sphere() noexcept = default;
        ~Sphere() = default;

        constexpr const std::optional<std::array<double, 2>> intersect(const Ray &r) const noexcept override {
            const auto sphere_to_ray = r.getOrigin() - predefined_tuples::zero_point;
            const auto &direction = r.getDirection();

            const auto a = direction.dot_product(direction);
            const auto b = 2 * direction.dot_product(sphere_to_ray);
            const auto c = sphere_to_ray.dot_product(sphere_to_ray) - 1;

            const auto discriminant = b * b - 4 * a * c;
            if (discriminant < 0)
                return std::nullopt;

            const auto t1 = (-b - sqrtd(discriminant)) / (2 * a);
            const auto t2 = (-b + sqrtd(discriminant)) / (2 * a);
            if (t1 < t2) return std::make_optional(std::array<double,2>{t1, t2});
            else return std::make_optional(std::array<double,2>{t2, t1});
        }
    };
}
