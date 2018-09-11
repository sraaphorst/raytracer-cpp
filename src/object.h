/**
 * object.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <optional>

#include "ray.h"

namespace raytracer {
    // We are using static polymorphism here. I'm not sure if this will help the issue of intersect needing a type,
    // but it probably won't hurt.
    template<typename T>
    class object {
    protected:
        object() = default;

    public:
        constexpr const std::optional<std::array<double, 2>> intersect(const Ray &r) const noexcept {
            return static_cast<T*>(this)->intersect(r);
        }
    };
}